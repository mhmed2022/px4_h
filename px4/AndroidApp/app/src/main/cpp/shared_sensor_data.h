#pragma once

// ============================================================
// Ardophone — Shared Sensor Data
// Bridge between JNI (Kotlin writes) and Native Backends (C++ reads)
// ============================================================

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cstdint>
#include <cstring>
#include <drivers/drv_hrt.h>

struct SharedSensorData {
    // === Accelerometer (independent from gyro) ===
    struct {
        float data[3]{0,0,0};         // m/s^2 in FRD body frame
        float temperature{0};         // degrees Celsius
        int64_t timestamp_ns{0};      // ASensorEvent.timestamp (nanoseconds, BOOTTIME)
        hrt_abstime hrt_receipt_us{0};// hrt_absolute_time() لحظة استقبال الـ event — يُستخدم كـ timestamp_sample
        std::atomic<bool> has_new_data{false};
    } accel;

    // === Gyroscope (independent from accel) ===
    struct {
        float data[3]{0,0,0};         // rad/s in FRD body frame
        float temperature{0};         // degrees Celsius
        int64_t timestamp_ns{0};      // ASensorEvent.timestamp (nanoseconds, BOOTTIME)
        hrt_abstime hrt_receipt_us{0};// hrt_absolute_time() لحظة استقبال الـ event — يُستخدم كـ timestamp_sample
        std::atomic<bool> has_new_data{false};
    } gyro;

    // === Barometer ===
    // Accumulates samples between backend update() calls to avoid data loss.
    // feedBaro() adds to sums; backend reads, averages, and resets.
    struct {
        float sum_pressure{0};    // accumulated Pascals
        float sum_temperature{0}; // accumulated degrees Celsius
        uint32_t count{0};        // number of accumulated samples
    } baro;

    // === Magnetometer (compass) ===
    // Accumulates samples like baro to avoid data loss.
    struct {
        float sum_field[3]{0, 0, 0}; // accumulated milligauss in FRD body frame
        uint32_t count{0};
    } mag;

    // Each mutex protects one struct only
    std::mutex accel_mutex;
    std::mutex gyro_mutex;
    std::mutex baro_mutex;
    std::mutex mag_mutex;

    // Wake publisher immediately when new IMU data arrives.
    std::mutex notify_mutex;
    std::condition_variable notify_cv;
};

// Single global instance — defined in ardupilot_jni.cpp
extern SharedSensorData g_sensor_data;
