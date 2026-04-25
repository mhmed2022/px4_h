/**
 * arduino_output.cpp
 * Subscribes to actuator_motors + actuator_test and writes to Arduino via USB.
 */

#include "arduino_output.hpp"
#include "arduino_pty_usb_bridge.h"

#include <fcntl.h>
#include <unistd.h>
#include <drivers/drv_hrt.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/getopt.h>

#include <atomic>

// From px4_jni.cpp — sends PWM via Java bulkTransfer
extern "C" int arduino_send_pwm_via_java(const uint8_t* data, size_t len);

// Protocol Constants
static constexpr uint8_t PROTO_SYNC_H = 0xAA;
static constexpr uint8_t PROTO_SYNC_L = 0x55;
static constexpr uint8_t PROTO_MAX_CHANNELS = 16;

// PWM Constants (must match Arduino firmware)
static constexpr uint16_t PWM_SAFE_MIN  = 1000;
static constexpr uint16_t PWM_SAFE_MAX  = 2000;
static constexpr uint16_t PWM_DISARMED  = 1000;

// manual_control timeout — if no valid GCS signal for this long, send disarm
static constexpr uint64_t MANUAL_CONTROL_TIMEOUT_US = 500000; // 500ms

static const char *g_pty_path = nullptr;
static std::atomic<uint16_t> g_last_pwm_1{1000};
static std::atomic<uint16_t> g_last_pwm_2{1000};
static std::atomic<uint16_t> g_last_pwm_3{1000};
static std::atomic<uint16_t> g_last_pwm_4{1000};
static std::atomic<bool> g_usb_connected{false};

void ArduinoOutput::set_pty_path(const char *path)
{
    g_pty_path = path;
}

ArduinoOutput::ArduinoOutput() :
    ModuleBase<ArduinoOutput>(),
    ScheduledWorkItem("arduino_output", px4::wq_configurations::hp_default)
{
    if (g_pty_path) {
        _device = g_pty_path;
        PX4_INFO("PTY path set to: %s", _device);
    } else {
        PX4_INFO("PTY not set — using Java bulkTransfer bridge");
    }
}

ArduinoOutput::~ArduinoOutput()
{
    if (_uart_fd >= 0) {
        close(_uart_fd);
    }
}

void ArduinoOutput::send_frame(const uint16_t pwm[4])
{
    constexpr uint8_t NUM_CHANNELS = 4;
    uint8_t frame[2 + 1 + (NUM_CHANNELS * 2) + 1];
    frame[0] = PROTO_SYNC_H;
    frame[1] = PROTO_SYNC_L;
    frame[2] = NUM_CHANNELS;

    for (uint8_t i = 0; i < NUM_CHANNELS; i++) {
        frame[3 + i * 2]     = (uint8_t)(pwm[i] >> 8);
        frame[3 + i * 2 + 1] = (uint8_t)(pwm[i] & 0xFF);
    }

    uint8_t checksum = 0;
    size_t frame_len = sizeof(frame);
    for (size_t i = 0; i < frame_len - 1; i++) {
        checksum ^= frame[i];
    }
    frame[frame_len - 1] = checksum;

    int sent = arduino_send_pwm_via_java(frame, frame_len);
    if (sent < 0) {
        _send_fail++;
        _connected = false;
        g_usb_connected.store(false, std::memory_order_relaxed);
        // Rate-limit error: log once every 5 seconds
        uint64_t now = hrt_absolute_time();
        if (now - _last_error_time > 5000000) {
            PX4_WARN("Arduino not connected (fail count: %u)", _send_fail);
            _last_error_time = now;
        }
    } else {
        if (!_connected) {
            PX4_INFO("Arduino connected! Sending PWM data");
        }
        _connected = true;
        g_usb_connected.store(true, std::memory_order_relaxed);
        g_last_pwm_1.store(pwm[0], std::memory_order_relaxed);
        g_last_pwm_2.store(pwm[1], std::memory_order_relaxed);
        g_last_pwm_3.store(pwm[2], std::memory_order_relaxed);
        g_last_pwm_4.store(pwm[3], std::memory_order_relaxed);
        _send_ok++;
    }
}

uint16_t arduino_last_pwm_1() { return g_last_pwm_1.load(std::memory_order_relaxed); }
uint16_t arduino_last_pwm_2() { return g_last_pwm_2.load(std::memory_order_relaxed); }
uint16_t arduino_last_pwm_3() { return g_last_pwm_3.load(std::memory_order_relaxed); }
uint16_t arduino_last_pwm_4() { return g_last_pwm_4.load(std::memory_order_relaxed); }
bool arduino_usb_connected() { return g_usb_connected.load(std::memory_order_relaxed); }

void ArduinoOutput::Run()
{
    if (should_exit()) {
        ScheduleClear();
        exit_and_cleanup();
        return;
    }

    // --- Check manual_control_setpoint validity ---
    {
        manual_control_setpoint_s mcs;
        if (_manual_sub.copy(&mcs)) {
            if (mcs.valid) {
                _last_mcs_valid_time = hrt_absolute_time();
                if (!_mcs_valid_reported) {
                    PX4_INFO("manual_control_setpoint is VALID (QGC connected)");
                    _mcs_valid_reported = true;
                }
            } else {
                uint64_t now = hrt_absolute_time();
                if (now - _last_mcs_warn_time > 10000000) { // every 10s
                    PX4_WARN("manual_control_setpoint.valid=false — is QGC Virtual Joystick active?");
                    _last_mcs_warn_time = now;
                }
                _mcs_valid_reported = false;
            }
        }
    }

    // --- Check actuator_test (from actuator_test set -m X -v Y) ---
    actuator_test_s test;
    while (_test_sub.update(&test)) {
        if (test.action == actuator_test_s::ACTION_DO_CONTROL) {
            int motor_idx = (int)test.function - (int)actuator_test_s::FUNCTION_MOTOR1;
            if (motor_idx >= 0 && motor_idx < 4) {
                _test_value[motor_idx] = test.value;
                PX4_INFO("Test motor %d = %.2f", motor_idx + 1, (double)test.value);
            }
        } else if (test.action == actuator_test_s::ACTION_RELEASE_CONTROL) {
            for (int i = 0; i < 4; i++) {
                _test_value[i] = NAN;
            }
            // Send disarmed frame immediately so motors stop
            uint16_t disarmed[4] = {PWM_DISARMED, PWM_DISARMED, PWM_DISARMED, PWM_DISARMED};
            send_frame(disarmed);
            PX4_INFO("Test released — back to normal");
        }
    }

    // --- Build PWM values ---
    bool has_test = false;
    for (int i = 0; i < 4; i++) {
        if (PX4_ISFINITE(_test_value[i])) {
            has_test = true;
            break;
        }
    }

    uint16_t pwm[4];

    if (has_test) {
        // Test mode: use test values where set, disarmed for rest
        for (int i = 0; i < 4; i++) {
            if (PX4_ISFINITE(_test_value[i])) {
                float v = _test_value[i];
                if (v < 0.0f) {
                    // Bidirectional range [-1,1] → [1000,2000]
                    if (v < -1.0f) { v = -1.0f; }
                    float pwm_f = 1500.0f + (v * 500.0f);
                    pwm[i] = (uint16_t)pwm_f; // pwm_f ∈ [1000,1500]
                } else {
                    if (v > 1.0f) { v = 1.0f; }
                    float pwm_f = 1000.0f + (v * 1000.0f);
                    pwm[i] = (uint16_t)pwm_f; // pwm_f ∈ [1000,2000]
                }
                // Safety clamp (defence in depth)
                if (pwm[i] < PWM_SAFE_MIN) { pwm[i] = PWM_SAFE_MIN; }
                if (pwm[i] > PWM_SAFE_MAX) { pwm[i] = PWM_SAFE_MAX; }
            } else {
                pwm[i] = PWM_DISARMED;
            }
        }
        send_frame(pwm);
    } else {
        // Normal mode: read actuator_motors
        actuator_motors_s motors;
        if (_actuator_sub.update(&motors)) {
            // Only send if at least one motor is armed (not NaN)
            bool any_armed = false;
            for (uint8_t i = 0; i < 4; i++) {
                float ctrl = motors.control[i];
                if (!PX4_ISFINITE(ctrl)) {
                    pwm[i] = PWM_DISARMED;
                } else {
                    any_armed = true;
                    if (ctrl < 0.0f) {
                        // Bidirectional range [-1,1] → [1000,2000]
                        if (ctrl < -1.0f) { ctrl = -1.0f; }
                        float pwm_f = 1500.0f + (ctrl * 500.0f);
                        pwm[i] = (uint16_t)pwm_f; // pwm_f ∈ [1000,1500]
                    } else {
                        if (ctrl > 1.0f) { ctrl = 1.0f; }
                        float pwm_f = 1000.0f + (ctrl * 1000.0f);
                        pwm[i] = (uint16_t)pwm_f; // pwm_f ∈ [1000,2000]
                    }
                    // Safety clamp (defence in depth)
                    if (pwm[i] < PWM_SAFE_MIN) { pwm[i] = PWM_SAFE_MIN; }
                    if (pwm[i] > PWM_SAFE_MAX) { pwm[i] = PWM_SAFE_MAX; }
                }
            }

            if (any_armed) {
                // F-10: manual_control timeout — disarm if GCS lost
                bool mcs_timed_out = false;
                if (_last_mcs_valid_time != 0) {
                    uint64_t now_mcs = hrt_absolute_time();
                    if ((now_mcs - _last_mcs_valid_time) > MANUAL_CONTROL_TIMEOUT_US) {
                        mcs_timed_out = true;
                    }
                }

                if (mcs_timed_out) {
                    uint16_t safe[4] = {PWM_DISARMED, PWM_DISARMED, PWM_DISARMED, PWM_DISARMED};
                    send_frame(safe);
                    uint64_t now_warn = hrt_absolute_time();
                    if (now_warn - _last_mcs_timeout_warn > 5000000) {
                        PX4_WARN("manual_control timeout — sending disarm frame");
                        _last_mcs_timeout_warn = now_warn;
                    }
                } else {
                    send_frame(pwm);
                }

                // Diagnostic: log motor values + PWM every 2 seconds when armed
                uint64_t now_diag = hrt_absolute_time();
                if (now_diag - _last_diag_time > 2000000) {
                    bool throttle_near_zero = true;
                    for (int i = 0; i < 4; i++) {
                        if (PX4_ISFINITE(motors.control[i]) && motors.control[i] > 0.02f) {
                            throttle_near_zero = false;
                            break;
                        }
                    }
                    PX4_INFO("MOTORS: [%.3f %.3f %.3f %.3f] PWM: [%u %u %u %u] USB:%s%s",
                        (double)motors.control[0], (double)motors.control[1],
                        (double)motors.control[2], (double)motors.control[3],
                        pwm[0], pwm[1], pwm[2], pwm[3],
                        _connected ? "OK" : "FAIL",
                        throttle_near_zero ? " THROTTLE~0" : "");
                    _last_diag_time = now_diag;
                }
            } else {
                // F-02: any_armed=false — explicitly send disarm frame
                uint16_t safe[4] = {PWM_DISARMED, PWM_DISARMED, PWM_DISARMED, PWM_DISARMED};
                send_frame(safe);
            }
        }
    }

    // Run at 50Hz
    ScheduleDelayed(20_ms);
}

int ArduinoOutput::task_spawn(int argc, char *argv[])
{
    ArduinoOutput *instance = new ArduinoOutput();
    if (!instance) {
        PX4_ERR("alloc failed");
        return PX4_ERROR;
    }

    instance->ScheduleNow();
    _object.store(instance);
    _task_id = task_id_is_work_queue;
    return PX4_OK;
}

int ArduinoOutput::custom_command(int argc, char *argv[])
{
    return print_usage("unknown command");
}

int ArduinoOutput::print_usage(const char *reason)
{
    if (reason) {
        PX4_WARN("%s\n", reason);
    }
    PRINT_MODULE_USAGE_NAME("arduino_output", "driver");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
    return 0;
}

int ArduinoOutput::print_status()
{
    PX4_INFO("Running, UART FD: %d, PTY: %s", _uart_fd, _device ? _device : "(none)");
    PX4_INFO("Arduino: %s, sent: %u, failed: %u", _connected ? "CONNECTED" : "NOT CONNECTED", _send_ok, _send_fail);

    // manual_control_setpoint status
    manual_control_setpoint_s mcs;
    if (_manual_sub.copy(&mcs)) {
        PX4_INFO("manual_control_setpoint: valid=%s, timestamp=%llu",
            mcs.valid ? "true" : "false", (unsigned long long)mcs.timestamp);
    } else {
        PX4_INFO("manual_control_setpoint: no data");
    }

    // Latest actuator_motors
    actuator_motors_s motors;
    if (_actuator_sub.copy(&motors)) {
        PX4_INFO("actuator_motors: [%.3f %.3f %.3f %.3f]",
            (double)motors.control[0], (double)motors.control[1],
            (double)motors.control[2], (double)motors.control[3]);
    }

    bool testing = false;
    for (int i = 0; i < 4; i++) {
        if (PX4_ISFINITE(_test_value[i])) { testing = true; break; }
    }
    PX4_INFO("Test mode: %s", testing ? "ACTIVE" : "off");
    return 0;
}

extern "C" __EXPORT int arduino_output_main(int argc, char *argv[])
{
    return ArduinoOutput::main(argc, argv);
}
