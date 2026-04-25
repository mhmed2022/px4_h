/**
 * arduino_output.h
 * PX4 driver that sends actuator outputs to Arduino via USB Serial (PTY bridge).
 */
#pragma once

#include <px4_platform_common/module.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/actuator_motors.h>
#include <uORB/topics/actuator_test.h>
#include <uORB/topics/manual_control_setpoint.h>

using namespace time_literals;

class ArduinoOutput final : public ModuleBase<ArduinoOutput>, public px4::ScheduledWorkItem
{
public:
    ArduinoOutput();
    ~ArduinoOutput() override;

    static int task_spawn(int argc, char *argv[]);
    static int custom_command(int argc, char *argv[]);
    static int print_usage(const char *reason = nullptr);
    int print_status() override;

    static void set_pty_path(const char *path);

private:
    void Run() override;
    void send_frame(const uint16_t pwm[4]);

    uORB::Subscription _actuator_sub{ORB_ID(actuator_motors)};
    uORB::Subscription _test_sub{ORB_ID(actuator_test)};
    uORB::Subscription _manual_sub{ORB_ID(manual_control_setpoint)};
    int _uart_fd{-1};
    const char *_device{nullptr};

    float _test_value[4] {NAN, NAN, NAN, NAN};
    bool _connected{false};
    uint64_t _last_error_time{0};
    uint32_t _send_ok{0};
    uint32_t _send_fail{0};
    uint64_t _last_mcs_warn_time{0};
    bool _mcs_valid_reported{false};
    uint64_t _last_diag_time{0};
    uint64_t _last_mcs_valid_time{0};
    uint64_t _last_mcs_timeout_warn{0};
};

// C entry point
extern "C" __EXPORT int arduino_output_main(int argc, char *argv[]);

// Latest PWM frame successfully sent to Arduino over USB.
uint16_t arduino_last_pwm_1();
uint16_t arduino_last_pwm_2();
uint16_t arduino_last_pwm_3();
uint16_t arduino_last_pwm_4();
bool arduino_usb_connected();
