#include "native_sensor_reader.h"
#include "shared_sensor_data.h"
#include "sensor_sanity.h"

#include <android/sensor.h>
#include <android/looper.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <time.h>
#include <sys/resource.h>
#include <drivers/drv_hrt.h>

#define SR_TAG "NativeSensor"
#define SR_LOGI(...) __android_log_print(ANDROID_LOG_INFO, SR_TAG, __VA_ARGS__)
#define SR_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, SR_TAG, __VA_ARGS__)

// Shared sensor data (defined in px4_jni.cpp)
extern SharedSensorData g_sensor_data;

// Global counters for UI
NativeSensorCounts g_sensor_counts;

static pthread_t sensor_thread;
static std::atomic<bool> sensor_running{false};
static ALooper* sensor_looper = nullptr;

// Cached CPU temperature — updated every 2 seconds, not on every baro event
static float cached_cpu_temp = 25.0f;
static struct timespec last_temp_read = {0, 0};

// Android → FRD body frame: phone flat, top=forward, screen up
// Android: X=right, Y=forward, Z=up
// FRD:     X=forward, Y=right, Z=down
static inline void phone_to_frd(float x, float y, float z, float* nx, float* ny, float* nz) {
    *nx = y;
    *ny = x;
    *nz = -z;
}

// Read CPU temperature from thermal zone (filesystem I/O — call sparingly)
static float read_cpu_temperature_raw() {
    for (int zone = 0; zone <= 4; zone++) {
        char path[64];
        snprintf(path, sizeof(path), "/sys/class/thermal/thermal_zone%d/temp", zone);
        FILE* f = fopen(path, "r");
        if (!f) continue;
        int raw = 0;
        if (fscanf(f, "%d", &raw) == 1) {
            fclose(f);
            float temp = (raw > 1000) ? raw / 1000.0f : (float)raw;
            if (temp >= 10.0f && temp <= 90.0f) return temp;
        } else {
            fclose(f);
        }
    }
    return 25.0f;
}

// Get cached CPU temperature — refreshes every 2 seconds
static float get_cpu_temperature() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    long elapsed_ms = (now.tv_sec - last_temp_read.tv_sec) * 1000
                    + (now.tv_nsec - last_temp_read.tv_nsec) / 1000000;
    if (elapsed_ms >= 2000 || last_temp_read.tv_sec == 0) {
        cached_cpu_temp = read_cpu_temperature_raw();
        last_temp_read = now;
    }
    return cached_cpu_temp;
}

// Process a single sensor event
static void process_event(const ASensorEvent& event) {
    switch (event.type) {

    case ASENSOR_TYPE_ACCELEROMETER:
    case 35: { // TYPE_ACCELEROMETER_UNCALIBRATED
        // سجّل وقت الاستقبال بـ PX4 HRT فوراً — هذا هو timestamp_sample الحقيقي
        const hrt_abstime receipt_us = hrt_absolute_time();
        float nx, ny, nz;
        phone_to_frd(event.acceleration.x, event.acceleration.y, event.acceleration.z,
                     &nx, &ny, &nz);
        // F-04: reject NaN / Inf / out-of-range samples before they reach EKF2.
        if (!imu_sample_sane(nx, ny, nz, ACCEL_MAX_ABS_MPS2)) {
            g_sensor_counts.rejected_imu.fetch_add(1, std::memory_order_relaxed);
            break;
        }
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.accel_mutex);
            g_sensor_data.accel.data[0] = nx;
            g_sensor_data.accel.data[1] = ny;
            g_sensor_data.accel.data[2] = nz;
            g_sensor_data.accel.timestamp_ns   = event.timestamp;
            g_sensor_data.accel.hrt_receipt_us = receipt_us;
            g_sensor_data.accel.temperature    = get_cpu_temperature();
            g_sensor_data.accel.has_new_data.store(true);
        }
        // F-11: serialize with publisher's wait_for() to avoid missed wakeups.
        {
            std::lock_guard<std::mutex> lk(g_sensor_data.notify_mutex);
        }
        g_sensor_data.notify_cv.notify_one();
        g_sensor_counts.imu.fetch_add(1, std::memory_order_relaxed);
        break;
    }

    case ASENSOR_TYPE_GYROSCOPE:
    case 16: { // TYPE_GYROSCOPE_UNCALIBRATED
        // سجّل وقت الاستقبال بـ PX4 HRT فوراً — هذا هو timestamp_sample الحقيقي
        const hrt_abstime receipt_us = hrt_absolute_time();
        float nx, ny, nz;
        phone_to_frd(event.uncalibrated_gyro.x_uncalib,
                     event.uncalibrated_gyro.y_uncalib,
                     event.uncalibrated_gyro.z_uncalib,
                     &nx, &ny, &nz);
        // F-04: reject NaN / Inf / out-of-range samples before they reach EKF2.
        if (!imu_sample_sane(nx, ny, nz, GYRO_MAX_ABS_RADPS)) {
            g_sensor_counts.rejected_imu.fetch_add(1, std::memory_order_relaxed);
            break;
        }
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.gyro_mutex);
            g_sensor_data.gyro.data[0] = nx;
            g_sensor_data.gyro.data[1] = ny;
            g_sensor_data.gyro.data[2] = nz;
            g_sensor_data.gyro.timestamp_ns   = event.timestamp;
            g_sensor_data.gyro.hrt_receipt_us = receipt_us;
            g_sensor_data.gyro.temperature    = get_cpu_temperature();
            g_sensor_data.gyro.has_new_data.store(true);
        }
        // F-11: serialize with publisher's wait_for() to avoid missed wakeups.
        {
            std::lock_guard<std::mutex> lk(g_sensor_data.notify_mutex);
        }
        g_sensor_data.notify_cv.notify_one();
        g_sensor_counts.imu.fetch_add(1, std::memory_order_relaxed);
        break;
    }

    case ASENSOR_TYPE_PRESSURE: {
        float pa = event.pressure * 100.0f; // hPa → Pa
        // F-09: reject NaN / Inf / out-of-range pressure so sum_pressure never turns into NaN.
        if (!baro_pa_sane(pa)) {
            g_sensor_counts.rejected_baro.fetch_add(1, std::memory_order_relaxed);
            break;
        }
        const float temp_c = get_cpu_temperature();
        if (!PX4_ISFINITE(temp_c)) {
            g_sensor_counts.rejected_baro.fetch_add(1, std::memory_order_relaxed);
            break;
        }
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.baro_mutex);
            g_sensor_data.baro.sum_pressure += pa;
            g_sensor_data.baro.sum_temperature += temp_c;
            g_sensor_data.baro.count++;
        }
        g_sensor_counts.baro.fetch_add(1, std::memory_order_relaxed);
        break;
    }

    case ASENSOR_TYPE_MAGNETIC_FIELD:
    case 14: { // TYPE_MAGNETIC_FIELD_UNCALIBRATED
        // µT → milligauss (*10), then phone→NED
        float mx = event.magnetic.x * 10.0f;
        float my = event.magnetic.y * 10.0f;
        float mz = event.magnetic.z * 10.0f;
        float nx, ny, nz;
        phone_to_frd(mx, my, mz, &nx, &ny, &nz);
        // F-04 + F-09: reject bogus samples before they poison sum_field.
        if (!imu_sample_sane(nx, ny, nz, MAG_MAX_ABS_MGAUSS)) {
            g_sensor_counts.rejected_mag.fetch_add(1, std::memory_order_relaxed);
            break;
        }
        {
            std::lock_guard<std::mutex> lock(g_sensor_data.mag_mutex);
            g_sensor_data.mag.sum_field[0] += nx;
            g_sensor_data.mag.sum_field[1] += ny;
            g_sensor_data.mag.sum_field[2] += nz;
            g_sensor_data.mag.count++;
        }
        g_sensor_counts.mag.fetch_add(1, std::memory_order_relaxed);
        break;
    }

    default:
        break;
    }
}

static void* sensor_thread_func(void* /*arg*/) {
    SR_LOGI("Sensor thread starting");

    // Elevate thread priority — Android equivalent of SCHED_FIFO
    // ANDROID_PRIORITY_URGENT_AUDIO = -19, highest non-root priority
    setpriority(PRIO_PROCESS, 0, -19);
    SR_LOGI("Thread priority set to -19 (urgent audio level)");

    ASensorManager* mgr = ASensorManager_getInstanceForPackage("com.ardophone.px4v17");
    if (!mgr) {
        SR_LOGE("ASensorManager_getInstanceForPackage failed");
        sensor_running = false;
        return nullptr;
    }

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    if (!looper) {
        SR_LOGE("ALooper_prepare failed");
        sensor_running = false;
        return nullptr;
    }
    sensor_looper = looper;

    // Create queue without callback — we poll events manually
    ASensorEventQueue* queue = ASensorManager_createEventQueue(
        mgr, looper, 1, nullptr, nullptr);
    if (!queue) {
        SR_LOGE("ASensorManager_createEventQueue failed");
        sensor_running = false;
        return nullptr;
    }

    // Try uncalibrated sensors first, fallback to calibrated
    const ASensor* accel = ASensorManager_getDefaultSensor(mgr, 35); // UNCALIBRATED
    if (!accel) accel = ASensorManager_getDefaultSensor(mgr, ASENSOR_TYPE_ACCELEROMETER);

    const ASensor* gyro = ASensorManager_getDefaultSensor(mgr, 16); // UNCALIBRATED
    if (!gyro) gyro = ASensorManager_getDefaultSensor(mgr, ASENSOR_TYPE_GYROSCOPE);

    const ASensor* mag = ASensorManager_getDefaultSensor(mgr, 14); // UNCALIBRATED
    if (!mag) mag = ASensorManager_getDefaultSensor(mgr, ASENSOR_TYPE_MAGNETIC_FIELD);

    const ASensor* baro = ASensorManager_getDefaultSensor(mgr, ASENSOR_TYPE_PRESSURE);

    // Register sensors: fastest rate + zero batch latency (no event batching)
    // ASensorEventQueue_registerSensor (API 26+) sets maxBatchReportLatencyUs = 0
    // This matches Java's registerListener(SENSOR_DELAY_FASTEST) behavior — instant delivery
    if (accel) {
        ASensorEventQueue_registerSensor(queue, accel, 0, 0);
        SR_LOGI("Accel registered (no-batch): %s", ASensor_getName(accel));
    } else {
        SR_LOGE("No accelerometer found");
    }

    if (gyro) {
        ASensorEventQueue_registerSensor(queue, gyro, 0, 0);
        SR_LOGI("Gyro registered (no-batch): %s", ASensor_getName(gyro));
    } else {
        SR_LOGE("No gyroscope found");
    }

    if (mag) {
        ASensorEventQueue_registerSensor(queue, mag, 0, 0);
        SR_LOGI("Mag registered (no-batch): %s", ASensor_getName(mag));
    } else {
        SR_LOGE("No magnetometer found");
    }

    if (baro) {
        ASensorEventQueue_registerSensor(queue, baro, 0, 0);
        SR_LOGI("Baro registered (no-batch): %s", ASensor_getName(baro));
    } else {
        SR_LOGE("No barometer found");
    }

    SR_LOGI("Sensor loop running (polling mode)");

    // Rate monitoring
    struct timespec rate_start;
    clock_gettime(CLOCK_MONOTONIC, &rate_start);
    uint64_t local_imu = 0, local_baro = 0, local_mag = 0;

    // Main loop — block until events arrive (woken by ALooper_wake on stop)
    while (sensor_running) {
        // Block indefinitely until sensor events arrive — zero CPU waste
        ALooper_pollOnce(-1, nullptr, nullptr, nullptr);

        // Drain all available events
        ASensorEvent events[64];
        int count;
        while ((count = ASensorEventQueue_getEvents(queue, events, 64)) > 0) {
            for (int i = 0; i < count; i++) {
                process_event(events[i]);
                // Count by type for rate log
                switch (events[i].type) {
                    case ASENSOR_TYPE_GYROSCOPE: case 16: local_imu++; break;
                    case ASENSOR_TYPE_PRESSURE: local_baro++; break;
                    case ASENSOR_TYPE_MAGNETIC_FIELD: case 14: local_mag++; break;
                }
            }
        }

        // Log rates every 5 seconds
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        long elapsed = now.tv_sec - rate_start.tv_sec;
        if (elapsed >= 5) {
            const uint64_t elapsed_u = (uint64_t)elapsed;
            const uint64_t imu_rate  = local_imu  / elapsed_u;
            const uint64_t baro_rate = local_baro / elapsed_u;
            const uint64_t mag_rate  = local_mag  / elapsed_u;
            SR_LOGI("RATES: IMU=%" PRIu64 "/s  Baro=%" PRIu64 "/s  Mag=%" PRIu64 "/s",
                imu_rate, baro_rate, mag_rate);
            if (imu_rate < 10) {
                SR_LOGE("WARNING: IMU rate critically low (%" PRIu64 "/s) — sensor flow may be stalled!", imu_rate);
            }
            local_imu = local_baro = local_mag = 0;
            rate_start = now;
        }
    }

    // Cleanup
    if (accel) ASensorEventQueue_disableSensor(queue, accel);
    if (gyro) ASensorEventQueue_disableSensor(queue, gyro);
    if (mag) ASensorEventQueue_disableSensor(queue, mag);
    if (baro) ASensorEventQueue_disableSensor(queue, baro);
    ASensorManager_destroyEventQueue(mgr, queue);

    SR_LOGI("Sensor thread stopped");
    return nullptr;
}

void native_sensor_start() {
    if (sensor_running) return;
    sensor_running = true;
    g_sensor_counts.imu.store(0);
    g_sensor_counts.baro.store(0);
    g_sensor_counts.mag.store(0);
    g_sensor_counts.rejected_imu.store(0);
    g_sensor_counts.rejected_baro.store(0);
    g_sensor_counts.rejected_mag.store(0);
    SR_LOGI("native_sensor_start() called — launching sensor thread");
    pthread_create(&sensor_thread, nullptr, sensor_thread_func, nullptr);
}

void native_sensor_stop() {
    if (!sensor_running) return;
    SR_LOGI("native_sensor_stop() called — stopping sensor thread");
    sensor_running = false;
    // Wake the looper so the thread exits from ALooper_pollOnce(-1)
    if (sensor_looper) ALooper_wake(sensor_looper);
    pthread_join(sensor_thread, nullptr);
    sensor_looper = nullptr;
    SR_LOGI("Sensor thread joined — fully stopped");
}
