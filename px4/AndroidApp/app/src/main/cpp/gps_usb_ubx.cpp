/**
 * gps_usb_ubx.cpp — USB GPS UBX Driver
 * يقرأ بروتوكول UBX الثنائي من مستقبل u-blox عبر USB (libusb)
 * وينشر sensor_gps مباشرة على uORB بكل الحقول الكاملة.
 *
 * النمط: مطابق لـ servo_usb_output.cpp (thread مستقل + libusb + fd من JNI)
 * الفرق: يستخدم GPSDriverUBX الموجود في PX4 لتحليل بروتوكول UBX الثنائي
 *        بدلاً من NMEA النصي — يعطي بيانات مكافئة لأي GPS مدعوم رسمياً في PX4.
 *
 * تدفق البيانات:
 *   u-blox USB → libusb bulk read → GPSDriverUBX::receive() → sensor_gps_s → uORB
 *
 * الحقول التي يملأها UBX ولا يعطيها NMEA:
 *   - altitude_ellipsoid_m (منفصل عن MSL)
 *   - vel_d_m_s من Doppler (دقة ±0.05 م/ث بدل ±3 م/ث)
 *   - time_utc_usec (نانوثانية)
 *   - hdop/vdop حقيقية من NAV-DOP
 *   - c_variance_rad (دقة الاتجاه)
 *   - jamming_indicator / jamming_state
 *   - spoofing_state
 */
#include "gps_usb_ubx.h"
#include "native_sensor_reader.h"

#include <uORB/Publication.hpp>
#include <uORB/topics/sensor_gps.h>
#include <uORB/topics/satellite_info.h>
#include <drivers/drv_hrt.h>

// PX4 GPS UBX driver — يحلل بروتوكول UBX الثنائي
#include "ubx.h"

#include <libusb.h>

#include <android/log.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <cstring>
#include <unistd.h>

#define TAG_GPS "GpsUSB"
#define LOGI_G(...) __android_log_print(ANDROID_LOG_INFO,  TAG_GPS, __VA_ARGS__)
#define LOGW_G(...) __android_log_print(ANDROID_LOG_WARN,  TAG_GPS, __VA_ARGS__)
#define LOGE_G(...) __android_log_print(ANDROID_LOG_ERROR, TAG_GPS, __VA_ARGS__)

// ===== Device ID =====
// u-blox حقيقي — ليس محاكاة. DRV_GPS_DEVTYPE_UBX=0x01, bus_type=0 (UNKNOWN)
#define USB_GPS_DEVID  ((0x01 << 16) | 0)

// ===== State =====
static std::atomic<bool> s_gps_running{false};
static std::atomic<bool> s_configure_phase{true}; // true أثناء configure, false بعده
static std::thread s_gps_thread;
static std::atomic<int> s_gps_usb_fd{-1};
static std::atomic<int> s_usb_gps_status{0};     // 0=disconnected, 1=connected, 2=configured, 3=receiving
static std::atomic<long> s_usb_gps_rx_count{0};   // عدد حزم GPS المستقبلة
static std::atomic<int> s_usb_gps_diag{0};        // diagnostic: baud found or error code
static char s_usb_gps_msg[128] = "Idle";           // diagnostic message for UI
static uint32_t s_detected_baud = 9600;            // الباود المكتشف أثناء auto-baud
static std::mutex s_usb_mutex;

// libusb state (protected by s_usb_mutex)
static libusb_context *s_ctx = nullptr;
static libusb_device_handle *s_dev_handle = nullptr;
static int s_bulk_endpoint_in = -1;
static int s_bulk_endpoint_out = -1;

// ===== PL2303 USB-Serial Configuration =====
// u-blox متصل عبر محول Prolific PL2303 (VID=0x067B)
// يحتاج تهيئة baud rate عبر control transfers

static bool pl2303_configure(libusb_device_handle *h, uint32_t baudrate) {
    int r;

    // PL2303 initialization sequence (vendor-specific requests)
    // 1. Vendor read/write init sequence
    uint8_t buf[7];
    libusb_control_transfer(h, 0xC0, 0x01, 0x8484, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0x40, 0x01, 0x0404, 0, nullptr, 0, 1000);
    libusb_control_transfer(h, 0xC0, 0x01, 0x8484, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0xC0, 0x01, 0x8383, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0xC0, 0x01, 0x8484, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0x40, 0x01, 0x0404, 1, nullptr, 0, 1000);
    libusb_control_transfer(h, 0xC0, 0x01, 0x8484, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0xC0, 0x01, 0x8383, 0, buf, 1, 1000);
    libusb_control_transfer(h, 0x40, 0x01, 0x0000, 1, nullptr, 0, 1000);
    libusb_control_transfer(h, 0x40, 0x01, 0x0001, 0, nullptr, 0, 1000);
    libusb_control_transfer(h, 0x40, 0x01, 0x0002, 0x0044, nullptr, 0, 1000);

    // 2. GET_LINE_CODING (class request 0x21, bRequest=0x21)
    // بعض شرائح PL2303 المقلدة لا تدعمه — نتجاوزه إذا فشل
    r = libusb_control_transfer(h, 0xA1, 0x21, 0, 0, buf, 7, 1000);
    if (r < 0) {
        LOGW_G("PL2303 GET_LINE_CODING failed: %s (non-fatal, skipping)", libusb_error_name(r));
    }

    // 3. SET_LINE_CODING — baud rate + 8N1
    memset(buf, 0, sizeof(buf));
    buf[0] = (uint8_t)(baudrate & 0xFF);
    buf[1] = (uint8_t)((baudrate >> 8) & 0xFF);
    buf[2] = (uint8_t)((baudrate >> 16) & 0xFF);
    buf[3] = (uint8_t)((baudrate >> 24) & 0xFF);
    buf[4] = 0; // stop bits: 0=1, 1=1.5, 2=2
    buf[5] = 0; // parity: 0=none
    buf[6] = 8; // data bits

    r = libusb_control_transfer(h, 0x21, 0x20, 0, 0, buf, 7, 1000);
    if (r < 0) {
        LOGE_G("PL2303 SET_LINE_CODING failed: %s", libusb_error_name(r));
        return false;
    }

    // 4. SET_CONTROL_LINE_STATE (DTR=1, RTS=1)
    r = libusb_control_transfer(h, 0x21, 0x22, 0x0003, 0, nullptr, 0, 1000);
    if (r < 0) {
        LOGW_G("PL2303 SET_CONTROL_LINE_STATE failed: %s (non-fatal)", libusb_error_name(r));
    }

    LOGI_G("PL2303 configured: %u baud, 8N1", baudrate);
    return true;
}

// ===== USB Open/Close =====
// u-blox عبر USB: إما CDC-ACM مباشر (VID=0x1546) أو عبر محول PL2303 (VID=0x067B)

static bool usb_open(int fd) {
    std::lock_guard<std::mutex> lock(s_usb_mutex);

    if (s_dev_handle) {
        libusb_close(s_dev_handle);
        s_dev_handle = nullptr;
    }

    if (!s_ctx) {
        int r = libusb_set_option(NULL, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
        if (r != LIBUSB_SUCCESS) {
            LOGW_G("libusb_set_option(NO_DEVICE_DISCOVERY) failed: %s (non-fatal)", libusb_error_name(r));
        }
        r = libusb_init(&s_ctx);
        if (r < 0) {
            LOGE_G("libusb_init failed: %s", libusb_error_name(r));
            return false;
        }
    }

    int r = libusb_wrap_sys_device(s_ctx, (intptr_t)fd, &s_dev_handle);
    if (r < 0 || !s_dev_handle) {
        LOGE_G("libusb_wrap_sys_device(fd=%d) failed: %s", fd, libusb_error_name(r));
        return false;
    }

    // Try to claim interfaces — CDC-ACM has 2 interfaces
    // Try interface 0 first, then interface 1 (data)
    for (int iface = 0; iface < 3; iface++) {
        libusb_detach_kernel_driver(s_dev_handle, iface); // ignore error
        libusb_claim_interface(s_dev_handle, iface); // ignore error — may not exist
    }

    // Find bulk IN and OUT endpoints
    libusb_device *dev = libusb_get_device(s_dev_handle);
    struct libusb_config_descriptor *config = nullptr;
    libusb_get_active_config_descriptor(dev, &config);
    s_bulk_endpoint_in = -1;
    s_bulk_endpoint_out = -1;

    if (config) {
        for (uint8_t i = 0; i < config->bNumInterfaces; i++) {
            const struct libusb_interface *iface = &config->interface[i];
            for (int j = 0; j < iface->num_altsetting; j++) {
                const struct libusb_interface_descriptor *alt = &iface->altsetting[j];
                for (uint8_t k = 0; k < alt->bNumEndpoints; k++) {
                    const struct libusb_endpoint_descriptor *ep = &alt->endpoint[k];
                    if ((ep->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_TRANSFER_TYPE_BULK) {
                        if ((ep->bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
                            if (s_bulk_endpoint_in < 0) s_bulk_endpoint_in = ep->bEndpointAddress;
                        } else {
                            if (s_bulk_endpoint_out < 0) s_bulk_endpoint_out = ep->bEndpointAddress;
                        }
                    }
                }
            }
        }
        libusb_free_config_descriptor(config);
    }

    if (s_bulk_endpoint_in < 0) {
        LOGE_G("No bulk IN endpoint found");
        libusb_close(s_dev_handle);
        s_dev_handle = nullptr;
        return false;
    }

    if (s_bulk_endpoint_out < 0) {
        LOGW_G("No bulk OUT endpoint — configure commands won't be sent");
    }

    // Detect chip type and configure
    libusb_device_descriptor desc{};
    libusb_get_device_descriptor(dev, &desc);
    uint16_t vid = desc.idVendor;

    if (vid == 0x067B) {
        // PL2303 USB-Serial — نجرب عدة باودات حتى نكتشف بيانات UBX أو NMEA
        static const uint32_t baud_candidates[] = {
            9600, 115200, 38400, 57600, 19200, 230400, 460800, 4800
        };
        bool baud_found = false;

        for (size_t bi = 0; bi < sizeof(baud_candidates) / sizeof(baud_candidates[0]); bi++) {
            uint32_t try_baud = baud_candidates[bi];
            LOGI_G("PL2303: trying baud %u ...", try_baud);
            snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "Trying %u baud...", try_baud);

            if (!pl2303_configure(s_dev_handle, try_baud)) {
                LOGW_G("PL2303: configure failed at %u — skipping", try_baud);
                snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "PL2303 cfg fail @%u", try_baud);
                continue;
            }

            // اقرأ لمدة 1.5 ثانية وابحث عن بيانات (UBX header 0xB5 0x62 أو NMEA '$')
            uint8_t probe_buf[512];
            int transferred = 0;
            bool got_data = false;

            for (int attempt = 0; attempt < 6; attempt++) {  // 6 × 250ms = 1.5s
                int r = libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in,
                                             probe_buf, sizeof(probe_buf), &transferred, 250);
                if (r == 0 && transferred > 0) {
                    // ابحث عن UBX header أو NMEA
                    for (int k = 0; k < transferred - 1; k++) {
                        if ((probe_buf[k] == 0xB5 && probe_buf[k+1] == 0x62) ||
                            probe_buf[k] == '$') {
                            got_data = true;
                            break;
                        }
                    }
                    if (got_data) break;
                }
            }

            if (got_data) {
                LOGI_G("PL2303: GPS data detected at %u baud!", try_baud);
                snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "Found @%u baud", try_baud);
                s_usb_gps_diag.store((int)try_baud);
                s_detected_baud = try_baud;
                baud_found = true;
                break;
            } else {
                LOGI_G("PL2303: no GPS data at %u baud", try_baud);
                snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "No data @%u", try_baud);
            }
        }

        if (!baud_found) {
            LOGW_G("PL2303: no GPS data at any baud — using 9600 as fallback");
            snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "No data any baud! fallback 9600");
            pl2303_configure(s_dev_handle, 9600);
        }
    }

    // تأخير ليستقر PL2303 بعد claim interfaces — بدونه bulk_transfer يرجع فوراً
    usleep(300000); // 300ms

    LOGI_G("USB GPS opened: fd=%d, VID=0x%04X, bulk_in=0x%02X, bulk_out=0x%02X",
           fd, vid, s_bulk_endpoint_in, s_bulk_endpoint_out);
    return true;
}

static void usb_close() {
    std::lock_guard<std::mutex> lock(s_usb_mutex);
    if (s_dev_handle) {
        // أرسل UBX-CFG-RST (hot start) لإعادة GPS لباود 9600 الافتراضي
        // هذا يضمن أن عند إعادة التشغيل، GPS يكون على 9600
        if (s_bulk_endpoint_out >= 0) {
            // UBX-CFG-RST: header(2) + class(1) + id(1) + len(2) + payload(4) + cksum(2) = 12 bytes
            // navBbrMask=0x0000 (hot start), resetMode=0x00 (hardware reset)
            uint8_t ubx_rst[] = {
                0xB5, 0x62,       // UBX sync
                0x06, 0x04,       // CFG-RST
                0x04, 0x00,       // length = 4
                0x00, 0x00,       // navBbrMask = 0 (hot start — يحفظ ephemeris)
                0x00,             // resetMode = 0 (hardware reset)
                0x00,             // reserved
                0x0E, 0x64        // checksum
            };
            int transferred = 0;
            libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_out,
                                 ubx_rst, sizeof(ubx_rst), &transferred, 500);
            LOGI_G("Sent UBX-CFG-RST (hot start) to reset GPS to 9600 baud");
            usleep(200000); // 200ms — وقت لـ GPS يعيد التشغيل
        }

        for (int iface = 0; iface < 3; iface++) {
            libusb_release_interface(s_dev_handle, iface);
        }
        libusb_close(s_dev_handle);
        s_dev_handle = nullptr;
        LOGI_G("USB GPS closed");
    }
    s_bulk_endpoint_in = -1;
    s_bulk_endpoint_out = -1;
}

// ===== GPS Callback =====
// الجسر بين GPSDriverUBX و libusb
// GPSDriverUBX يستدعي read()/write()/setBaudrate() عبر هذا الـ callback

static int gps_callback(GPSCallbackType type, void *data1, int data2, void *user) {
    (void)user;

    switch (type) {
    case GPSCallbackType::readDeviceData: {
        // data1: buffer (أول sizeof(int) bytes = timeout_ms)
        // data2: buffer length
        // return: bytes read, 0 on timeout
        int timeout_ms = 0;
        memcpy(&timeout_ms, data1, sizeof(timeout_ms));
        if (timeout_ms < 10) timeout_ms = 10;

        if (!s_dev_handle || s_bulk_endpoint_in < 0) return 0;

        // قراءة مجمّعة — PL2303 يعطي بايتات قليلة في كل bulk transfer
        unsigned char *buf = (unsigned char *)data1;
        int total = 0;
        gps_abstime deadline = gps_absolute_time() + (gps_abstime)timeout_ms * 1000;
        while (total < data2) {
            gps_abstime now = gps_absolute_time();
            if (now >= deadline) break;
            int ms_left = (int)((deadline - now) / 1000);
            if (ms_left < 1) ms_left = 1;
            int chunk_ms = (ms_left < 50) ? ms_left : 50;
            int transferred = 0;
            int r = libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in,
                                         buf + total, data2 - total, &transferred, chunk_ms);
            if (r == 0 && transferred > 0) { total += transferred; }
            else if (r == LIBUSB_ERROR_TIMEOUT) { if (total > 0) break; }
            else if (r == LIBUSB_ERROR_OVERFLOW) { if (transferred > 0) total += transferred; break; }
            else if (r == LIBUSB_ERROR_PIPE) { libusb_clear_halt(s_dev_handle, s_bulk_endpoint_in); break; }
            else if (r < 0 && r != LIBUSB_ERROR_TIMEOUT) {
                LOGE_G("USB read error: %s (code=%d)", libusb_error_name(r), r);
                return -1;
            }
        }
        return total;
    }

    case GPSCallbackType::writeDeviceData: {
        // data1: data to write, data2: length
        if (!s_dev_handle || s_bulk_endpoint_out < 0) return data2;

        int transferred = 0;
        int r = libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_out,
                                     (unsigned char *)data1, data2,
                                     &transferred, 1000);
        if (r == 0) {
            return transferred;
        } else {
            LOGE_G("USB write error: %s", libusb_error_name(r));
            return -1;
        }
    }

    case GPSCallbackType::setBaudrate: {
        // GPSDriverUBX يطلب تغيير baud rate أثناء configure()
        // configure() يرسل UBX-CFG-PRT لتغيير باود GPS أولاً (على الباود الحالي)
        // ثم يستدعي هذا callback لتغيير باود PL2303 ليطابق
        uint32_t new_baud = (uint32_t)data2;
        if (s_dev_handle) {
            // SET_LINE_CODING for PL2303 / CDC-ACM
            uint8_t buf[7] = {};
            buf[0] = (uint8_t)(new_baud & 0xFF);
            buf[1] = (uint8_t)((new_baud >> 8) & 0xFF);
            buf[2] = (uint8_t)((new_baud >> 16) & 0xFF);
            buf[3] = (uint8_t)((new_baud >> 24) & 0xFF);
            buf[4] = 0; buf[5] = 0; buf[6] = 8;
            int r = libusb_control_transfer(s_dev_handle, 0x21, 0x20, 0, 0, buf, 7, 1000);
            if (r >= 0) {
                LOGI_G("Baud rate set to %u", new_baud);
            }
            // انتظر لتثبيت PL2303 + وقت لـ GPS لتغيير الباود
            usleep(100000); // 100ms
            // Flush بيانات قديمة على الباود السابق
            uint8_t flush[512];
            int xfer = 0;
            for (int i = 0; i < 3; i++) {
                libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in, flush, sizeof(flush), &xfer, 50);
            }
        }
        return 0;
    }

    case GPSCallbackType::setClock: {
        // u-blox يعطي وقت UTC دقيق — يمكن استخدامه لضبط ساعة النظام
        // حالياً نتجاهله لأن Android يدير الساعة
        return 0;
    }

    case GPSCallbackType::gotRTCMMessage:
    case GPSCallbackType::gotRelativePositionMessage:
    case GPSCallbackType::surveyInStatus:
        return 0;

    default:
        return 0;
    }
}

// ===== Main Loop =====

static void gps_ubx_loop() {
    LOGI_G("GPS UBX thread started — waiting for USB connection...");

    // uORB publisher
    uORB::Publication<sensor_gps_s> gps_pub{ORB_ID(sensor_gps)};

    bool usb_connected = false;
    int prev_fd = -1;

    // حالة GPSDriverUBX
    GPSDriverUBX *ubx_driver = nullptr;
    sensor_gps_s gps_report{};
    satellite_info_s sat_info{};

    while (s_gps_running.load()) {
        int current_fd = s_gps_usb_fd.load();

        // Handle USB connect/disconnect (نفس نمط servo_usb_output.cpp)
        if (current_fd != prev_fd) {
            // Cleanup old driver
            if (ubx_driver) {
                delete ubx_driver;
                ubx_driver = nullptr;
            }

            if (current_fd >= 0) {
                snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "Opening fd=%d...", current_fd);
                s_usb_gps_status.store(1);

                usb_connected = usb_open(current_fd);
                if (usb_connected) {
                    s_configure_phase.store(true);
                    // اعرض VID
                    {
                        libusb_device *d = libusb_get_device(s_dev_handle);
                        libusb_device_descriptor dd{};
                        if (d) libusb_get_device_descriptor(d, &dd);
                        snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg),
                                 "Opened VID=0x%04X ep_in=0x%02X", dd.idVendor, s_bulk_endpoint_in);
                    }
                    LOGI_G("USB GPS connected (fd=%d) — configuring u-blox...", current_fd);

                    memset(&gps_report, 0, sizeof(gps_report));
                    memset(&sat_info, 0, sizeof(sat_info));

                    ubx_driver = new GPSDriverUBX(
                        GPSHelper::Interface::UART,
                        gps_callback,
                        nullptr,
                        &gps_report,
                        &sat_info,
                        8
                    );

                    // Flush
                    {
                        uint8_t flush_buf[512];
                        int transferred = 0;
                        for (int i = 0; i < 10; i++) {
                            libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in,
                                                 flush_buf, sizeof(flush_buf), &transferred, 100);
                        }
                    }

                    snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "Configuring UBX...");

                    // أرسل UBX-CFG-PRT على الباود الحالي (9600) لتفعيل UBX protocol
                    // هذا يجعل GPS يقبل ويرد على أوامر UBX قبل أن configure() يغير الباود
                    if (s_bulk_endpoint_out >= 0 && s_detected_baud > 0) {
                        // ضبط PL2303 على الباود المكتشف أولاً
                        {
                            uint8_t lc[7] = {};
                            lc[0] = (uint8_t)(s_detected_baud & 0xFF);
                            lc[1] = (uint8_t)((s_detected_baud >> 8) & 0xFF);
                            lc[2] = (uint8_t)((s_detected_baud >> 16) & 0xFF);
                            lc[3] = (uint8_t)((s_detected_baud >> 24) & 0xFF);
                            lc[4] = 0; lc[5] = 0; lc[6] = 8;
                            libusb_control_transfer(s_dev_handle, 0x21, 0x20, 0, 0, lc, 7, 1000);
                            usleep(50000);
                        }

                        // UBX-CFG-PRT: تفعيل UBX in+out على UART1 بباود الحالي
                        uint8_t cfg_prt[28] = {
                            0xB5, 0x62,             // sync
                            0x06, 0x00,             // CFG-PRT
                            0x14, 0x00,             // length = 20
                            0x01,                   // portID = 1 (UART1)
                            0x00,                   // reserved
                            0x00, 0x00,             // txReady disabled
                            0xD0, 0x08, 0x00, 0x00, // mode: 8N1
                            // baudrate (4 bytes, little-endian)
                            (uint8_t)(s_detected_baud & 0xFF),
                            (uint8_t)((s_detected_baud >> 8) & 0xFF),
                            (uint8_t)((s_detected_baud >> 16) & 0xFF),
                            (uint8_t)((s_detected_baud >> 24) & 0xFF),
                            0x03, 0x00,             // inProtoMask: UBX + NMEA
                            0x03, 0x00,             // outProtoMask: UBX + NMEA
                            0x00, 0x00,             // flags
                            0x00, 0x00,             // reserved
                            0x00, 0x00              // checksum (سنحسبها)
                        };
                        // حساب UBX checksum
                        uint8_t ck_a = 0, ck_b = 0;
                        for (int i = 2; i < 26; i++) {  // من class إلى نهاية payload
                            ck_a = (ck_a + cfg_prt[i]) & 0xFF;
                            ck_b = (ck_b + ck_a) & 0xFF;
                        }
                        cfg_prt[26] = ck_a;
                        cfg_prt[27] = ck_b;

                        int transferred = 0;
                        libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_out,
                                             cfg_prt, sizeof(cfg_prt), &transferred, 1000);
                        LOGI_G("Sent UBX-CFG-PRT to enable UBX protocol at %u baud", s_detected_baud);
                        usleep(200000); // 200ms — وقت لـ GPS يطبق التغيير

                        // Flush الرد
                        uint8_t flush[512];
                        int xfer = 0;
                        for (int i = 0; i < 5; i++) {
                            libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in,
                                                 flush, sizeof(flush), &xfer, 100);
                        }
                    }

                    GPSHelper::GPSConfig config{};
                    config.output_mode = GPSHelper::OutputMode::GPS;
                    config.gnss_systems = GPSHelper::GNSSSystemsMask::RECEIVER_DEFAULTS;
                    config.interface_protocols = GPSHelper::InterfaceProtocolsMask::ALL_DISABLED;
                    config.cfg_wipe = false;

                    // نحاول configure() حتى 5 مرات — بين كل محاولة نرسل RST + CFG-PRT
                    int ret = -1;
                    unsigned baudrate = 0;
                    for (int attempt = 0; attempt < 5; attempt++) {
                        baudrate = 0;
                        ret = ubx_driver->configure(baudrate, config);
                        if (ret == 0) break;

                        LOGW_G("u-blox configure attempt %d/5 failed (ret=%d) — resetting GPS...", attempt + 1, ret);
                        snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "cfg retry %d/5...", attempt + 1);

                        // أعد إنشاء driver لتنظيف الحالة الداخلية
                        delete ubx_driver;
                        memset(&gps_report, 0, sizeof(gps_report));
                        memset(&sat_info, 0, sizeof(sat_info));
                        ubx_driver = new GPSDriverUBX(
                            GPSHelper::Interface::UART,
                            gps_callback, nullptr, &gps_report, &sat_info, 8
                        );

                        // أرسل RST لإرجاع GPS لـ 9600
                        if (s_bulk_endpoint_out >= 0) {
                            uint8_t ubx_rst[] = {
                                0xB5, 0x62, 0x06, 0x04, 0x04, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x0E, 0x64
                            };
                            int xfer = 0;
                            // أرسل RST على كل الباودات المحتملة
                            static const uint32_t rst_bauds[] = {9600, 38400, 115200};
                            for (size_t bi = 0; bi < 3; bi++) {
                                uint8_t lc[7] = {};
                                lc[0] = (uint8_t)(rst_bauds[bi] & 0xFF);
                                lc[1] = (uint8_t)((rst_bauds[bi] >> 8) & 0xFF);
                                lc[2] = (uint8_t)((rst_bauds[bi] >> 16) & 0xFF);
                                lc[3] = (uint8_t)((rst_bauds[bi] >> 24) & 0xFF);
                                lc[4] = 0; lc[5] = 0; lc[6] = 8;
                                libusb_control_transfer(s_dev_handle, 0x21, 0x20, 0, 0, lc, 7, 1000);
                                usleep(50000);
                                libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_out,
                                                     ubx_rst, sizeof(ubx_rst), &xfer, 500);
                            }
                            LOGI_G("Sent RST on 9600/38400/115200 — waiting for GPS reboot...");
                        }

                        usleep(1500000); // 1.5 ثانية — وقت كافي لـ GPS يعيد التشغيل

                        // أعد ضبط PL2303 على 9600 + أرسل CFG-PRT
                        {
                            uint8_t lc[7] = {0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08}; // 9600
                            libusb_control_transfer(s_dev_handle, 0x21, 0x20, 0, 0, lc, 7, 1000);
                            usleep(100000);
                        }

                        // Flush
                        {
                            uint8_t flush[512];
                            int xfer = 0;
                            for (int i = 0; i < 5; i++) {
                                libusb_bulk_transfer(s_dev_handle, s_bulk_endpoint_in,
                                                     flush, sizeof(flush), &xfer, 100);
                            }
                        }
                    }

                    if (ret == 0) {
                        s_usb_gps_status.store(2);
                        snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "UBX OK baud=%u", baudrate);
                        LOGI_G("u-blox configured successfully (baud=%u)", baudrate);
                    } else {
                        snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "cfg FAIL after 3 attempts");
                        LOGE_G("u-blox configure failed after 3 attempts");
                    }
                } else {
                    snprintf(s_usb_gps_msg, sizeof(s_usb_gps_msg), "usb_open FAIL fd=%d", current_fd);
                    s_usb_gps_status.store(0);
                    LOGE_G("Failed to open USB GPS (fd=%d)", current_fd);
                }
            } else {
                if (usb_connected) {
                    usb_close();
                    usb_connected = false;
                    s_usb_gps_status.store(0);
                    s_usb_gps_rx_count.store(0);
                    LOGI_G("USB GPS disconnected");
                }
                if (ubx_driver) {
                    delete ubx_driver;
                    ubx_driver = nullptr;
                }
            }
            prev_fd = current_fd;
        }

        // Read and parse UBX data
        if (usb_connected && ubx_driver) {
            int ret = ubx_driver->receive(500); // 500ms

            if (ret > 0) {
                // bit 0 = got position update
                s_usb_gps_status.store(3);
                s_usb_gps_rx_count.fetch_add(1);
                if (ret & 1) {
                    gps_report.device_id = USB_GPS_DEVID;
                    gps_pub.publish(gps_report);
                    g_sensor_counts.gps.fetch_add(1, std::memory_order_relaxed);
                }
            } else if (ret < 0) {
                if (s_gps_usb_fd.load() < 0) {
                    LOGI_G("USB GPS fd=-1 — disconnected");
                    usb_close();
                    usb_connected = false;
                    delete ubx_driver;
                    ubx_driver = nullptr;
                    prev_fd = -1;
                }
            }
        } else {
            usleep(500000); // 500ms
        }
    }

    // Cleanup
    if (ubx_driver) {
        delete ubx_driver;
        ubx_driver = nullptr;
    }
    if (usb_connected) {
        usb_close();
    }
    if (s_ctx) {
        libusb_exit(s_ctx);
        s_ctx = nullptr;
    }

    LOGI_G("GPS UBX thread stopped");
}

// ===== Public API =====

void gps_usb_ubx_start() {
    if (s_gps_running.load()) return;
    s_gps_running.store(true);
    s_gps_thread = std::thread(gps_ubx_loop);
}

void gps_usb_ubx_stop() {
    s_gps_running.store(false);
    if (s_gps_thread.joinable()) s_gps_thread.join();
}

void gps_usb_set_fd(int fd) {
    LOGI_G("gps_usb_set_fd(%d)", fd);
    s_gps_usb_fd.store(fd);
}

int gps_usb_get_status() { return s_usb_gps_status.load(); }
long gps_usb_get_rx_count() { return s_usb_gps_rx_count.load(); }
int gps_usb_get_fd() { return s_gps_usb_fd.load(); }
const char* gps_usb_get_diag_msg() { return s_usb_gps_msg; }
