/**
 * arduino_pty_usb_bridge.cpp
 * PTY (slave) for PX4 arduino_output driver - master bridged to CH340 USB bulk on Android.
 * 
 * Protocol: [0xAA][0x55][num_channels][ch0_H][ch0_L]...[chN_H][chN_L][checksum]
 * - Sync header: 0xAA 0x55
 * - num_channels: number of PWM channels (1 byte)
 * - Each channel: 2 bytes big-endian (PWM microseconds, 1000-2000)
 * - checksum: XOR of all bytes except checksum itself
 */
#include "arduino_pty_usb_bridge.h"

// Forward declaration for Java bridge (defined in px4_jni.cpp)
extern "C" int arduino_send_pwm_via_java(const uint8_t* data, size_t len);

#include <android/log.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/usbdevice_fs.h>
#include <poll.h>
#include <pty.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#define TAG_ARDUINO "ArduinoPtyUsb"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG_ARDUINO, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG_ARDUINO, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG_ARDUINO, __VA_ARGS__)

// CH340 control requests
static constexpr uint8_t CH340_REQ_WRITE_REG   = 0x9A;
static constexpr uint8_t CH340_REQ_READ_REG    = 0x95;
static constexpr uint8_t CH340_REG_BREAK1      = 0x05;
static constexpr uint8_t CH340_REG_BREAK2      = 0x18;
static constexpr uint8_t CH340_REG_LCR         = 0x18;
static constexpr uint8_t CH340_REG_LCR_ENABLE  = 0xC0;
static constexpr uint8_t CH340_REG_LCR_DISABLE = 0x00;
static constexpr unsigned int CH340_USB_INTERFACE = 0;

// Arduino bridge protocol
static constexpr uint8_t PROTO_SYNC_H = 0xAA;
static constexpr uint8_t PROTO_SYNC_L = 0x55;
static constexpr uint8_t PROTO_MAX_CHANNELS = 16;

// Global state for Arduino bridge (prefixed to avoid conflicts with mavlink bridge)
std::mutex arduino_g_bridge_mutex;
std::atomic<bool> arduino_g_running{false};
std::thread arduino_g_th_pty_to_usb;
int arduino_g_usb_fd{-1};
int arduino_g_pty_master{-1};
std::string arduino_g_slave_path;
std::atomic<bool> arduino_g_threads_should_run{false};

// USB control transfer helper
static int usb_control(int fd, uint8_t reqtype, uint8_t request, uint16_t value, uint16_t index,
                       void *data, uint16_t len)
{
    struct usbdevfs_ctrltransfer ct {};
    ct.bRequestType = reqtype;
    ct.bRequest = request;
    ct.wValue = value;
    ct.wIndex = index;
    ct.wLength = len;
    ct.timeout = 1000;
    ct.data = (unsigned char *)data;
    return ioctl(fd, USBDEVFS_CONTROL, &ct);
}

// CH340 register write
static bool ch340_write_reg(int fd, uint8_t reg, uint8_t val)
{
    uint16_t v = ((uint16_t)val << 8) | reg;
    int r = usb_control(fd, 0x40, CH340_REQ_WRITE_REG, v, CH340_USB_INTERFACE, nullptr, 0);
    return (r >= 0);
}

// CH340 register read
static bool ch340_read_reg(int fd, uint8_t reg, uint8_t &out)
{
    uint16_t v = reg;
    uint16_t index = CH340_USB_INTERFACE;
    int r = usb_control(fd, 0xC0, CH340_REQ_READ_REG, v, index, &out, 1);
    return (r >= 0);
}

// CH340 configure UART
static bool ch340_configure(int fd, unsigned baud)
{
    // Enable UART
    if (!ch340_write_reg(fd, CH340_REG_BREAK1, 0x00)) {
        LOGE("CH340 write REG_BREAK1 failed");
        return false;
    }
    if (!ch340_write_reg(fd, CH340_REG_BREAK2, 0x00)) {
        LOGE("CH340 write REG_BREAK2 failed");
        return false;
    }

    // Set baud rate divisor (CH340 uses a different formula)
    // divisor = 1532620800 / baud / 2
    uint32_t divisor = 1532620800UL / baud / 2;
    uint8_t reg_value = (uint8_t)(divisor & 0xFF);
    uint8_t reg_index = (uint8_t)((divisor >> 8) & 0xFF);

    // Write baud rate
    uint16_t v = ((uint16_t)reg_value << 8) | reg_index;
    int r = usb_control(fd, 0x40, 0x9A, v, CH340_USB_INTERFACE, nullptr, 0);
    if (r < 0) {
        LOGE("CH340 set baud failed: r=%d errno=%d", r, errno);
        return false;
    }

    // Set line control: 8N1
    if (!ch340_write_reg(fd, CH340_REG_LCR, CH340_REG_LCR_ENABLE)) {
        LOGE("CH340 set LCR enable failed");
        return false;
    }
    if (!ch340_write_reg(fd, CH340_REG_LCR, CH340_REG_LCR_DISABLE | 0x03)) { // 8 bits
        LOGE("CH340 set LCR format failed");
        return false;
    }

    LOGI("CH340 configured: baud=%u", baud);
    return true;
}

// Claim USB interface and configure
static bool usb_claim_and_prep(int fd, unsigned baud)
{
    unsigned int iface = CH340_USB_INTERFACE;
    int r = ioctl(fd, USBDEVFS_CLAIMINTERFACE, &iface);
    if (r < 0) {
        if (errno == EBUSY) {
            LOGI("USBDEVFS_CLAIMINTERFACE EBUSY — continuing (Java may own interface)");
        } else {
            LOGE("USBDEVFS_CLAIMINTERFACE failed: errno=%d", errno);
            return false;
        }
    }

    // Try CH340 configuration first
    if (!ch340_configure(fd, baud)) {
        // CH340 config failed — device may be CDC-ACM (Arduino Mega/Uno official)
        // For CDC-ACM, we just need to claim the interface (already done above)
        // No special configuration needed — baud rate is set by Java UsbDeviceConnection
        LOGI("CH340 config failed — assuming CDC-ACM device (Arduino Mega/Uno)");
        LOGI("CDC-ACM device ready (baud set by Java)");
        // Don't fail — CDC-ACM devices work without special config
    }

    return true;
}

// Build Arduino protocol frame from PWM data
// Format: [0xAA][0x55][N][ch0_H][ch0_L]...[checksum]
static ssize_t build_proto_frame(const uint8_t *pwm_data, size_t data_len, uint8_t *out, size_t out_capacity)
{
    // data is raw bytes from PTY — we expect actuator output format
    // For simplicity, we forward raw bytes with protocol framing
    // The PTY slave will send raw actuator data that we frame

    if (data_len == 0 || data_len > PROTO_MAX_CHANNELS * 2) {
        return -1;
    }

    size_t frame_len = 2 + 1 + data_len + 1; // sync(2) + count(1) + data + checksum(1)
    if (frame_len > out_capacity) {
        return -1;
    }

    // Sync header
    out[0] = PROTO_SYNC_H;
    out[1] = PROTO_SYNC_L;

    // Number of channels (each channel = 2 bytes)
    uint8_t num_channels = (uint8_t)(data_len / 2);
    out[2] = num_channels;

    // Data
    memcpy(out + 3, pwm_data, data_len);

    // Checksum (XOR of all bytes except checksum)
    uint8_t checksum = 0;
    for (size_t i = 0; i < frame_len - 1; i++) {
        checksum ^= out[i];
    }
    out[frame_len - 1] = checksum;

    return (ssize_t)frame_len;
}

// Thread: PTY master → USB (send actuator data to Arduino)
static void pty_to_usb_thread()
{
    uint8_t read_buf[PROTO_MAX_CHANNELS * 2];
    uint8_t frame_buf[2 + 1 + PROTO_MAX_CHANNELS * 2 + 1];
    struct pollfd pfd {};
    pfd.fd = arduino_g_pty_master;
    pfd.events = POLLIN;

    while (arduino_g_threads_should_run.load()) {
        int ret = poll(&pfd, 1, 100);
        if (ret <= 0) continue;
        if (!(pfd.revents & POLLIN)) continue;

        ssize_t n = read(arduino_g_pty_master, read_buf, sizeof(read_buf));
        if (n <= 0) continue;

        // Build framed packet
        ssize_t frame_len = build_proto_frame(read_buf, (size_t)n, frame_buf, sizeof(frame_buf));
        if (frame_len < 0) {
            LOGE("Failed to build protocol frame");
            continue;
        }

        // Send via USB — Java bulkTransfer (works for CDC-ACM)
        int sent = arduino_send_pwm_via_java(frame_buf, (size_t)frame_len);
        if (sent < 0) {
            LOGE("Java bulkTransfer failed: result=%d", sent);
        } else {
            LOGI("Sent %d/%zd bytes to Arduino via Java bulkTransfer", sent, frame_len);
        }
    }
}

// NOTE: A usb_to_pty_thread() existed here previously, reading directly from
// arduino_g_usb_fd and forwarding to the PTY. It was dead code: the USB
// descriptor is owned by the Java side (UsbDeviceConnection) and reads now
// flow through UsbDeviceManager.readFromArduino() via JNI. Removed to avoid
// a second consumer racing the Java-owned endpoint.

// ===== Public API =====

bool arduino_pty_usb_bridge_start(int usb_fd, int baud)
{
    std::lock_guard<std::mutex> lock(arduino_g_bridge_mutex);

    if (arduino_g_running.load()) {
        LOGE("Bridge already running");
        return false;
    }

    arduino_g_usb_fd = usb_fd;

    // Create PTY pair
    arduino_g_pty_master = posix_openpt(O_RDWR | O_NOCTTY);
    if (arduino_g_pty_master < 0) {
        LOGE("posix_openpt failed: errno=%d", errno);
        return false;
    }

    if (grantpt(arduino_g_pty_master) != 0 || unlockpt(arduino_g_pty_master) != 0) {
        LOGE("grantpt/unlockpt failed: errno=%d", errno);
        close(arduino_g_pty_master);
        arduino_g_pty_master = -1;
        return false;
    }

    char *slave_name = ptsname(arduino_g_pty_master);
    if (!slave_name) {
        LOGE("ptsname failed: errno=%d", errno);
        close(arduino_g_pty_master);
        arduino_g_pty_master = -1;
        return false;
    }
    arduino_g_slave_path = slave_name;
    LOGI("PTY slave: %s", arduino_g_slave_path.c_str());

    // Configure USB
    if (!usb_claim_and_prep(usb_fd, (unsigned)baud)) {
        LOGE("USB configuration failed");
        close(arduino_g_pty_master);
        arduino_g_pty_master = -1;
        return false;
    }

    // Start bridge threads
    arduino_g_threads_should_run.store(true);
    arduino_g_running.store(true);

    arduino_g_th_pty_to_usb = std::thread(pty_to_usb_thread);

    LOGI("Arduino PTY-USB bridge started (PTY=%s, baud=%d)", arduino_g_slave_path.c_str(), baud);
    return true;
}

void arduino_pty_usb_bridge_stop()
{
    std::lock_guard<std::mutex> lock(arduino_g_bridge_mutex);

    if (!arduino_g_running.load()) return;

    arduino_g_threads_should_run.store(false);
    arduino_g_running.store(false);

    if (arduino_g_th_pty_to_usb.joinable()) arduino_g_th_pty_to_usb.join();

    if (arduino_g_usb_fd >= 0) {
        ioctl(arduino_g_usb_fd, USBDEVFS_RELEASEINTERFACE, (unsigned int)CH340_USB_INTERFACE);
        arduino_g_usb_fd = -1;
    }
    if (arduino_g_pty_master >= 0) {
        close(arduino_g_pty_master);
        arduino_g_pty_master = -1;
    }
    arduino_g_slave_path.clear();

    LOGI("Arduino PTY-USB bridge stopped");
}

const char *arduino_pty_usb_bridge_get_slave_path()
{
    return arduino_g_slave_path.empty() ? nullptr : arduino_g_slave_path.c_str();
}
