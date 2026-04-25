/**
 * px4_jni.cpp — Phase 3 + Phase 4 fixes
 * JNI bridge بين Kotlin و PX4 native
 * يشغّل native_sensor_reader + uorb_publishers + PX4 modules
 */
#include <jni.h>
#include <android/log.h>
#include <atomic>
#include <string>
#include <thread>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#include "shared_sensor_data.h"
#include "native_sensor_reader.h"
#include "android_uorb_publishers.h"
#include "mavlink_tcp_bridge.h"
#include "gps_usb_ubx.h"
#include "mavlink_pty_usb_bridge.h"
#include "arduino_pty_usb_bridge.h"
#include "arduino_output.hpp"

// CP210x telemetry: FD from UsbDeviceConnection (-1 = not connected)
static std::atomic<int> g_mavlink_telemetry_usb_fd{-1};

// PX4 platform init — حرج! بدونه لا يعمل hrt, work queues, params, uORB
#include <px4_platform_common/init.h>
#include <px4_platform_common/posix.h>
#include <drivers/drv_hrt.h>
#include <parameters/param.h>

namespace px4 {
    void init_once();
    void init(int argc, char *argv[], const char *process_name);
}

// مطلوب من px4_posix_impl.cpp (يُعرَّف هناك كـ extern)
extern pthread_t _shell_task_id;

#define TAG "PX4Phone"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

// بنية البيانات المشتركة (global — يستخدمها native_sensor_reader + uorb_publishers + JNI GPS)
SharedSensorData g_sensor_data;

// PX4 modules — نشغّلهم برمجياً بدل rcS script
// ملاحظة: uorb_main لم نعد نحتاجه — uORB يبدأ تلقائياً من px4::init_once()
extern "C" {
    int sensors_main(int argc, char *argv[]);
    int ekf2_main(int argc, char *argv[]);
    int commander_main(int argc, char *argv[]);
    int navigator_main(int argc, char *argv[]);
    int mavlink_main(int argc, char *argv[]);
    int logger_main(int argc, char *argv[]);
    int land_detector_main(int argc, char *argv[]);
    int mc_att_control_main(int argc, char *argv[]);
    int mc_pos_control_main(int argc, char *argv[]);
    int mc_rate_control_main(int argc, char *argv[]);
    int fw_att_control_main(int argc, char *argv[]);
    // fw_pos_control لا يوجد في PX4 — يوجد fw_lateral_longitudinal_control
    int fw_rate_control_main(int argc, char *argv[]);
    int control_allocator_main(int argc, char *argv[]);
    int flight_mode_manager_main(int argc, char *argv[]);
    int manual_control_main(int argc, char *argv[]);
    int dataman_main(int argc, char *argv[]);
    int load_mon_main(int argc, char *argv[]);
    int pwm_out_sim_main(int argc, char *argv[]);
    int simulator_mavlink_main(int argc, char *argv[]);
}

// PR-6 (C-4): lifecycle flags are touched from Kotlin (JNI thread) AND the PX4
// worker thread AND the PX4 shutdown hook → must be atomic to avoid torn
// reads / missed-wakeup on the `while (s_running)` spin in the worker loop.
static std::thread s_px4_thread;
static std::atomic<bool> s_running{false};
static std::atomic<bool> g_restart_requested{false};
static std::atomic<bool> s_platform_initialized{false};

// Called from shutdown.cpp when QGC requests reboot — no exit() on Android
extern "C" void px4_android_request_restart() {
    param_save_default(true);
    g_restart_requested.store(true);
    s_running.store(false);
    LOGI("PX4 Android: soft restart requested");
}

static void stop_modules_only() {
    LOGI("Stopping PX4 modules for restart...");
    arduino_pty_usb_bridge_stop();
    mavlink_pty_usb_bridge_stop();
    gps_usb_ubx_stop();
    { const char* a[]={"logger","stop",nullptr}; logger_main(2,(char**)a); }
    mavlink_tcp_bridge_stop();
    { const char* a[]={"mavlink","stop",nullptr}; mavlink_main(2,(char**)a); }
    { const char* a[]={"control_allocator","stop",nullptr}; control_allocator_main(2,(char**)a); }
    { const char* a[]={"fw_rate_control","stop",nullptr}; fw_rate_control_main(2,(char**)a); }
    { const char* a[]={"fw_att_control","stop",nullptr}; fw_att_control_main(2,(char**)a); }
    { const char* a[]={"mc_rate_control","stop",nullptr}; mc_rate_control_main(2,(char**)a); }
    { const char* a[]={"mc_pos_control","stop",nullptr}; mc_pos_control_main(2,(char**)a); }
    { const char* a[]={"mc_att_control","stop",nullptr}; mc_att_control_main(2,(char**)a); }
    { const char* a[]={"manual_control","stop",nullptr}; manual_control_main(2,(char**)a); }
    { const char* a[]={"flight_mode_manager","stop",nullptr}; flight_mode_manager_main(2,(char**)a); }
    { const char* a[]={"land_detector","stop",nullptr}; land_detector_main(2,(char**)a); }
    { const char* a[]={"navigator","stop",nullptr}; navigator_main(2,(char**)a); }
    { const char* a[]={"dataman","stop",nullptr}; dataman_main(2,(char**)a); }
    { const char* a[]={"commander","stop",nullptr}; commander_main(2,(char**)a); }
    { const char* a[]={"ekf2","stop",nullptr}; ekf2_main(2,(char**)a); }
    { const char* a[]={"sensors","stop",nullptr}; sensors_main(2,(char**)a); }
    { const char* a[]={"load_mon","stop",nullptr}; load_mon_main(2,(char**)a); }
    stop_uorb_publishers();
    usleep(1500000); // 1.5s for modules to stop
    LOGI("PX4 modules stopped");
}

static void start_px4_modules(const std::string& storage_path) {
    LOGI("Starting PX4 modules... (restart=%s)", s_platform_initialized.load() ? "yes" : "no");

    if (!s_platform_initialized.load()) {
        // تهيئة المسارات — ننشئ المجلد الأساسي وكل المجلدات الفرعية
        mkdir(storage_path.c_str(), 0755);
        chdir(storage_path.c_str());
        mkdir("logs", 0755);
        mkdir("log", 0755);
        mkdir("eeprom", 0755);
        mkdir("mavlink", 0755);
        mkdir("dataman", 0755);

        // ===== Phase 4: تهيئة منصة PX4 (مرة واحدة فقط!) =====
        px4::init_once();
        LOGI("PX4 platform initialized (hrt, work queues, params, uORB)");

        px4::init(0, nullptr, "px4");
        LOGI("PX4 init done");

        s_platform_initialized.store(true);
    }

    // ===== ضبط ملف حفظ البارامترات (دائماً — لإعادة تحميل بعد Restart) =====
    param_set_default_file("./eeprom/parameters");
    param_load_default();
    LOGI("Parameters loaded from: ./eeprom/parameters");

    // ===== ترتيب التشغيل مهم! =====
    // (مستوحى من ROMFS/px4fmu_common/init.d-posix/rcS)
    // ملاحظة: uORB يبدأ تلقائياً من px4::init_once() → px4_platform_init() → uorb_start()

    // 2. uORB publishers — يجب أن تبدأ قبل sensors حتى تكون بيانات الحساسات متاحة
    start_uorb_publishers();
    LOGI("uORB publishers started");

    // ===== انتظر حتى تتدفق بيانات الحساسات من Android =====
    // ASensorManager يحتاج ~500ms-1s قبل أن يبدأ بتسليم events.
    // بدون هذا الانتظار يبدأ commander قبل وصول أي بيانات → "Accel/Gyro/Baro missing".
    LOGI("Waiting 3s for Android sensor pipeline to warm up...");
    usleep(3000000); // 3 ثوانٍ — يضمن وصول أول batch من الحساسات قبل sensors_main
    LOGI("Sensor warm-up done — starting PX4 modules");

    // 3. Parameters
    //
    // السياسة:
    //   - أول تشغيل = SYS_AUTOSTART == 0 (تثبيت جديد أو بعد Reset من QGC)
    //   - عند أول تشغيل: تُضبط جميع بارميترات الـ airframe دفعةً واحدة
    //   - بعد ذلك: أي تغيير من المحطة الأرضية يُحفظ ويبقى حتى Reset
    //   - استثناء: CAL_*_ID تُضبط دائماً إذا كانت 0 (ضرورة Android)
    //              COM_ARM_SDCARD و COM_RC_IN_MODE دائماً (بنية تحتية Android)
    {
        param_t p = param_find("SYS_AUTOSTART");

        // ===== كشف أول تشغيل =====
        int32_t current_autostart = 0;
        if (p != PARAM_INVALID) {
            param_get(p, &current_autostart);
        }
        // أول تشغيل: SYS_AUTOSTART لم يُضبط بعد (يساوي 0)
        const bool first_run = (current_autostart == 0);

        // SYS_AUTOSTART: يُضبط على 4001 (quad_x) فقط في أول تشغيل
        // بعدها يحترم القيمة المحفوظة
        if (p != PARAM_INVALID) {
            if (first_run) {
                int32_t val = 4001;
                param_set(p, &val);
                current_autostart = 4001;
                LOGI("SYS_AUTOSTART = 4001 — FIRST RUN (Quadcopter 4001_quad_x), applying all defaults");
            } else {
                LOGI("SYS_AUTOSTART = %d — params preserved from previous session", current_autostart);
            }
        } else {
            LOGE("SYS_AUTOSTART param not found!");
        }

        // ===== كشف وضع HITL =====
        int32_t sys_hitl = 0;
        p = param_find("SYS_HITL");
        if (p != PARAM_INVALID) {
            param_get(p, &sys_hitl);
            LOGI("SYS_HITL = %d", sys_hitl);
        }

        // ===== HITL: CAL_ACC/GYRO must use sim sensor ID (runs EVERY start) =====
        // When SYS_HITL=1, sensor_combined must use HIL_SENSOR data (device 1310988)
        // not the real phone sensors (device 1310724).
        if (sys_hitl == 1) {
            p = param_find("CAL_ACC0_ID");
            if (p != PARAM_INVALID) { int32_t v = 1310988; param_set(p, &v); }
            p = param_find("CAL_GYRO0_ID");
            if (p != PARAM_INVALID) { int32_t v = 1310988; param_set(p, &v); }
            LOGI("HITL: CAL_ACC0_ID/CAL_GYRO0_ID = 1310988 (sim sensor)");
        }

        // ===== كشف تغيّر الـ airframe =====
        // نحفظ آخر airframe في SYS_AUTOCONFIG
        // إذا تغيّر → نطبّق كل الافتراضيات من جديد
        // إذا لم يتغيّر → تعديلات المستخدم تبقى محفوظة
        int32_t last_applied_airframe = 0;
        param_t p_autoconfig = param_find("SYS_AUTOCONFIG");
        if (p_autoconfig != PARAM_INVALID) {
            param_get(p_autoconfig, &last_applied_airframe);
        }
        const bool airframe_changed = (current_autostart != last_applied_airframe);

        if (airframe_changed) {
            LOGI("Airframe changed: %d -> %d — applying all defaults", last_applied_airframe, current_autostart);

            // حفظ الـ airframe الحالي لكشف التغيير في المرة القادمة
            if (p_autoconfig != PARAM_INVALID) {
                param_set(p_autoconfig, &current_autostart);
            }
        } else {
            LOGI("Airframe %d unchanged — user params preserved", current_autostart);
        }

        // ===== بارميترات الـ airframe =====
        // تُطبّق عند: أول تشغيل أو تغيّر الـ airframe
        // تبقى محفوظة عند: reboot بدون تغيير airframe
        if (first_run || airframe_changed) {

            // ============================================================
            // rc.mc_defaults — المعاملات المشتركة للطائرة الرباعية
            // ============================================================

            // -- نوع المركبة: 2 = Quadcopter --
            p = param_find("MAV_TYPE");
            if (p != PARAM_INVALID) { int32_t v = 2; param_set(p, &v); }

            // -- EKF2 أساسي (بدون GPS -- لا توجد وحدة GPS متصلة) --
            p = param_find("EKF2_GPS_CTRL");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // تعطيل دمج GPS
            p = param_find("EKF2_HGT_REF");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // الارتفاع من الباروميتر
            p = param_find("EKF2_BARO_CTRL");
            if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }  // تفعيل الباروميتر
            p = param_find("EKF2_MAG_TYPE");
            if (p != PARAM_INVALID) { int32_t v = 6; param_set(p, &v); }  // INIT: البوصلة للتهيئة فقط
            p = param_find("EKF2_MAG_CHECK");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // تعطيل فحص قوة المغناطيس
            p = param_find("SYS_HAS_GPS");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // إخبار النظام: لا يوجد GPS
            p = param_find("EKF2_GPS_CHECK");
            if (p != PARAM_INVALID) { int32_t v = 24; param_set(p, &v); }

            // -- EKF2 ضبط ضوضاء الحساسات (قيم قياسية للمروحية) --
            p = param_find("EKF2_ACC_NOISE");
            if (p != PARAM_INVALID) { float v = 0.5f; param_set(p, &v); }
            p = param_find("EKF2_GYR_NOISE");
            if (p != PARAM_INVALID) { float v = 0.015f; param_set(p, &v); }
            p = param_find("EKF2_GPS_V_NOISE");
            if (p != PARAM_INVALID) { float v = 0.3f; param_set(p, &v); }
            p = param_find("EKF2_GPS_P_NOISE");
            if (p != PARAM_INVALID) { float v = 0.5f; param_set(p, &v); }
            p = param_find("EKF2_BARO_NOISE");
            if (p != PARAM_INVALID) { float v = 2.0f; param_set(p, &v); }

            // -- EKF2 حدود السرعة (قياسية) --
            p = param_find("EKF2_VEL_LIM");
            if (p != PARAM_INVALID) { float v = 50.0f; param_set(p, &v); }
            p = param_find("EKF2_GPS_DELAY");
            if (p != PARAM_INVALID) { float v = 200.0f; param_set(p, &v); }

            // -- EKF2 ثوابت زمنية --
            p = param_find("EKF2_TAU_VEL");
            if (p != PARAM_INVALID) { float v = 0.5f; param_set(p, &v); }
            p = param_find("EKF2_TAU_POS");
            if (p != PARAM_INVALID) { float v = 0.5f; param_set(p, &v); }

            // -- EKF2 تعلّم انحياز المقياس --
            p = param_find("EKF2_ACC_B_NOISE");
            if (p != PARAM_INVALID) { float v = 0.0001f; param_set(p, &v); }
            p = param_find("EKF2_GYR_B_NOISE");
            if (p != PARAM_INVALID) { float v = 0.0001f; param_set(p, &v); }
            p = param_find("EKF2_ABL_LIM");
            if (p != PARAM_INVALID) { float v = 1.0f; param_set(p, &v); }
            p = param_find("EKF2_ABL_ACCLIM");
            if (p != PARAM_INVALID) { float v = 25.0f; param_set(p, &v); }
            p = param_find("EKF2_ABL_GYRLIM");
            if (p != PARAM_INVALID) { float v = 10.0f; param_set(p, &v); }
            p = param_find("EKF2_ABL_TAU");
            if (p != PARAM_INVALID) { float v = 0.2f; param_set(p, &v); }

            // -- EKF2 بوابات الابتكار --
            p = param_find("EKF2_GPS_P_GATE");
            if (p != PARAM_INVALID) { float v = 5.0f; param_set(p, &v); }
            p = param_find("EKF2_GPS_V_GATE");
            if (p != PARAM_INVALID) { float v = 5.0f; param_set(p, &v); }
            p = param_find("EKF2_BARO_GATE");
            if (p != PARAM_INVALID) { float v = 5.0f; param_set(p, &v); }

            // -- EKF2 بدون GPS --
            p = param_find("EKF2_NOAID_TOUT");
            if (p != PARAM_INVALID) { int32_t v = 500000; param_set(p, &v); }
            p = param_find("EKF2_NOAID_NOISE");
            if (p != PARAM_INVALID) { float v = 10.0f; param_set(p, &v); }

            // -- Failure Detection --
            p = param_find("FD_FAIL_P");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("FD_FAIL_R");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("FD_ESCS_EN");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }

            // -- فحوصات التسليح --
            p = param_find("COM_ARM_CHK_ESCS");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("COM_ARM_HFLT_CHK");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("COM_ARM_WO_GPS");
            if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }
            p = param_find("COM_ARM_ODID");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }

            // -- Failsafe --
            p = param_find("COM_ACT_FAIL_ACT");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("COM_LOW_BAT_ACT");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("NAV_DLL_ACT");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("NAV_RCL_ACT");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("GF_ACTION");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("CBRK_SUPPLY_CHK");
            if (p != PARAM_INVALID) { int32_t v = 894281; param_set(p, &v); }
            p = param_find("COM_DISARM_PRFLT");
            if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }
            p = param_find("COM_FLT_TIME_MAX");
            if (p != PARAM_INVALID) { int32_t v = -1; param_set(p, &v); }
            p = param_find("COM_PARACHUTE");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("COM_WIND_WARN");
            if (p != PARAM_INVALID) { float v = 12.0f; param_set(p, &v); }
            p = param_find("COM_WIND_MAX");
            if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }

            // -- Android/RC --
            p = param_find("COM_RC_IN_MODE");
            if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }  // Joystick via MAVLink (QGC Virtual Joystick)
            { int32_t chk = -1; param_get(param_find("COM_RC_IN_MODE"), &chk); LOGI("COM_RC_IN_MODE set → verified = %d", chk); }
            p = param_find("CBRK_USB_CHK");
            if (p != PARAM_INVALID) { int32_t v = 197848; param_set(p, &v); }  // تعطيل فحص USB
            p = param_find("SYS_HAS_GPS");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // لا يوجد GPS
            p = param_find("COM_ARM_WO_GPS");
            if (p != PARAM_INVALID) { int32_t v = 2; param_set(p, &v); }  // تحذير فقط بدون GPS
            p = param_find("SYS_HAS_NUM_ASPD");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("SYS_HAS_NUM_OF");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("SYS_HAS_NUM_DIST");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }

            // -- Logging --
            p = param_find("SDLOG_MODE");
            if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
            p = param_find("SDLOG_BACKEND");
            if (p != PARAM_INVALID) { int32_t v = 3; param_set(p, &v); }

            // ============================================================
            // معاملات خاصة بالـ airframe
            // ============================================================
            if (sys_hitl == 1) {
                // === HITL — محاكي خارجي ===
                p = param_find("EKF2_MAG_TYPE");
                if (p != PARAM_INVALID) { int32_t v = 5; param_set(p, &v); }
                // Simulated sensors — ضروري لـ HITL لاستقبال بيانات المحاكي
                p = param_find("SENS_EN_GPSSIM");
                if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }
                p = param_find("SENS_EN_BAROSIM");
                if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }
                p = param_find("SENS_EN_MAGSIM");
                if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }
                LOGI("Airframe HITL defaults applied");

            } else {
                // === طيران حقيقي — حساسات الهاتف ===
                p = param_find("SENS_EN_GPSSIM");
                if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
                p = param_find("SENS_EN_BAROSIM");
                if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
                p = param_find("SENS_EN_MAGSIM");
                if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }
                p = param_find("EKF2_MAG_TYPE");
                if (p != PARAM_INVALID) { int32_t v = 6; param_set(p, &v); }
                p = param_find("EKF2_MAG_ACCLIM");
                if (p != PARAM_INVALID) { float v = 30.0f; param_set(p, &v); }
                p = param_find("SYS_HAS_MAG");
                if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }
                p = param_find("COM_ARM_EKF_POS");
                if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }
                p = param_find("COM_ARM_EKF_VEL");
                if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }
                p = param_find("COM_ARM_EKF_HGT");
                if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }
                LOGI("Airframe Real Flight defaults applied");
            }

            // -- Control Allocator: Quad X rotor geometry (defensive — 4001_quad_x) --
            // Ensures control_allocator knows rotor count & geometry even if
            // the airframe script hasn't been processed yet at this point.
            p = param_find("CA_ROTOR_COUNT");
            if (p != PARAM_INVALID) { int32_t v = 4; param_set(p, &v); }
            // Rotor 0: front-right CW
            p = param_find("CA_ROTOR0_PX");
            if (p != PARAM_INVALID) { float v = 1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR0_PY");
            if (p != PARAM_INVALID) { float v = 1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR0_KM");
            if (p != PARAM_INVALID) { float v = 0.05f; param_set(p, &v); }
            // Rotor 1: rear-left CW
            p = param_find("CA_ROTOR1_PX");
            if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR1_PY");
            if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR1_KM");
            if (p != PARAM_INVALID) { float v = 0.05f; param_set(p, &v); }
            // Rotor 2: front-left CCW
            p = param_find("CA_ROTOR2_PX");
            if (p != PARAM_INVALID) { float v = 1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR2_PY");
            if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR2_KM");
            if (p != PARAM_INVALID) { float v = -0.05f; param_set(p, &v); }
            // Rotor 3: rear-right CCW
            p = param_find("CA_ROTOR3_PX");
            if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR3_PY");
            if (p != PARAM_INVALID) { float v = 1.0f; param_set(p, &v); }
            p = param_find("CA_ROTOR3_KM");
            if (p != PARAM_INVALID) { float v = -0.05f; param_set(p, &v); }
            LOGI("CA_ROTOR_COUNT=4, Quad-X geometry set");

            LOGI("All quadcopter airframe defaults applied (4001_quad_x)");
        }

        // ===== CAL_*_ID — دائماً إذا كانت 0 (ضرورة Android) =====
        // تُضبط إذا لم تكن موجودة. إذا عايَر المستخدم الحساسات عبر QGC تبقى قيمه.
        // في وضع HITL: لا نكتب ACC/GYRO لأننا ضبطناها أعلاه على 1310988 (sim sensor)
        {
            struct { const char* name; int32_t id; bool skip_in_hitl; } cal_ids[] = {
                {"CAL_ACC0_ID",  1310724, true },  // ANDROID_DEVID_IMU  (skip in HITL)
                {"CAL_GYRO0_ID", 1310724, true },  // ANDROID_DEVID_IMU  (skip in HITL)
                {"CAL_MAG0_ID",  196612,  false},  // ANDROID_DEVID_MAG
                {"CAL_BARO0_ID", 6619140, false},  // ANDROID_DEVID_BARO
            };
            for (auto& cal : cal_ids) {
                if (sys_hitl == 1 && cal.skip_in_hitl) continue;
                p = param_find(cal.name);
                if (p != PARAM_INVALID) {
                    int32_t cur = 0;
                    param_get(p, &cur);
                    if (cur == 0) {
                        param_set(p, &cal.id);
                        LOGI("%s = %d (Android device ID)", cal.name, cal.id);
                    }
                }
            }

            // CAL_MAG0_PRIO: تُضبط على 50 إذا كانت <= 0 (الافتراضي -1 = معطّل)
            p = param_find("CAL_MAG0_PRIO");
            if (p != PARAM_INVALID) {
                int32_t prio = 0;
                param_get(p, &prio);
                if (prio <= 0) {
                    int32_t v = 50;
                    param_set(p, &v);
                    LOGI("CAL_MAG0_PRIO = 50");
                }
            }
        }

        // COM_ARM_SDCARD = 0: دائماً — الهاتف لا يملك SD Card فيمنع ARM بدون هذا
        p = param_find("COM_ARM_SDCARD");
        if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }

        // معاملات ضرورية — تُطبَّق في كل تشغيل بغض النظر عن first_run
        p = param_find("SYS_HAS_GPS");
        if (p != PARAM_INVALID) { int32_t v = 0; param_set(p, &v); }  // لا يوجد GPS
        p = param_find("COM_ARM_WO_GPS");
        if (p != PARAM_INVALID) { int32_t v = 2; param_set(p, &v); }  // تحذير فقط بدون GPS
        p = param_find("CBRK_USB_CHK");
        if (p != PARAM_INVALID) { int32_t v = 197848; param_set(p, &v); }  // تعطيل فحص USB
        p = param_find("EKF2_MAG_TYPE");
        if (p != PARAM_INVALID) {
            int32_t cur = 0;
            param_get(p, &cur);

            if (sys_hitl == 1) {
                // HITL uses simulated heading source.
                int32_t v = 5; // NONE
                param_set(p, &v);

            } else if (cur == 5) {
                // Real flight: recover from previous forced-NONE setting.
                int32_t v = 6; // INIT
                param_set(p, &v);
                LOGI("EKF2_MAG_TYPE restored to INIT (6) for real flight");
            }
        }
        p = param_find("COM_CPU_MAX");
        if (p != PARAM_INVALID) { float v = -1.0f; param_set(p, &v); }  // تعطيل فحص CPU
        p = param_find("COM_RC_IN_MODE");
        if (p != PARAM_INVALID) { int32_t v = 1; param_set(p, &v); }  // Joystick via MAVLink (QGC Virtual Joystick)
        { int32_t chk = -1; param_get(param_find("COM_RC_IN_MODE"), &chk); LOGI("COM_RC_IN_MODE (every-run) → verified = %d", chk); }
        p = param_find("COM_ARM_EKF_POS");
        if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }  // تعطيل فحص موضع EKF
        p = param_find("COM_ARM_EKF_VEL");
        if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }  // تعطيل فحص سرعة EKF
        p = param_find("COM_ARM_EKF_HGT");
        if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }  // تعطيل فحص ارتفاع EKF
        p = param_find("COM_ARM_EKF_YAW");
        if (p != PARAM_INVALID) { float v = 0.0f; param_set(p, &v); }  // تعطيل فحص اتجاه EKF

        // ===== Failsafe: Land عند أي انقطاع — تُضبط فقط إذا لم يغيّرها المستخدم =====
        // NAV_RCL_ACT: 2 = Land عند فقد RC (جويستيك QGC)
        p = param_find("NAV_RCL_ACT");
        if (p != PARAM_INVALID) {
            int32_t cur = 0; param_get(p, &cur);
            if (cur == 0) { int32_t v = 2; param_set(p, &v); LOGI("NAV_RCL_ACT = 2 (Land)"); }
        }

        // NAV_DLL_ACT: 2 = Land عند انقطاع Data Link (WiFi/QGC)
        p = param_find("NAV_DLL_ACT");
        if (p != PARAM_INVALID) {
            int32_t cur = 0; param_get(p, &cur);
            if (cur == 0) { int32_t v = 2; param_set(p, &v); LOGI("NAV_DLL_ACT = 2 (Land)"); }
        }

        // COM_LOW_BAT_ACT: 2 = Land عند انخفاض البطارية (سيعمل عند إضافة قراءة VBAT)
        p = param_find("COM_LOW_BAT_ACT");
        if (p != PARAM_INVALID) {
            int32_t cur = 0; param_get(p, &cur);
            if (cur == 0) { int32_t v = 2; param_set(p, &v); LOGI("COM_LOW_BAT_ACT = 2 (Land)"); }
        }

    }

    // 3b. Load monitor (CPU/RAM — مطلوب من commander لفحص ما قبل الطيران)
    const char* lm_argv[] = {"load_mon", "start", nullptr};
    load_mon_main(2, (char**)lm_argv);
    LOGI("Load monitor started");

    // 4. Sensors module (يقرأ من uORB sensor topics ويعالجها)
    const char* sensors_argv[] = {"sensors", "start", nullptr};
    sensors_main(2, (char**)sensors_argv);
    LOGI("Sensors started");

    // 4. EKF2
    const char* ekf2_argv[] = {"ekf2", "start", nullptr};
    ekf2_main(2, (char**)ekf2_argv);
    LOGI("EKF2 started");

    // 5. Commander
    // Check SYS_HITL parameter: if 1, pass -h flag to enable HIL mode
    // (commander -h sets hil_state = HIL_STATE_ON, required for
    //  HIL_ACTUATOR_CONTROLS stream and sensor check bypass)
    int32_t sys_hitl = 0;
    param_get(param_find("SYS_HITL"), &sys_hitl);
    if (sys_hitl == 1) {
        const char* cmd_argv[] = {"commander", "start", "-h", nullptr};
        commander_main(3, (char**)cmd_argv);
        LOGI("Commander started with HIL mode (-h)");
    } else {
        const char* cmd_argv[] = {"commander", "start", nullptr};
        commander_main(2, (char**)cmd_argv);
        LOGI("Commander started");
    }

    // 6a. Dataman (تخزين المهمات في ملف — يجب أن يسبق Navigator)
    const char* dm_argv[] = {"dataman", "start", "-f", "./dataman/dataman", nullptr};
    dataman_main(4, (char**)dm_argv);
    LOGI("Dataman started (file-backed: ./dataman/dataman)");

    // 6b. Navigator
    const char* nav_argv[] = {"navigator", "start", nullptr};
    navigator_main(2, (char**)nav_argv);
    LOGI("Navigator started");

    // 7. Land detector (multicopter افتراضياً — يتغير حسب SYS_AUTOSTART)
    const char* ld_argv[] = {"land_detector", "start", "multicopter", nullptr};
    land_detector_main(3, (char**)ld_argv);

    // 8. Flight mode manager
    const char* fmm_argv[] = {"flight_mode_manager", "start", nullptr};
    flight_mode_manager_main(2, (char**)fmm_argv);

    // 9. Manual control
    const char* mc_argv[] = {"manual_control", "start", nullptr};
    manual_control_main(2, (char**)mc_argv);

    // 10. Controllers (كلهم — commander يفعّل المناسب حسب SYS_AUTOSTART)
    const char* mca_argv[] = {"mc_att_control", "start", nullptr};
    mc_att_control_main(2, (char**)mca_argv);

    const char* mcp_argv[] = {"mc_pos_control", "start", nullptr};
    mc_pos_control_main(2, (char**)mcp_argv);

    const char* mcr_argv[] = {"mc_rate_control", "start", nullptr};
    mc_rate_control_main(2, (char**)mcr_argv);

    const char* fwa_argv[] = {"fw_att_control", "start", nullptr};
    fw_att_control_main(2, (char**)fwa_argv);

    // fw_pos_control غير موجود في PX4 — يُستبدل بـ fw_lateral_longitudinal_control لاحقاً

    const char* fwr_argv[] = {"fw_rate_control", "start", nullptr};
    fw_rate_control_main(2, (char**)fwr_argv);

    const char* ca_argv[] = {"control_allocator", "start", nullptr};
    control_allocator_main(2, (char**)ca_argv);

    // 10b. Arduino USB Serial output driver
    // PTY bridge must be started first (from Java via setArduinoUsbFd)
    // The driver will be started below after PTY is ready

    // 10c. GPS USB UBX (reads u-blox binary protocol from USB GPS receiver)
    gps_usb_ubx_start();
    LOGI("GPS USB UBX reader started");

    // 10d. Arduino output driver — always start
    // Uses Java bulkTransfer (arduino_send_pwm_via_java) for USB I/O
    // PTY bridge is optional — driver works without it
    {
        const char *arduino_pty = arduino_pty_usb_bridge_get_slave_path();
        if (arduino_pty) {
            ArduinoOutput::set_pty_path(arduino_pty);
        }
        const char *ard_argv[] = {"arduino_output", "start", nullptr};
        arduino_output_main(2, (char **)ard_argv);
        LOGI("Arduino output driver started (PTY=%s)", arduino_pty ? arduino_pty : "none — Java bridge");
    }

    // 11. MAVLink — UDP دائماً + CP210x telemetry إذا متصل
    {
        // UDP MAVLink — دائماً يعمل (للـ TCP bridge و WiFi)
        const char* mav_argv[] = {"mavlink", "start", "-u", "14550", "-o", "14551",
                                  "-t", "127.0.0.1", "-r", "10000", "-m", "config", nullptr};
        mavlink_main(12, (char**)mav_argv);
        LOGI("MAVLink started on UDP 14550, sending to 127.0.0.1:14551, rate=10000");

        // TCP bridge — دائماً يعمل (TCP:5760 ↔ UDP:14550)
        mavlink_tcp_bridge_start(5760, 14550);
        LOGI("TCP bridge started on port 5760 (for USB/ADB)");

        // CP210x telemetry — إضافي إذا متصل (instance ثاني من MAVLink)
        const int telem_fd = g_mavlink_telemetry_usb_fd.load();
        if (telem_fd >= 0 && mavlink_pty_usb_bridge_start(telem_fd, 115200)) {
            const char* pty = mavlink_pty_usb_bridge_get_slave_path();
            if (pty) {
                const char* mav2_argv[] = {"mavlink", "start",
                    "-d", pty, "-b", "115200", "-r", "10000", "-m", "0", nullptr};
                mavlink_main(10, (char**)mav2_argv);
                LOGI("MAVLink serial instance started (CP210x): %s baud=115200", pty);
            }
        } else if (telem_fd >= 0) {
            mavlink_pty_usb_bridge_stop();
            LOGE("CP210x PTY bridge failed — serial telemetry unavailable");
        }
    }

    // 12b. HITL: pwm_out_sim
    if (sys_hitl == 1) {
        const char* pwmsim_argv[] = {"pwm_out_sim", "start", nullptr};
        pwm_out_sim_main(2, (char**)pwmsim_argv);
        LOGI("pwm_out_sim started (HITL mode)");
    }

    // 13. Logger — MUST start after all publishing modules
    usleep(1000000); // 1s — ensure all work queue modules have run at least once

    const char* log_argv[] = {"logger", "start", "-f", "-t", nullptr};
    logger_main(4, (char**)log_argv);
    LOGI("Logger started (boot-to-shutdown mode)");

    // === Post-boot parameter verification ===
    {
        int32_t rc_mode = -1, rotor_count = -1;
        param_get(param_find("COM_RC_IN_MODE"), &rc_mode);
        param_get(param_find("CA_ROTOR_COUNT"), &rotor_count);
        LOGI("POST-BOOT CHECK: COM_RC_IN_MODE=%d  CA_ROTOR_COUNT=%d", rc_mode, rotor_count);
        if (rc_mode != 1) { LOGE("WARNING: COM_RC_IN_MODE is %d (expected 1) — manual_control may not work!", rc_mode); }
        if (rotor_count != 4) { LOGE("WARNING: CA_ROTOR_COUNT is %d (expected 4) — control_allocator misconfigured!", rotor_count); }
    }

    LOGI("All PX4 modules started successfully!");
}

// ===== JNI Functions =====

extern "C" JNIEXPORT jboolean JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_startPX4(
    JNIEnv* env, jobject, jstring storagePath) {

    if (s_running.load()) {
        LOGE("PX4 already running!");
        return JNI_FALSE;
    }

    // PR-6: null guard on JNI input — Kotlin call-sites never pass null today
    // but the JVM contract allows it, and GetStringUTFChars(nullptr) is UB.
    if (storagePath == nullptr) {
        LOGE("startPX4: storagePath is null");
        return JNI_FALSE;
    }

    const char* path = env->GetStringUTFChars(storagePath, nullptr);
    if (path == nullptr) {
        // OOM or pending exception — swallow it so we don't propagate into Java.
        if (env->ExceptionCheck()) env->ExceptionClear();
        LOGE("startPX4: GetStringUTFChars failed");
        return JNI_FALSE;
    }
    std::string storage(path);
    env->ReleaseStringUTFChars(storagePath, path);

    LOGI("Starting PX4 at: %s", storage.c_str());

    // 1. تشغيل native sensor reader (IMU/Baro/Mag مباشرة من ASensorManager)
    native_sensor_start();
    LOGI("Native sensor reader started");

    // 2. تشغيل PX4 modules في thread منفصل
    // ملاحظة: uORB publishers تبدأ بعد init_once لأنها تحتاج uORB يكون جاهز
    s_px4_thread = std::thread([storage]() {
        g_restart_requested.store(false);
        s_running.store(true);
        start_px4_modules(storage);

        while (true) {
            // PX4 modules تعمل في work queues — ننام حتى يُطلب إيقاف أو إعادة تشغيل
            while (s_running.load()) { sleep(1); }

            if (!g_restart_requested.load()) {
                break; // إيقاف طبيعي
            }

            // QGC طلب Reboot — نعيد تشغيل الـ modules
            LOGI("PX4 Android: executing soft restart...");
            stop_modules_only();
            g_restart_requested.store(false);
            s_running.store(true);
            start_px4_modules(storage); // init_once مُتجاهَل، params تُعاد
            LOGI("PX4 Android: soft restart complete");
        }
    });
    // H-1: thread is detached intentionally — FlightService.kt installs a 2 s
    // killProcess() failsafe that supersedes any in-process join timeout.
    s_px4_thread.detach();

    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_stopPX4(JNIEnv*, jobject) {
    if (!s_running.load()) {
        LOGI("PX4 already stopped — skipping");
        return;
    }
    s_running.store(false);
    arduino_pty_usb_bridge_stop();
    mavlink_pty_usb_bridge_stop();
    gps_usb_ubx_stop();
    mavlink_tcp_bridge_stop();
    stop_uorb_publishers();
    native_sensor_stop();
    LOGI("PX4 stopped");
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_isRunning(JNIEnv*, jobject) {
    return s_running.load() ? JNI_TRUE : JNI_FALSE;
}

// ===== عدادات الحساسات (للواجهة) =====

extern "C" JNIEXPORT jlong JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getNativeImuCount(JNIEnv*, jobject) {
    return g_sensor_counts.imu.load();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getNativeBaroCount(JNIEnv*, jobject) {
    return g_sensor_counts.baro.load();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getNativeMagCount(JNIEnv*, jobject) {
    return g_sensor_counts.mag.load();
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getNativeGpsCount(JNIEnv*, jobject) {
    return g_sensor_counts.gps.load();
}

// ===== حالة المركبة (من uORB → JNI → Kotlin UI) =====

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getRoll(JNIEnv*, jobject) {
    return get_vehicle_roll();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getPitch(JNIEnv*, jobject) {
    return get_vehicle_pitch();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getYaw(JNIEnv*, jobject) {
    return get_vehicle_yaw();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getAltitude(JNIEnv*, jobject) {
    return get_vehicle_altitude();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_isArmed(JNIEnv*, jobject) {
    return get_vehicle_armed() ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getEKFStatus(JNIEnv*, jobject) {
    return get_ekf_status();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getAirframeId(JNIEnv*, jobject) {
    return get_airframe_id();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotor1(JNIEnv*, jobject) {
    return get_motor_output_1();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotor2(JNIEnv*, jobject) {
    return get_motor_output_2();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotor3(JNIEnv*, jobject) {
    return get_motor_output_3();
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotor4(JNIEnv*, jobject) {
    return get_motor_output_4();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotorPwm1(JNIEnv*, jobject) {
    return static_cast<jint>(arduino_last_pwm_1());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotorPwm2(JNIEnv*, jobject) {
    return static_cast<jint>(arduino_last_pwm_2());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotorPwm3(JNIEnv*, jobject) {
    return static_cast<jint>(arduino_last_pwm_3());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_getMotorPwm4(JNIEnv*, jobject) {
    return static_cast<jint>(arduino_last_pwm_4());
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_isMotorUsbConnected(JNIEnv*, jobject) {
    return arduino_usb_connected() ? JNI_TRUE : JNI_FALSE;
}

// ===== MAVLink Telemetry USB (CP210x) =====

extern "C" JNIEXPORT void JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_setMavlinkTelemetryUsbFd(JNIEnv*, jobject, jint fd) {
    g_mavlink_telemetry_usb_fd.store(static_cast<int>(fd));
}

// ===== USB GPS UBX (u-blox binary protocol) =====

extern "C" JNIEXPORT void JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_setGpsUsbFd(JNIEnv*, jobject, jint fd) {
    gps_usb_set_fd(fd);
}

// ===== Arduino USB Serial (CH340) for actuator output =====

extern "C" JNIEXPORT void JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_setArduinoUsbFd(JNIEnv*, jobject, jint fd, jint baud) {
    if (fd >= 0) {
        bool ok = arduino_pty_usb_bridge_start(fd, baud);
        if (ok) {
            LOGI("Arduino USB bridge started (fd=%d, baud=%d)", fd, baud);
            // CRITICAL FIX: Update the driver with the new PTY path
            const char *path = arduino_pty_usb_bridge_get_slave_path();
            if (path) {
                ArduinoOutput::set_pty_path(path);
                LOGI("Arduino output driver PTY path updated: %s", path);
            }
        } else {
            LOGE("Arduino USB bridge FAILED to start (fd=%d, baud=%d)", fd, baud);
        }
    } else {
        arduino_pty_usb_bridge_stop();
        LOGI("Arduino USB bridge stopped (fd=-1)");
    }
}

// JNI_OnLoad — store VM pointer and UsbDeviceManager class for Java bulkTransfer
static JavaVM* g_jvm = nullptr;
static jclass g_UsbDeviceMgrClass = nullptr;
static jmethodID g_sendPwmDirectMethod = nullptr;

// PR-6 (C-3): bound the PWM frame at 64 B — Arduino protocol is 10 B today;
// any caller asking for more is a bug and must not reach NewByteArray.
static constexpr size_t kMaxPwmFrameBytes = 64;

// Java bulkTransfer bridge function (called at 50Hz from PX4 mixer thread)
static jint arduino_send_pwm_via_java_impl(const uint8_t* data, size_t len) {
    if (!g_jvm || !g_UsbDeviceMgrClass || !g_sendPwmDirectMethod) return -1;
    if (data == nullptr || len == 0 || len > kMaxPwmFrameBytes) return -1;

    JNIEnv* env = nullptr;
    int status = g_jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED) {
        if (g_jvm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return -1;
        }
        // Don't detach — called at 50Hz, keep thread attached
    } else if (status != JNI_OK) {
        return -1;
    }

    jbyteArray arr = env->NewByteArray(static_cast<jsize>(len));
    if (arr == nullptr || env->ExceptionCheck()) {
        if (env->ExceptionCheck()) env->ExceptionClear();
        return -1;
    }
    env->SetByteArrayRegion(arr, 0, static_cast<jsize>(len), reinterpret_cast<const jbyte*>(data));
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        env->DeleteLocalRef(arr);
        return -1;
    }

    jint result = env->CallStaticIntMethod(g_UsbDeviceMgrClass, g_sendPwmDirectMethod, arr);
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        result = -1;
    }

    env->DeleteLocalRef(arr);
    return result;
}

// Export for arduino_output driver
extern "C" __EXPORT int arduino_send_pwm_via_java(const uint8_t* data, size_t len) {
    return arduino_send_pwm_via_java_impl(data, len);
}

// PR-6 (C-2): JNI_OnLoad MUST return JNI_ERR on any failure — returning
// JNI_VERSION_* tells the runtime "ready to serve" and will surface later as
// opaque null-deref / JNI-ERROR crashes once arduino_send_pwm_via_java tries
// to use a missing class / methodID.
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    if (vm == nullptr) return JNI_ERR;
    g_jvm = vm;

    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JNI_OnLoad: GetEnv failed");
        return JNI_ERR;
    }

    jclass cls = env->FindClass("com/ardophone/px4v17/usb/UsbDeviceManager");
    if (cls == nullptr || env->ExceptionCheck()) {
        if (env->ExceptionCheck()) env->ExceptionClear();
        LOGE("JNI_OnLoad: FindClass(UsbDeviceManager) failed");
        return JNI_ERR;
    }

    g_UsbDeviceMgrClass = static_cast<jclass>(env->NewGlobalRef(cls));
    env->DeleteLocalRef(cls);
    if (g_UsbDeviceMgrClass == nullptr) {
        LOGE("JNI_OnLoad: NewGlobalRef(UsbDeviceManager) failed");
        return JNI_ERR;
    }

    g_sendPwmDirectMethod = env->GetStaticMethodID(g_UsbDeviceMgrClass, "sendPwmDirect", "([B)I");
    if (g_sendPwmDirectMethod == nullptr || env->ExceptionCheck()) {
        if (env->ExceptionCheck()) env->ExceptionClear();
        LOGE("JNI_OnLoad: sendPwmDirect method not found!");
        env->DeleteGlobalRef(g_UsbDeviceMgrClass);
        g_UsbDeviceMgrClass = nullptr;
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

// PR-6 (C-2): symmetrical teardown — the global ref must be released so it
// doesn't leak into the next process instance after System.loadLibrary reuse.
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void*) {
    if (vm == nullptr) return;
    JNIEnv* env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return;
    }
    if (g_UsbDeviceMgrClass != nullptr) {
        env->DeleteGlobalRef(g_UsbDeviceMgrClass);
        g_UsbDeviceMgrClass = nullptr;
    }
    g_sendPwmDirectMethod = nullptr;
    g_jvm = nullptr;
}
