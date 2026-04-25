#pragma once

#include <atomic>
#include <cstdint>

// Start/stop native sensor reading (IMU, Baro, Mag)
// Reads directly from Android NDK ASensorManager, writes to SharedSensorData
void native_sensor_start();
void native_sensor_stop();

// Sensor sample counters (for UI display via JNI)
struct NativeSensorCounts {
    std::atomic<uint64_t> imu{0};
    std::atomic<uint64_t> baro{0};
    std::atomic<uint64_t> mag{0};
    std::atomic<uint64_t> gps{0};

    // Rejected samples (NaN / Inf / out-of-range) — diagnostics only.
    // Published for UI so operators can notice sensor glitches early.
    std::atomic<uint64_t> rejected_imu{0};
    std::atomic<uint64_t> rejected_baro{0};
    std::atomic<uint64_t> rejected_mag{0};
};

extern NativeSensorCounts g_sensor_counts;
