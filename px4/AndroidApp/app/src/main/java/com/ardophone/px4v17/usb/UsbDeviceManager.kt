package com.ardophone.px4v17.usb

import android.app.PendingIntent
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.hardware.usb.UsbConstants
import android.hardware.usb.UsbDevice
import android.hardware.usb.UsbDeviceConnection
import android.hardware.usb.UsbEndpoint
import android.hardware.usb.UsbManager
import android.os.Build
import android.util.Log
import com.ardophone.px4v17.bridge.PX4Bridge
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch

/**
 * UsbDeviceManager — Arduino USB Serial + GPS UBX + Telemetry
 * يكتشف أجهزة USB (CH340/CP2102/STM32/u-blox) عبر Android UsbManager
 * ويمرر file descriptor لـ C++ عبر JNI.
 *
 * يدعم أجهزة متعددة متصلة في نفس الوقت عبر USB Hub:
 *   - CH340        → arduino_pty_usb_bridge (إشارات المحركات عبر Arduino)
 *   - Arduino Mega → Java bulkTransfer مباشرة
 *   - u-blox       → gps_usb_ubx (بروتوكول UBX الثنائي)
 *   - CP2102/STM32 → MAVLink telemetry (radio via PTY bridge)
 */
class UsbDeviceManager(private val context: Context) {

    companion object {
        private const val TAG = "UsbDeviceMgr"
        private const val ACTION_USB_PERMISSION = "com.ardophone.px4v17.USB_PERMISSION"
        private const val RESCAN_INTERVAL_MS = 3000L  // إعادة مسح كل 3 ثوانٍ
        private const val MAX_RESCAN_ATTEMPTS = 20     // حد أقصى 20 محاولة (60 ثانية)

        // Arduino USB Serial baud rate
        const val ARDUINO_BAUD_RATE = 115200

        // Supported USB chips (VID)
        private const val VID_CH340   = 0x1A86  // QinHeng CH340 (Arduino Nano/Uno clones)
        private const val VID_ARDUINO = 0x2341  // Arduino SA (Mega 2560, Uno R3, etc.)
        private const val VID_CP2102  = 0x10C4  // Silicon Labs CP2102
        private const val VID_STM32   = 0x0483  // STMicroelectronics (CDC-ACM)
        private const val VID_UBLOX   = 0x1546  // u-blox AG (GPS direct USB)
        private const val VID_PROLIFIC = 0x067B // Prolific PL2303 (USB-Serial for GPS)

        // USB spec constant missing from Android SDK
        private const val USB_RECIP_INTERFACE = 0x01

        @Volatile private var sArduinoConn: UsbDeviceConnection? = null
        @Volatile private var sArduinoDevice: UsbDevice? = null

        // Cached endpoints — avoid searching every 20ms frame
        @Volatile private var cachedOutEp: UsbEndpoint? = null
        @Volatile private var cachedInEp: UsbEndpoint? = null

        // Failure tracking for auto-reconnect
        @Volatile private var sendCount: Long = 0
        @Volatile private var consecutiveFails: Int = 0
        @Volatile var rescanRequested: Boolean = false
            private set

        /** Reset Arduino connection state — called on disconnect or fatal error */
        fun resetArduinoConnection() {
            sArduinoConn = null
            sArduinoDevice = null
            cachedOutEp = null
            cachedInEp = null
            consecutiveFails = 0
            Log.i(TAG, "Arduino connection reset")
        }

        // Called from C++ arduino_output driver to send PWM via Java bulkTransfer
        @JvmStatic
        fun sendPwmDirect(data: ByteArray): Int {
            val conn = sArduinoConn ?: return -1
            val device = sArduinoDevice ?: return -1

            // Use cached endpoint — only search once after connection
            var outEp = cachedOutEp
            if (outEp == null) {
                val ifaceCount = device.interfaceCount
                for (i in 0 until ifaceCount) {
                    val iface = device.getInterface(i)
                    for (j in 0 until iface.endpointCount) {
                        val ep = iface.getEndpoint(j)
                        if (ep.direction == UsbConstants.USB_DIR_OUT &&
                            ep.type == UsbConstants.USB_ENDPOINT_XFER_BULK) {
                            outEp = ep
                            cachedOutEp = ep
                            Log.i(TAG, "Cached OUT bulk EP: 0x${Integer.toHexString(ep.address)}")
                            break
                        }
                    }
                    if (outEp != null) break
                }
                if (outEp == null) {
                    Log.e(TAG, "sendPwmDirect: no OUT bulk endpoint found!")
                    return -2
                }
            }

            val sent = try {
                conn.bulkTransfer(outEp, data, data.size, 100)
            } catch (e: Exception) {
                // Connection became invalid (device disconnected)
                Log.e(TAG, "bulkTransfer exception: ${e.message}")
                resetArduinoConnection()
                rescanRequested = true
                return -1
            }

            sendCount++

            if (sent < 0) {
                consecutiveFails++
                // After 150 consecutive failures (3s at 50Hz), invalidate + request rescan
                if (consecutiveFails == 150) {
                    Log.e(TAG, "150 consecutive send failures — invalidating connection, requesting rescan")
                    resetArduinoConnection()
                    rescanRequested = true
                }
            } else {
                if (consecutiveFails > 0) {
                    Log.i(TAG, "sendPwmDirect: recovered after $consecutiveFails failures")
                }
                consecutiveFails = 0
            }

            // Log every 500 calls (~10s at 50Hz) to reduce logcat noise
            if (sendCount % 500 == 0L) {
                Log.i(TAG, "sendPwmDirect: count=$sendCount last=$sent fails=$consecutiveFails")
            }
            return sent
        }

        /**
         * Read data from Arduino via USB IN bulk endpoint.
         * Returns the string received, or null if nothing available.
         */
        @JvmStatic
        fun readFromArduino(): String? {
            val conn = sArduinoConn ?: return null
            val device = sArduinoDevice ?: return null

            var inEp = cachedInEp
            if (inEp == null) {
                val ifaceCount = device.interfaceCount
                for (i in 0 until ifaceCount) {
                    val iface = device.getInterface(i)
                    for (j in 0 until iface.endpointCount) {
                        val ep = iface.getEndpoint(j)
                        if (ep.direction == UsbConstants.USB_DIR_IN &&
                            ep.type == UsbConstants.USB_ENDPOINT_XFER_BULK) {
                            inEp = ep
                            cachedInEp = ep
                            Log.i(TAG, "Cached IN bulk EP: 0x${Integer.toHexString(ep.address)}")
                            break
                        }
                    }
                    if (inEp != null) break
                }
                if (inEp == null) return null
            }

            val buf = ByteArray(256)
            val received = try {
                conn.bulkTransfer(inEp, buf, buf.size, 50)
            } catch (_: Exception) {
                return null
            }
            return if (received > 0) String(buf, 0, received, Charsets.UTF_8) else null
        }
    }

    private val usbManager: UsbManager =
        context.getSystemService(Context.USB_SERVICE) as UsbManager

    // دعم أجهزة متعددة — كل جهاز له connection خاص
    private val connections = mutableMapOf<Int, UsbDeviceConnection>()  // deviceId → connection
    private val devices = mutableMapOf<Int, UsbDevice>()               // deviceId → device
    private var started = false
    @Volatile private var rescanThread: Thread? = null

    // IO scope لفتح أجهزة USB خارج الـmain thread (controlTransfer + sleep → ANR إن استُدعي على main)
    private val ioScope = CoroutineScope(SupervisorJob() + Dispatchers.IO)

    private val usbReceiver = object : BroadcastReceiver() {
        override fun onReceive(ctx: Context, intent: Intent) {
            when (intent.action) {
                UsbManager.ACTION_USB_DEVICE_ATTACHED -> {
                    val device = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE, UsbDevice::class.java)
                    } else {
                        @Suppress("DEPRECATION")
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE)
                    }
                    device?.let { onDeviceAttached(it) }
                }

                UsbManager.ACTION_USB_DEVICE_DETACHED -> {
                    val device = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE, UsbDevice::class.java)
                    } else {
                        @Suppress("DEPRECATION")
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE)
                    }
                    device?.let { onDeviceDetached(it) }
                }

                ACTION_USB_PERMISSION -> {
                    val granted = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)
                    val device = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE, UsbDevice::class.java)
                    } else {
                        @Suppress("DEPRECATION")
                        intent.getParcelableExtra(UsbManager.EXTRA_DEVICE)
                    }
                    if (granted && device != null) {
                        Log.i(TAG, "USB permission granted for ${device.deviceName}")
                        ioScope.launch { openDevice(device) }
                    } else {
                        Log.w(TAG, "USB permission denied")
                    }
                }
            }
        }
    }

    fun start() {
        if (started) return
        started = true

        val filter = IntentFilter().apply {
            addAction(UsbManager.ACTION_USB_DEVICE_ATTACHED)
            addAction(UsbManager.ACTION_USB_DEVICE_DETACHED)
            addAction(ACTION_USB_PERMISSION)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            context.registerReceiver(usbReceiver, filter, Context.RECEIVER_NOT_EXPORTED)
        } else {
            context.registerReceiver(usbReceiver, filter)
        }

        Log.i(TAG, "UsbDeviceManager started — scanning for devices...")
        ioScope.launch { scanExistingDevices() }
        startRescanIfArduinoMissing()
    }

    fun stop() {
        if (!started) return
        started = false
        rescanThread?.interrupt()
        rescanThread = null

        try {
            context.unregisterReceiver(usbReceiver)
        } catch (_: Exception) {}

        closeAllDevices()
        ioScope.cancel()
        Log.i(TAG, "UsbDeviceManager stopped")
    }

    /**
     * إذا لم يُكتشف Arduino في المسح الأول، أعد المحاولة كل 3 ثوانٍ
     * حتى يُكتشف أو تنتهي المحاولات (60 ثانية كحد أقصى)
     */
    private fun startRescanIfArduinoMissing() {
        if (sArduinoConn != null) {
            Log.i(TAG, "Arduino already connected — no rescan needed")
            return
        }
        rescanThread = Thread {
            var attempts = 0
            try {
                // Phase 1: Initial scan (up to 60s at startup)
                while (started && sArduinoConn == null && attempts < MAX_RESCAN_ATTEMPTS) {
                    Thread.sleep(RESCAN_INTERVAL_MS)
                    attempts++
                    rescanOnce(attempts, MAX_RESCAN_ATTEMPTS)
                }
                if (sArduinoConn != null) {
                    Log.i(TAG, "Arduino found after $attempts rescan attempts!")
                } else {
                    Log.w(TAG, "Arduino NOT found after $MAX_RESCAN_ATTEMPTS rescan attempts")
                }

                // Phase 2: Monitor for disconnect/reconnect via rescanRequested flag
                while (started) {
                    Thread.sleep(RESCAN_INTERVAL_MS)
                    if (rescanRequested) {
                        Log.i(TAG, "Rescan requested (send failures detected) — searching for Arduino...")
                        rescanRequested = false
                        for (retries in 1..10) {
                            rescanOnce(retries, 10)
                            if (sArduinoConn != null) break
                            Thread.sleep(RESCAN_INTERVAL_MS)
                        }
                    }
                }
            } catch (_: InterruptedException) {
                Log.i(TAG, "Rescan thread interrupted")
            }
        }.apply {
            name = "UsbRescan"
            isDaemon = true
            start()
        }
    }

    /** Single rescan sweep — find and attach Arduino if present */
    private fun rescanOnce(attempt: Int, maxAttempts: Int) {
        val devList = usbManager.deviceList
        Log.i(TAG, "Rescan attempt $attempt/$maxAttempts — visible devices: ${devList.size}")
        for ((name, device) in devList) {
            Log.i(TAG, "  device: $name VID=0x${Integer.toHexString(device.vendorId)} PID=0x${Integer.toHexString(device.productId)} class=${device.deviceClass}")
            if ((device.vendorId == VID_CH340 || device.vendorId == VID_ARDUINO) &&
                !devices.containsKey(device.deviceId)) {
                Log.i(TAG, "Rescan found Arduino: ${deviceInfo(device)}")
                ioScope.launch { onDeviceAttached(device) }
            }
        }
    }

    private fun scanExistingDevices() {
        for ((_, device) in usbManager.deviceList) {
            if (isSupportedDevice(device)) {
                Log.i(TAG, "Found existing device: ${deviceInfo(device)}")
                onDeviceAttached(device)
            }
        }
    }

    private fun onDeviceAttached(device: UsbDevice) {
        if (!isSupportedDevice(device)) return
        if (devices.containsKey(device.deviceId)) {
            Log.w(TAG, "Device already connected: ${device.deviceName}")
            return
        }

        Log.i(TAG, "Device attached: ${deviceInfo(device)}")

        if (usbManager.hasPermission(device)) {
            ioScope.launch { openDevice(device) }
        } else {
            Log.i(TAG, "Requesting USB permission for ${deviceInfo(device)}...")
            val flags = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                PendingIntent.FLAG_UPDATE_CURRENT or PendingIntent.FLAG_MUTABLE
            } else {
                PendingIntent.FLAG_UPDATE_CURRENT
            }
            val pi = PendingIntent.getBroadcast(context, device.deviceId,
                Intent(ACTION_USB_PERMISSION).setPackage(context.packageName), flags)
            usbManager.requestPermission(device, pi)
        }
    }

    private fun onDeviceDetached(device: UsbDevice) {
        if (!devices.containsKey(device.deviceId)) return
        Log.i(TAG, "Device detached: ${deviceInfo(device)}")
        closeDevice(device.deviceId)
    }

    private fun openDevice(device: UsbDevice) {
        val conn = usbManager.openDevice(device)
        if (conn == null) {
            Log.e(TAG, "Failed to open USB device: ${deviceInfo(device)}")
            return
        }

        val fd = conn.fileDescriptor
        if (fd < 0) {
            Log.e(TAG, "Invalid file descriptor for ${deviceInfo(device)}")
            conn.close()
            return
        }

        connections[device.deviceId] = conn
        devices[device.deviceId] = device

        // Route FD based on chip type:
        // CH340/Arduino → Arduino actuator output bridge
        // u-blox        → gps_usb_ubx (UBX binary protocol)
        // CP2102/STM32  → MAVLink telemetry (radio via PTY bridge)
        when (device.vendorId) {
            VID_CH340, VID_ARDUINO -> {
                PX4Bridge.setArduinoUsbFd(fd, ARDUINO_BAUD_RATE)
                val chipName = if (device.vendorId == VID_ARDUINO) "Arduino Mega/Uno" else "CH340"
                Log.i(TAG, "$chipName opened: ${deviceInfo(device)}, fd=$fd")

                // Step 1: Claim control IF0 first (CDC-ACM control interface)
                if (device.interfaceCount > 1) {
                    val ctrlIface = device.getInterface(0)
                    if (conn.claimInterface(ctrlIface, true)) {
                        Log.i(TAG, "Claimed control IF 0 for CDC-ACM")
                    }
                }

                // Step 2: Find and claim the data interface with bulk OUT endpoint
                var claimed = false
                val ifaceCount = device.interfaceCount
                for (i in 0 until ifaceCount) {
                    val iface = device.getInterface(i)
                    for (j in 0 until iface.endpointCount) {
                        val ep = iface.getEndpoint(j)
                        if (ep.direction == UsbConstants.USB_DIR_OUT &&
                            ep.type == UsbConstants.USB_ENDPOINT_XFER_BULK) {
                            if (conn.claimInterface(iface, true)) {
                                Log.i(TAG, "Claimed IF $i for $chipName bulkTransfer (EP 0x${Integer.toHexString(ep.address)})")
                                claimed = true
                            }
                            break
                        }
                    }
                    if (claimed) break
                }

                // Step 3: CDC-ACM init (for genuine Arduino with ATmega16U2; harmless no-op for CH340)
                if (claimed) {
                    try {
                        // SET_LINE_CODING: 115200 baud, 8N1
                        val lineCoding = byteArrayOf(
                            0x00.toByte(), 0xC2.toByte(), 0x01.toByte(), 0x00.toByte(),
                            0x00.toByte(), 0x00.toByte(), 0x08.toByte()
                        )
                        val result = conn.controlTransfer(
                            UsbConstants.USB_DIR_OUT or
                            UsbConstants.USB_TYPE_CLASS or
                            USB_RECIP_INTERFACE,
                            0x20, 0, 0, lineCoding, lineCoding.size, 1000
                        )
                        Log.i(TAG, "CDC-ACM SET_LINE_CODING: ${if (result >= 0) "OK" else "failed ($result, OK for CH340)"}")

                        // SET_CONTROL_LINE_STATE: DTR=1, RTS=1
                        val dtrResult = conn.controlTransfer(
                            UsbConstants.USB_DIR_OUT or
                            UsbConstants.USB_TYPE_CLASS or
                            USB_RECIP_INTERFACE,
                            0x22, 0x0003, 0, null, 0, 1000
                        )
                        Log.i(TAG, "CDC-ACM SET_CONTROL_LINE_STATE: ${if (dtrResult >= 0) "OK" else "failed ($dtrResult, OK for CH340)"}")

                        // Step 3b: DTR toggle — triggers Arduino auto-reset for clean start
                        Thread.sleep(100)
                        conn.controlTransfer(
                            UsbConstants.USB_DIR_OUT or UsbConstants.USB_TYPE_CLASS or USB_RECIP_INTERFACE,
                            0x22, 0x00, 0, null, 0, 1000  // DTR=0 (falling edge = reset)
                        )
                        Thread.sleep(50)
                        conn.controlTransfer(
                            UsbConstants.USB_DIR_OUT or UsbConstants.USB_TYPE_CLASS or USB_RECIP_INTERFACE,
                            0x22, 0x03, 0, null, 0, 1000  // DTR=1 (release reset)
                        )
                        Log.i(TAG, "Arduino DTR reset toggle done — waiting 2.5s for reboot + ESC init")
                        Thread.sleep(2500)  // Arduino setup() takes ~1.5s (ESC boot delay)
                    } catch (e: Exception) {
                        Log.w(TAG, "CDC-ACM init exception: ${e.message} (OK for CH340)")
                    }
                }

                // CH340 specific init: set baud rate via vendor control transfer
                if (device.vendorId == VID_CH340 && claimed) {
                    try {
                        // CH340 init sequence
                        conn.controlTransfer(0x40, 0xA1, 0, 0, null, 0, 1000) // init
                        conn.controlTransfer(0x40, 0x9A, 0x2518, 0x0050, null, 0, 1000) // set baud (115200)
                        conn.controlTransfer(0x40, 0xA4, 0x00FF, 0x0000, null, 0, 1000) // handshake
                        conn.controlTransfer(0x40, 0xA1, 0x501F, 0xD90A, null, 0, 1000) // init2
                        Log.i(TAG, "CH340 vendor init complete (115200 baud)")
                    } catch (e: Exception) {
                        Log.w(TAG, "CH340 vendor init exception: ${e.message}")
                    }
                }

                // Step 4: NOW make connection visible to sendPwmDirect
                // CRITICAL: Must be AFTER CDC init, otherwise data arrives before Arduino is ready
                sArduinoConn = conn
                sArduinoDevice = device
                cachedOutEp = null   // force fresh endpoint lookup
                cachedInEp = null
                Log.i(TAG, "Arduino connection READY for PWM data (CDC init complete, VID=0x${Integer.toHexString(device.vendorId)})")

                if (!claimed) {
                    Log.w(TAG, "Failed to claim any interface for $chipName bulkTransfer")
                }
            }
            VID_UBLOX, VID_PROLIFIC -> {
                PX4Bridge.setGpsUsbFd(fd)
                Log.i(TAG, "GPS UBX opened: ${deviceInfo(device)}, fd=$fd")
            }
            VID_CP2102, VID_STM32 -> {
                PX4Bridge.setMavlinkTelemetryUsbFd(fd)
                Log.i(TAG, "CP210x/STM32 → MAVLink telemetry, fd=$fd")
            }
            else -> {
                Log.w(TAG, "Unknown device: ${deviceInfo(device)} — ignoring")
            }
        }
    }

    private fun closeDevice(deviceId: Int) {
        val device = devices[deviceId]

        // Tell C++ to release the USB fd
        device?.let {
            when (it.vendorId) {
                VID_CH340, VID_ARDUINO -> PX4Bridge.setArduinoUsbFd(-1, 0)
                VID_UBLOX, VID_PROLIFIC -> PX4Bridge.setGpsUsbFd(-1)
                VID_CP2102, VID_STM32   -> PX4Bridge.setMavlinkTelemetryUsbFd(-1)
                else                    -> { /* no-op */ }
            }
        }

        connections[deviceId]?.close()
        connections.remove(deviceId)
        devices.remove(deviceId)

        // Clear static Arduino references to prevent stale connection usage
        if (device != null &&
            (device.vendorId == VID_CH340 || device.vendorId == VID_ARDUINO)) {
            resetArduinoConnection()
        }
    }

    private fun closeAllDevices() {
        val ids = devices.keys.toList()
        for (id in ids) {
            closeDevice(id)
        }
    }

    private fun isSupportedDevice(device: UsbDevice): Boolean {
        return device.vendorId == VID_CH340 ||
               device.vendorId == VID_ARDUINO ||
               device.vendorId == VID_CP2102 ||
               device.vendorId == VID_STM32 ||
               device.vendorId == VID_UBLOX ||
               device.vendorId == VID_PROLIFIC
    }

    private fun deviceInfo(device: UsbDevice): String {
        val chip = when (device.vendorId) {
            VID_CH340   -> "CH340 (Arduino clone)"
            VID_ARDUINO -> "Arduino Mega/Uno"
            VID_CP2102  -> "CP2102"
            VID_STM32   -> "STM32"
            VID_UBLOX   -> "u-blox"
            VID_PROLIFIC -> "PL2303 (GPS)"
            else -> "Unknown"
        }
        return "$chip (VID=0x${device.vendorId.toString(16)}, PID=0x${device.productId.toString(16)})"
    }
}
