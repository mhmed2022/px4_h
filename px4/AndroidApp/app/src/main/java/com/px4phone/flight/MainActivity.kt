package com.ardophone.px4v17

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalLayoutDirection
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.LayoutDirection
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.core.content.ContextCompat
import com.ardophone.px4v17.bridge.PX4Bridge
import com.ardophone.px4v17.service.FlightService
import com.ardophone.px4v17.state.BridgeState
import com.ardophone.px4v17.storage.BlackBoxPreferences
import com.ardophone.px4v17.storage.SafBlackBoxMirror
import com.ardophone.px4v17.ui.theme.PX4PhoneTheme
import androidx.compose.ui.platform.LocalView
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.isActive
import kotlinx.coroutines.withContext

class MainActivity : ComponentActivity() {

    private val permissionLauncher = registerForActivityResult(
        ActivityResultContracts.RequestMultiplePermissions()
    ) { /* results handled silently */ }

    // SAF: user picks USB flash drive folder
    val safLauncher = registerForActivityResult(
        ActivityResultContracts.OpenDocumentTree()
    ) { uri ->
        if (uri != null) {
            contentResolver.takePersistableUriPermission(
                uri,
                Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_WRITE_URI_PERMISSION
            )
            BlackBoxPreferences.setTreeUri(this, uri)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        requestPermissions()
        enableEdgeToEdge()
        setContent {
            PX4PhoneTheme {
                PX4PhoneScreen()
            }
        }
    }

    private fun requestPermissions() {
        val permissions = mutableListOf(
            Manifest.permission.ACCESS_FINE_LOCATION,
            Manifest.permission.ACCESS_COARSE_LOCATION
        )
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            permissions.add(Manifest.permission.POST_NOTIFICATIONS)
        }
        val needed = permissions.filter {
            ContextCompat.checkSelfPermission(this, it) != PackageManager.PERMISSION_GRANTED
        }
        if (needed.isNotEmpty()) {
            permissionLauncher.launch(needed.toTypedArray())
        }
    }
}

@Composable
fun PX4PhoneScreen() {
    val context = LocalContext.current
    val isRunning by BridgeState.running.collectAsState()

    var motorPwm1 by remember { mutableIntStateOf(1000) }
    var motorPwm2 by remember { mutableIntStateOf(1000) }
    var motorPwm3 by remember { mutableIntStateOf(1000) }
    var motorPwm4 by remember { mutableIntStateOf(1000) }
    var motorUsbConnected by remember { mutableStateOf(false) }
    var armed by remember { mutableStateOf(false) }
    var ekfStatus by remember { mutableIntStateOf(0) }
    var airframeId by remember { mutableIntStateOf(0) }
    var prevArmed by remember { mutableStateOf(false) }

    var imuRate by remember { mutableLongStateOf(0) }
    var baroRate by remember { mutableLongStateOf(0) }
    var magRate by remember { mutableLongStateOf(0) }
    var gpsRate by remember { mutableLongStateOf(0) }

    val hasAccel by BridgeState.hasAccel.collectAsState()
    val hasGyro by BridgeState.hasGyro.collectAsState()
    val hasMag by BridgeState.hasMag.collectAsState()
    val hasBaro by BridgeState.hasBaro.collectAsState()

    // FPS counter — measures actual UI frame rate via Choreographer
    var fps by remember { mutableIntStateOf(0) }
    val view = LocalView.current
    DisposableEffect(Unit) {
        var frameCount = 0
        var lastTime = System.nanoTime()
        val choreographer = android.view.Choreographer.getInstance()
        val callback = object : android.view.Choreographer.FrameCallback {
            override fun doFrame(frameTimeNanos: Long) {
                frameCount++
                val elapsed = frameTimeNanos - lastTime
                if (elapsed >= 1_000_000_000L) {
                    fps = frameCount
                    frameCount = 0
                    lastTime = frameTimeNanos
                }
                choreographer.postFrameCallback(this)
            }
        }
        choreographer.postFrameCallback(callback)
        onDispose { choreographer.removeFrameCallback(callback) }
    }

    // Poll PX4 state on background thread (JNI calls no longer acquire uORB semaphores,
    // but keeping them off the main thread avoids any JNI overhead on the UI)
    LaunchedEffect(isRunning) {
        if (!isRunning) return@LaunchedEffect
        withContext(Dispatchers.Default) {
            while (true) {
                try {
                    val arm = PX4Bridge.isArmed()
                    val ekf = PX4Bridge.getEKFStatus()
                    val af = PX4Bridge.getAirframeId()
                    val p1 = PX4Bridge.getMotorPwm1()
                    val p2 = PX4Bridge.getMotorPwm2()
                    val p3 = PX4Bridge.getMotorPwm3()
                    val p4 = PX4Bridge.getMotorPwm4()
                    val usbOk = PX4Bridge.isMotorUsbConnected()
                    withContext(Dispatchers.Main) {
                        motorPwm1 = p1; motorPwm2 = p2; motorPwm3 = p3; motorPwm4 = p4
                        motorUsbConnected = usbOk
                        armed = arm; ekfStatus = ekf; airframeId = af
                        if (arm != prevArmed) {
                            val msg = if (arm) "ARMED — المحركات مسلّحة" else "DISARMED — المحركات متوقفة"
                            android.widget.Toast.makeText(context, msg, android.widget.Toast.LENGTH_LONG).show()
                            prevArmed = arm
                        }
                    }
                } catch (_: Exception) { }
                delay(200)
            }
        }
    }

    // === قراءة رسائل الأردوينو (ARM/DISARM) عبر USB ===
    LaunchedEffect(isRunning) {
        if (!isRunning) return@LaunchedEffect
        var arduinoLineBuf = StringBuilder()
        withContext(Dispatchers.Default) {
            while (true) {
                try {
                    val chunk = com.ardophone.px4v17.usb.UsbDeviceManager.readFromArduino()
                    if (chunk != null) {
                        arduinoLineBuf.append(chunk)
                        var newline = arduinoLineBuf.indexOf("\n")
                        while (newline >= 0) {
                            val line = arduinoLineBuf.substring(0, newline).trim()
                            arduinoLineBuf.delete(0, newline + 1)
                            if (line == "ARMED") {
                                withContext(Dispatchers.Main) {
                                    android.widget.Toast.makeText(context, "✅ الأردوينو استلم أمر التسليح — ARMED", android.widget.Toast.LENGTH_LONG).show()
                                }
                            } else if (line == "DISARMED") {
                                withContext(Dispatchers.Main) {
                                    android.widget.Toast.makeText(context, "⛔ الأردوينو استلم أمر إيقاف التسليح — DISARMED", android.widget.Toast.LENGTH_LONG).show()
                                }
                            }
                            newline = arduinoLineBuf.indexOf("\n")
                        }
                        if (arduinoLineBuf.length > 1024) arduinoLineBuf.clear()
                    }
                } catch (_: Exception) { }
                delay(100)
            }
        }
    }

    // Poll sensor rates on background thread
    LaunchedEffect(isRunning) {
        if (!isRunning) return@LaunchedEffect
        withContext(Dispatchers.Default) {
            var prevImu = 0L; var prevBaro = 0L; var prevMag = 0L; var prevGps = 0L
            while (true) {
                delay(1000)
                val curImu = PX4Bridge.getNativeImuCount()
                val curBaro = PX4Bridge.getNativeBaroCount()
                val curMag = PX4Bridge.getNativeMagCount()
                val curGps = PX4Bridge.getNativeGpsCount()
                withContext(Dispatchers.Main) {
                    imuRate = curImu - prevImu
                    baroRate = curBaro - prevBaro
                    magRate = curMag - prevMag
                    gpsRate = curGps - prevGps
                }
                prevImu = curImu; prevBaro = curBaro; prevMag = curMag; prevGps = curGps
            }
        }
    }

    Scaffold(
        modifier = Modifier.fillMaxSize(),
        containerColor = MaterialTheme.colorScheme.background
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(innerPadding)
                .padding(horizontal = 16.dp)
                .verticalScroll(rememberScrollState()),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Spacer(Modifier.height(8.dp))

            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Column(horizontalAlignment = Alignment.CenterHorizontally, modifier = Modifier.weight(1f)) {
                    Text(
                        text = "PX4 v1.17",
                        fontSize = 28.sp,
                        fontWeight = FontWeight.Bold,
                        color = MaterialTheme.colorScheme.primary
                    )
                    Text(
                        text = "PX4 Autopilot v1.17.0-alpha1",
                        fontSize = 12.sp,
                        color = MaterialTheme.colorScheme.onSurfaceVariant
                    )
                }
                Column(horizontalAlignment = Alignment.End) {
                    Text(
                        text = if (airframeId != 0) "$airframeId" else "---",
                        fontSize = 16.sp,
                        fontFamily = FontFamily.Monospace,
                        fontWeight = FontWeight.Bold,
                        color = MaterialTheme.colorScheme.primary
                    )
                    Text(
                        text = "${fps}fps",
                        fontSize = 11.sp,
                        fontFamily = FontFamily.Monospace,
                        color = when {
                            fps >= 50 -> Color(0xFF4CAF50)
                            fps >= 30 -> Color(0xFFFFC107)
                            else -> Color(0xFFF44336)
                        }
                    )
                }
            }
            Spacer(Modifier.height(16.dp))

            MotorsCard(motorPwm1, motorPwm2, motorPwm3, motorPwm4, motorUsbConnected)
            Spacer(Modifier.height(12.dp))

            SensorStatusCard(
                hasAccel, hasGyro, hasMag, hasBaro,
                imuRate, baroRate, magRate, gpsRate
            )
            Spacer(Modifier.height(12.dp))

            FlightStatusCard(armed, ekfStatus)
            Spacer(Modifier.height(12.dp))

            ConnectionCard()
            Spacer(Modifier.height(12.dp))

            StorageCard(context)
            Spacer(Modifier.height(16.dp))

            Button(
                onClick = {
                    val intent = Intent(context, FlightService::class.java)
                    if (isRunning) {
                        intent.action = FlightService.ACTION_STOP
                        context.startService(intent)
                    } else {
                        intent.action = FlightService.ACTION_START
                        ContextCompat.startForegroundService(context, intent)
                    }
                },
                modifier = Modifier
                    .fillMaxWidth()
                    .height(56.dp),
                colors = ButtonDefaults.buttonColors(
                    containerColor = if (isRunning)
                        MaterialTheme.colorScheme.error
                    else
                        MaterialTheme.colorScheme.primary
                ),
                shape = RoundedCornerShape(12.dp)
            ) {
                Text(
                    text = if (isRunning) "Stop PX4" else "Start PX4",
                    fontSize = 18.sp,
                    fontWeight = FontWeight.Bold
                )
            }
            Spacer(Modifier.height(24.dp))
        }
    }
}

@Composable
private fun MotorsCard(p1: Int, p2: Int, p3: Int, p4: Int, usbOk: Boolean) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text("Motors PWM", fontWeight = FontWeight.Bold, fontSize = 16.sp)
                Text(
                    if (usbOk) "USB TX: OK" else "USB TX: FAIL",
                    fontSize = 12.sp,
                    fontFamily = FontFamily.Monospace,
                    color = if (usbOk) Color(0xFF4CAF50) else Color(0xFFF44336)
                )
            }
            Spacer(Modifier.height(8.dp))
            CompositionLocalProvider(LocalLayoutDirection provides LayoutDirection.Ltr) {
                Text(
                    "Front / أمام",
                    modifier = Modifier.fillMaxWidth(),
                    textAlign = androidx.compose.ui.text.style.TextAlign.Center,
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
                Spacer(Modifier.height(4.dp))
                Row(modifier = Modifier.fillMaxWidth()) {
                    MotorPwmValue("M3", "أمام يسار", p3, Modifier.weight(1f))
                    MotorPwmValue("M1", "أمام يمين", p1, Modifier.weight(1f))
                }
                Spacer(Modifier.height(4.dp))
                Row(modifier = Modifier.fillMaxWidth()) {
                    MotorPwmValue("M2", "خلف يسار", p2, Modifier.weight(1f))
                    MotorPwmValue("M4", "خلف يمين", p4, Modifier.weight(1f))
                }
                Spacer(Modifier.height(4.dp))
                Text(
                    "Rear / خلف",
                    modifier = Modifier.fillMaxWidth(),
                    textAlign = androidx.compose.ui.text.style.TextAlign.Center,
                    fontSize = 12.sp,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
            }
        }
    }
}

@Composable
private fun MotorPwmValue(label: String, position: String, pwm: Int, modifier: Modifier) {
    val clampedPwm = pwm.coerceIn(900, 2100)
    val normalized = ((clampedPwm - 1000).toFloat() / 1000f).coerceIn(0f, 1f)
    Column(modifier = modifier.padding(horizontal = 4.dp), horizontalAlignment = Alignment.CenterHorizontally) {
        Text(label, fontSize = 12.sp, color = MaterialTheme.colorScheme.onSurfaceVariant)
        Text(position, fontSize = 11.sp, color = MaterialTheme.colorScheme.onSurfaceVariant)
        Text(
            text = "%4dus".format(clampedPwm),
            fontSize = 20.sp,
            fontWeight = FontWeight.Bold,
            fontFamily = FontFamily.Monospace
        )
        LinearProgressIndicator(
            progress = { normalized },
            modifier = Modifier
                .fillMaxWidth()
                .height(8.dp),
            trackColor = MaterialTheme.colorScheme.surfaceVariant,
            color = if (normalized > 0.02f) MaterialTheme.colorScheme.primary else Color.Gray
        )
    }
}

@Composable
private fun SensorStatusCard(
    hasAccel: Boolean, hasGyro: Boolean, hasMag: Boolean, hasBaro: Boolean,
    imuRate: Long, baroRate: Long, magRate: Long, gpsRate: Long
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            Text("Sensors", fontWeight = FontWeight.Bold, fontSize = 16.sp)
            Spacer(Modifier.height(8.dp))
            Row(modifier = Modifier.fillMaxWidth()) {
                SensorItem("IMU", hasAccel && hasGyro, "${imuRate}Hz", Modifier.weight(1f))
                SensorItem("Baro", hasBaro, "${baroRate}Hz", Modifier.weight(1f))
                SensorItem("Mag", hasMag, "${magRate}Hz", Modifier.weight(1f))
                SensorItem("GPS", true, "${gpsRate}Hz", Modifier.weight(1f))
            }
        }
    }
}

@Composable
private fun SensorItem(name: String, available: Boolean, rate: String, modifier: Modifier) {
    Column(modifier = modifier, horizontalAlignment = Alignment.CenterHorizontally) {
        Text(name, fontSize = 12.sp, fontWeight = FontWeight.Medium)
        StatusDot(if (available) StatusColor.GREEN else StatusColor.RED)
        Text(rate, fontSize = 11.sp, fontFamily = FontFamily.Monospace,
            color = MaterialTheme.colorScheme.onSurfaceVariant)
    }
}

@Composable
private fun FlightStatusCard(armed: Boolean, ekfStatus: Int) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            Text("Flight Status", fontWeight = FontWeight.Bold, fontSize = 16.sp)
            Spacer(Modifier.height(8.dp))
            Row(modifier = Modifier.fillMaxWidth()) {
                Column(Modifier.weight(1f), horizontalAlignment = Alignment.CenterHorizontally) {
                    Text("Vehicle", fontSize = 12.sp, color = MaterialTheme.colorScheme.onSurfaceVariant)
                    Text("PX4", fontSize = 16.sp, fontWeight = FontWeight.Bold,
                        fontFamily = FontFamily.Monospace)
                }
                Column(Modifier.weight(1f), horizontalAlignment = Alignment.CenterHorizontally) {
                    Text("Armed", fontSize = 12.sp, color = MaterialTheme.colorScheme.onSurfaceVariant)
                    StatusDot(if (armed) StatusColor.RED else StatusColor.GREEN)
                    Text(if (armed) "YES" else "NO", fontSize = 12.sp, fontFamily = FontFamily.Monospace)
                }
                Column(Modifier.weight(1f), horizontalAlignment = Alignment.CenterHorizontally) {
                    Text("EKF", fontSize = 12.sp, color = MaterialTheme.colorScheme.onSurfaceVariant)
                    StatusDot(
                        when (ekfStatus) {
                            2 -> StatusColor.GREEN
                            1 -> StatusColor.YELLOW
                            else -> StatusColor.RED
                        }
                    )
                    Text(
                        when (ekfStatus) { 2 -> "Good"; 1 -> "OK"; else -> "Bad" },
                        fontSize = 12.sp, fontFamily = FontFamily.Monospace
                    )
                }
            }
        }
    }
}

@Composable
private fun ConnectionCard() {
    Card(
        modifier = Modifier.fillMaxWidth(),
        shape = RoundedCornerShape(12.dp),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surfaceVariant
        )
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            Text("MAVLink", fontWeight = FontWeight.Bold, fontSize = 16.sp)
            Spacer(Modifier.height(4.dp))
            Text(
                "UDP:14550",
                fontSize = 14.sp, fontFamily = FontFamily.Monospace,
                color = MaterialTheme.colorScheme.primary
            )
            Text(
                "QGC: Add Comm Link → UDP → Phone IP:14550",
                fontSize = 12.sp, color = MaterialTheme.colorScheme.onSurfaceVariant
            )
        }
    }
}

@Composable
private fun StorageCard(context: android.content.Context) {
    val activity = context as? MainActivity
    var hasUri by remember { mutableStateOf(BlackBoxPreferences.getTreeUri(context) != null) }
    val mirrorActive = SafBlackBoxMirror.isScheduled()

    Card(
        modifier = Modifier.fillMaxWidth(),
        shape = RoundedCornerShape(12.dp),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surfaceVariant
        )
    ) {
        Column(modifier = Modifier.padding(16.dp)) {
            Text("USB Storage", fontWeight = FontWeight.Bold, fontSize = 16.sp)
            Spacer(Modifier.height(4.dp))

            if (hasUri) {
                Row(verticalAlignment = Alignment.CenterVertically) {
                    Box(
                        modifier = Modifier
                            .size(10.dp)
                            .background(
                                if (mirrorActive) Color(0xFF4CAF50) else Color(0xFFFFC107),
                                RoundedCornerShape(5.dp)
                            )
                    )
                    Spacer(Modifier.width(8.dp))
                    Text(
                        text = if (mirrorActive) "Mirror active" else "USB linked",
                        fontSize = 13.sp,
                        fontFamily = FontFamily.Monospace,
                        color = MaterialTheme.colorScheme.onSurfaceVariant
                    )
                }
                Spacer(Modifier.height(8.dp))
                Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
                    OutlinedButton(
                        onClick = {
                            activity?.safLauncher?.launch(null)
                            hasUri = BlackBoxPreferences.getTreeUri(context) != null
                        },
                        modifier = Modifier.weight(1f),
                        shape = RoundedCornerShape(8.dp)
                    ) {
                        Text("Change", fontSize = 12.sp)
                    }
                    OutlinedButton(
                        onClick = {
                            BlackBoxPreferences.clearTreeUri(context)
                            SafBlackBoxMirror.cancelSchedule()
                            hasUri = false
                        },
                        modifier = Modifier.weight(1f),
                        shape = RoundedCornerShape(8.dp)
                    ) {
                        Text("Remove", fontSize = 12.sp)
                    }
                }
            } else {
                Text(
                    "No USB flash selected",
                    fontSize = 13.sp,
                    color = MaterialTheme.colorScheme.onSurfaceVariant
                )
                Spacer(Modifier.height(8.dp))
                OutlinedButton(
                    onClick = {
                        activity?.safLauncher?.launch(null)
                        // hasUri updates on next recomposition
                    },
                    modifier = Modifier.fillMaxWidth(),
                    shape = RoundedCornerShape(8.dp)
                ) {
                    Text("Select USB Flash", fontSize = 13.sp)
                }
            }
        }
    }

    // Refresh hasUri when returning from SAF picker
    LaunchedEffect(Unit) {
        while (true) {
            delay(1000)
            hasUri = BlackBoxPreferences.getTreeUri(context) != null
        }
    }
}

private enum class StatusColor { GREEN, YELLOW, RED }

@Composable
private fun StatusDot(color: StatusColor) {
    val dotColor = when (color) {
        StatusColor.GREEN -> Color(0xFF4CAF50)
        StatusColor.YELLOW -> Color(0xFFFFC107)
        StatusColor.RED -> Color(0xFFF44336)
    }
    Box(
        modifier = Modifier
            .size(12.dp)
            .background(dotColor, RoundedCornerShape(6.dp))
    )
}
