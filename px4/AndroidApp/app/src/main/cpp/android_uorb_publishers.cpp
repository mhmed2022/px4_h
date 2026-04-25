/**
 * android_uorb_publishers.cpp — Phase 3
 * الجسر بين SharedSensorData (native_sensor_reader) و PX4 uORB
 * يقرأ من SharedSensorData وينشر على uORB topics
 */
#include "android_uorb_publishers.h"
#include "shared_sensor_data.h"
#include "sensor_sanity.h"
#include "native_sensor_reader.h"

#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/sensor_accel.h>
#include <uORB/topics/sensor_gyro.h>
#include <uORB/topics/sensor_baro.h>
#include <uORB/topics/sensor_mag.h>
#include <uORB/topics/vehicle_attitude.h>
#include <uORB/topics/vehicle_status.h>
#include <uORB/topics/vehicle_local_position.h>
#include <uORB/topics/estimator_status.h>
#include <uORB/topics/actuator_motors.h>
#include <drivers/drv_hrt.h>
#include <px4_platform_common/module_params.h>  // param_find / param_get

#include <thread>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <unistd.h>

// PX4 Device ID format: (devtype << 16) | (address << 8) | (bus << 3) | bus_type
// bus_type=4 (SIMULATION), bus=0, address=0
// devtype: من drv_sensor.h — أنواع الحساسات المحاكية
#define ANDROID_DEVID_IMU   ((0x14 << 16) | 4)  // DRV_IMU_DEVTYPE_SIM + BusType_SIMULATION
#define ANDROID_DEVID_MAG   ((0x03 << 16) | 4)  // DRV_MAG_DEVTYPE_MAGSIM
#define ANDROID_DEVID_BARO  ((0x65 << 16) | 4)  // DRV_BARO_DEVTYPE_BAROSIM

extern SharedSensorData g_sensor_data;

static std::atomic<bool> s_running{false};
static std::thread s_publisher_thread;

// Mag values for publishing (only when new data arrives)
static float s_mag_x = 0.0f;
static float s_mag_y = 0.0f;
static float s_mag_z = 0.0f;

// === Atomic cache for UI reads (no semaphore needed from JNI) ===
// publisher_loop updates these from uORB; JNI getters read them lock-free.
static std::atomic<float> s_cached_roll{0.0f};
static std::atomic<float> s_cached_pitch{0.0f};
static std::atomic<float> s_cached_yaw{0.0f};
static std::atomic<float> s_cached_altitude{0.0f};
static std::atomic<bool>  s_cached_armed{false};
static std::atomic<int>   s_cached_nav_state{0};
static std::atomic<int>   s_cached_ekf_status{0};
static std::atomic<float> s_cached_motor_1{0.0f};
static std::atomic<float> s_cached_motor_2{0.0f};
static std::atomic<float> s_cached_motor_3{0.0f};
static std::atomic<float> s_cached_motor_4{0.0f};

// === Publishers (كتابة على uORB) ===
static uORB::Publication<sensor_accel_s> s_accel_pub{ORB_ID(sensor_accel)};
static uORB::Publication<sensor_gyro_s>  s_gyro_pub{ORB_ID(sensor_gyro)};
static uORB::Publication<sensor_baro_s>  s_baro_pub{ORB_ID(sensor_baro)};
static uORB::Publication<sensor_mag_s>   s_mag_pub{ORB_ID(sensor_mag)};
// === Subscribers (قراءة من uORB — لعرض الحالة في الواجهة) ===
static uORB::Subscription s_att_sub{ORB_ID(vehicle_attitude)};
static uORB::Subscription s_status_sub{ORB_ID(vehicle_status)};
static uORB::Subscription s_local_pos_sub{ORB_ID(vehicle_local_position)};
static uORB::Subscription s_ekf_sub{ORB_ID(estimator_status)};
static uORB::Subscription s_actuator_motors_sub{ORB_ID(actuator_motors)};

static void publisher_loop() {
    // HITL: skip phone accel/gyro so sensor_combined uses HIL_SENSOR data only.
    // Without this, phone sensors get instance 0 and rocket_gnc reads desk accel
    // instead of simulated thrust → launch never detected → fins stay zero.
    int32_t sys_hitl = 0;
    param_t p_hitl = param_find("SYS_HITL");
    if (p_hitl != PARAM_INVALID) { param_get(p_hitl, &sys_hitl); }
    const bool skip_imu = (sys_hitl == 1);

    while (s_running.load()) {

        // ========== Accelerometer (independent) ==========
        if (!skip_imu && g_sensor_data.accel.has_new_data.load()) {
            std::lock_guard<std::mutex> lock(g_sensor_data.accel_mutex);

            // timestamp_sample = وقت استقبال event الهاردوير (مسجَّل في native_sensor_reader لحظة الوصول)
            // timestamp        = وقت النشر على uORB — دائماً >= timestamp_sample
            // الفرق = latency حقيقي يستخدمه EKF بدون drift أو تحويل ساعات
            const hrt_abstime sample_ts = g_sensor_data.accel.hrt_receipt_us;
            const hrt_abstime now       = hrt_absolute_time();

            const float ax = g_sensor_data.accel.data[0];
            const float ay = g_sensor_data.accel.data[1];
            const float az = g_sensor_data.accel.data[2];
            // F-04 defence-in-depth: never publish NaN / Inf / out-of-range to uORB.
            if (!imu_sample_sane(ax, ay, az, ACCEL_MAX_ABS_MPS2)) {
                g_sensor_counts.rejected_imu.fetch_add(1, std::memory_order_relaxed);
                g_sensor_data.accel.has_new_data.store(false);
            } else {
                sensor_accel_s accel{};
                accel.timestamp          = now;
                accel.timestamp_sample   = sample_ts;
                accel.device_id          = ANDROID_DEVID_IMU;
                accel.x                  = ax;
                accel.y                  = ay;
                accel.z                  = az;
                accel.temperature        = g_sensor_data.accel.temperature;
                accel.samples            = 1;
                s_accel_pub.publish(accel);

                g_sensor_data.accel.has_new_data.store(false);
            }
        }

        // ========== Gyroscope (independent) ==========
        if (!skip_imu && g_sensor_data.gyro.has_new_data.load()) {
            std::lock_guard<std::mutex> lock(g_sensor_data.gyro_mutex);

            // timestamp_sample = وقت استقبال event الهاردوير (مسجَّل في native_sensor_reader لحظة الوصول)
            // timestamp        = وقت النشر على uORB — دائماً >= timestamp_sample
            const hrt_abstime sample_ts = g_sensor_data.gyro.hrt_receipt_us;
            const hrt_abstime now       = hrt_absolute_time();

            const float gx = g_sensor_data.gyro.data[0];
            const float gy = g_sensor_data.gyro.data[1];
            const float gz = g_sensor_data.gyro.data[2];
            // F-04 defence-in-depth: never publish NaN / Inf / out-of-range to uORB.
            if (!imu_sample_sane(gx, gy, gz, GYRO_MAX_ABS_RADPS)) {
                g_sensor_counts.rejected_imu.fetch_add(1, std::memory_order_relaxed);
                g_sensor_data.gyro.has_new_data.store(false);
            } else {
                sensor_gyro_s gyro{};
                gyro.timestamp           = now;
                gyro.timestamp_sample    = sample_ts;
                gyro.device_id           = ANDROID_DEVID_IMU;
                gyro.x                   = gx;
                gyro.y                   = gy;
                gyro.z                   = gz;
                gyro.temperature         = g_sensor_data.gyro.temperature;
                gyro.samples             = 1;
                s_gyro_pub.publish(gyro);

                g_sensor_data.gyro.has_new_data.store(false);
            }
        }

        // ========== Barometer ==========
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.baro_mutex);
            if (g_sensor_data.baro.count > 0) {
                // sum_pressure already in Pa (converted in native_sensor_reader.cpp: hPa × 100)
                const float mean_pa   = (float)(g_sensor_data.baro.sum_pressure    / g_sensor_data.baro.count);
                const float mean_temp = (float)(g_sensor_data.baro.sum_temperature / g_sensor_data.baro.count);

                // F-09: if the accumulator ever went non-finite / out-of-range, drop the whole batch.
                if (!baro_pa_sane(mean_pa) || !PX4_ISFINITE(mean_temp)) {
                    g_sensor_counts.rejected_baro.fetch_add(1, std::memory_order_relaxed);
                } else {
                    sensor_baro_s baro{};
                    baro.timestamp        = hrt_absolute_time();
                    baro.timestamp_sample = baro.timestamp;
                    baro.device_id        = ANDROID_DEVID_BARO;
                    baro.pressure         = mean_pa;
                    baro.temperature      = mean_temp;
                    s_baro_pub.publish(baro);
                }

                // صفّر العداد (always reset, even on rejection, to avoid poison persisting).
                g_sensor_data.baro.sum_pressure = 0;
                g_sensor_data.baro.sum_temperature = 0;
                g_sensor_data.baro.count = 0;
            }
        }

        // ========== Magnetometer (publish only when new data arrives) ==========
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.mag_mutex);
            if (g_sensor_data.mag.count > 0) {
                // Mean in milligauss, then / 1000 → Gauss (uORB expects Gauss).
                const float mean_mx_g = (float)(g_sensor_data.mag.sum_field[0] / g_sensor_data.mag.count) / 1000.0f;
                const float mean_my_g = (float)(g_sensor_data.mag.sum_field[1] / g_sensor_data.mag.count) / 1000.0f;
                const float mean_mz_g = (float)(g_sensor_data.mag.sum_field[2] / g_sensor_data.mag.count) / 1000.0f;

                // Always reset the accumulator before we decide to publish or not,
                // so that poisoned sums don't linger into the next iteration.
                g_sensor_data.mag.sum_field[0] = 0;
                g_sensor_data.mag.sum_field[1] = 0;
                g_sensor_data.mag.sum_field[2] = 0;
                g_sensor_data.mag.count = 0;

                // F-04 / F-09: limit check expressed in Gauss (8 G = 8000 mG).
                if (!imu_sample_sane(mean_mx_g, mean_my_g, mean_mz_g, MAG_MAX_ABS_MGAUSS / 1000.0f)) {
                    g_sensor_counts.rejected_mag.fetch_add(1, std::memory_order_relaxed);
                } else {
                    s_mag_x = mean_mx_g;
                    s_mag_y = mean_my_g;
                    s_mag_z = mean_mz_g;

                    sensor_mag_s mag{};
                    mag.timestamp        = hrt_absolute_time();
                    mag.timestamp_sample = mag.timestamp;
                    mag.device_id        = ANDROID_DEVID_MAG;
                    mag.x = s_mag_x;
                    mag.y = s_mag_y;
                    mag.z = s_mag_z;
                    s_mag_pub.publish(mag);
                }
            }
        }

        // === Update cached state for UI (every ~50ms = 20 Hz is enough for display) ===
        static hrt_abstime s_last_ui_update = 0;
        const hrt_abstime now_ui = hrt_absolute_time();
        if ((now_ui - s_last_ui_update) >= 50000) { // 50 ms
            s_last_ui_update = now_ui;

            vehicle_attitude_s att{};
            if (s_att_sub.copy(&att)) {
                float sinr = 2.0f * (att.q[0] * att.q[1] + att.q[2] * att.q[3]);
                float cosr = 1.0f - 2.0f * (att.q[1] * att.q[1] + att.q[2] * att.q[2]);
                s_cached_roll.store(atan2f(sinr, cosr) * 57.2957795f, std::memory_order_relaxed);

                float sinp = 2.0f * (att.q[0] * att.q[2] - att.q[3] * att.q[1]);
                s_cached_pitch.store(asinf(fmaxf(-1.0f, fminf(1.0f, sinp))) * 57.2957795f, std::memory_order_relaxed);

                float siny = 2.0f * (att.q[0] * att.q[3] + att.q[1] * att.q[2]);
                float cosy = 1.0f - 2.0f * (att.q[2] * att.q[2] + att.q[3] * att.q[3]);
                s_cached_yaw.store(atan2f(siny, cosy) * 57.2957795f, std::memory_order_relaxed);
            }

            vehicle_local_position_s pos{};
            if (s_local_pos_sub.copy(&pos)) {
                s_cached_altitude.store(-pos.z, std::memory_order_relaxed);
            }

            vehicle_status_s status{};
            if (s_status_sub.copy(&status)) {
                s_cached_armed.store(status.arming_state == 2, std::memory_order_relaxed);
                s_cached_nav_state.store(status.nav_state, std::memory_order_relaxed);
            }

            estimator_status_s ekf{};
            if (s_ekf_sub.copy(&ekf)) {
                if (ekf.filter_fault_flags != 0) {
                    s_cached_ekf_status.store(0, std::memory_order_relaxed); // Bad — faults
                } else if (ekf.pre_flt_fail_innov_heading || ekf.pre_flt_fail_innov_vel_horiz ||
                           ekf.pre_flt_fail_innov_vel_vert || ekf.pre_flt_fail_innov_height) {
                    s_cached_ekf_status.store(1, std::memory_order_relaxed); // OK — innovations failing
                } else {
                    s_cached_ekf_status.store(2, std::memory_order_relaxed); // Good — converged
                }
            }

            actuator_motors_s motors{};
            if (s_actuator_motors_sub.copy(&motors)) {
                const float m1 = PX4_ISFINITE(motors.control[0]) ? fmaxf(0.0f, fminf(1.0f, motors.control[0])) : 0.0f;
                const float m2 = PX4_ISFINITE(motors.control[1]) ? fmaxf(0.0f, fminf(1.0f, motors.control[1])) : 0.0f;
                const float m3 = PX4_ISFINITE(motors.control[2]) ? fmaxf(0.0f, fminf(1.0f, motors.control[2])) : 0.0f;
                const float m4 = PX4_ISFINITE(motors.control[3]) ? fmaxf(0.0f, fminf(1.0f, motors.control[3])) : 0.0f;
                s_cached_motor_1.store(m1, std::memory_order_relaxed);
                s_cached_motor_2.store(m2, std::memory_order_relaxed);
                s_cached_motor_3.store(m3, std::memory_order_relaxed);
                s_cached_motor_4.store(m4, std::memory_order_relaxed);
            }
        }

        {
            std::unique_lock<std::mutex> lk(g_sensor_data.notify_mutex);
            g_sensor_data.notify_cv.wait_for(lk, std::chrono::microseconds(5000));
        }
    }
}

void start_uorb_publishers() {
    s_running.store(true);
    s_publisher_thread = std::thread(publisher_loop);
}

void stop_uorb_publishers() {
    s_running.store(false);
    if (s_publisher_thread.joinable()) {
        s_publisher_thread.join();
    }
}

// === قراءة حالة المركبة من الـ atomic cache (بدون uORB semaphore — آمن للاستدعاء من أي thread) ===

float get_vehicle_roll()     { return s_cached_roll.load(std::memory_order_relaxed); }
float get_vehicle_pitch()    { return s_cached_pitch.load(std::memory_order_relaxed); }
float get_vehicle_yaw()      { return s_cached_yaw.load(std::memory_order_relaxed); }
float get_vehicle_altitude() { return s_cached_altitude.load(std::memory_order_relaxed); }
bool  get_vehicle_armed()    { return s_cached_armed.load(std::memory_order_relaxed); }
int   get_vehicle_nav_state(){ return s_cached_nav_state.load(std::memory_order_relaxed); }
int   get_ekf_status()       { return s_cached_ekf_status.load(std::memory_order_relaxed); }
float get_motor_output_1()   { return s_cached_motor_1.load(std::memory_order_relaxed); }
float get_motor_output_2()   { return s_cached_motor_2.load(std::memory_order_relaxed); }
float get_motor_output_3()   { return s_cached_motor_3.load(std::memory_order_relaxed); }
float get_motor_output_4()   { return s_cached_motor_4.load(std::memory_order_relaxed); }

int get_airframe_id() {
    int32_t val = 0;
    param_t p = param_find("SYS_AUTOSTART");
    if (p != PARAM_INVALID) { param_get(p, &val); }
    return val;
}

// Board stubs for Android (مطلوبة من commander)
#include <px4_platform_common/board_common.h>

extern "C" {

int board_power_off(int status)
{
    return 0;
}

int board_register_power_state_notification_cb(power_button_state_notification_t cb)
{
    return 0;
}

} // extern "C"
