/* App map for Android PX4 build — all modules + system commands */
#include <cstdio>
#include <map>
#include <string>

#include "apps.h"

// ===== Modules (started from JNI, also accessible from console) =====
extern "C" int sensors_main(int argc, char *argv[]);
extern "C" int ekf2_main(int argc, char *argv[]);
extern "C" int commander_main(int argc, char *argv[]);
extern "C" int navigator_main(int argc, char *argv[]);
extern "C" int mavlink_main(int argc, char *argv[]);
extern "C" int logger_main(int argc, char *argv[]);
extern "C" int land_detector_main(int argc, char *argv[]);
extern "C" int mc_att_control_main(int argc, char *argv[]);
extern "C" int mc_pos_control_main(int argc, char *argv[]);
extern "C" int mc_rate_control_main(int argc, char *argv[]);
extern "C" int fw_att_control_main(int argc, char *argv[]);
extern "C" int fw_rate_control_main(int argc, char *argv[]);
extern "C" int control_allocator_main(int argc, char *argv[]);
extern "C" int flight_mode_manager_main(int argc, char *argv[]);
extern "C" int manual_control_main(int argc, char *argv[]);
extern "C" int dataman_main(int argc, char *argv[]);
extern "C" int load_mon_main(int argc, char *argv[]);
extern "C" int pwm_out_sim_main(int argc, char *argv[]);
extern "C" int simulator_mavlink_main(int argc, char *argv[]);
extern "C" int arduino_output_main(int argc, char *argv[]);

// ===== System commands (invoked from MAVLink console) =====
extern "C" int actuator_test_main(int argc, char *argv[]);
extern "C" int param_main(int argc, char *argv[]);
extern "C" int uorb_main(int argc, char *argv[]);
extern "C" int ver_main(int argc, char *argv[]);
extern "C" int top_main(int argc, char *argv[]);
extern "C" int perf_main(int argc, char *argv[]);
extern "C" int listener_main(int argc, char *argv[]);
extern "C" int reboot_main(int argc, char *argv[]);

void init_app_map(apps_map_type &apps)
{
    // Modules
    apps["sensors"] = sensors_main;
    apps["ekf2"] = ekf2_main;
    apps["commander"] = commander_main;
    apps["navigator"] = navigator_main;
    apps["mavlink"] = mavlink_main;
    apps["logger"] = logger_main;
    apps["land_detector"] = land_detector_main;
    apps["mc_att_control"] = mc_att_control_main;
    apps["mc_pos_control"] = mc_pos_control_main;
    apps["mc_rate_control"] = mc_rate_control_main;
    apps["fw_att_control"] = fw_att_control_main;
    apps["fw_rate_control"] = fw_rate_control_main;
    apps["control_allocator"] = control_allocator_main;
    apps["flight_mode_manager"] = flight_mode_manager_main;
    apps["manual_control"] = manual_control_main;
    apps["dataman"] = dataman_main;
    apps["load_mon"] = load_mon_main;
    apps["pwm_out_sim"] = pwm_out_sim_main;
    apps["simulator_mavlink"] = simulator_mavlink_main;
    apps["arduino_output"] = arduino_output_main;

    // System commands
    apps["actuator_test"] = actuator_test_main;
    apps["param"] = param_main;
    apps["uorb"] = uorb_main;
    apps["ver"] = ver_main;
    apps["top"] = top_main;
    apps["perf"] = perf_main;
    apps["listener"] = listener_main;
    apps["reboot"] = reboot_main;
}

void list_builtins(apps_map_type &apps)
{
    printf("Builtin Commands:\n");
    for (auto it = apps.begin(); it != apps.end(); ++it) {
        printf("  %s\n", it->first.c_str());
    }
}
