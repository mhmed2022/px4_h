#pragma once

// ============================================================
// Ardophone — Sensor sanity-check helpers (PR-4)
// F-04: reject NaN / Inf / out-of-range IMU / mag samples
// F-09: guard baro & mag accumulator sums against NaN
//
// The bounds are deliberately loose — they accept any realistic
// phone-class sensor reading and only catch driver glitches or
// uninitialised memory that would poison EKF2.
// ============================================================

#include <cmath>
#include <px4_platform_common/defines.h>

// Accelerometer: ±80 m/s² (~8 g) covers impact spikes on phone-class IMUs.
static constexpr float ACCEL_MAX_ABS_MPS2  = 80.0f;

// Gyroscope: ±35 rad/s (~2000 °/s) — upper bound of typical MEMS FSR.
static constexpr float GYRO_MAX_ABS_RADPS  = 35.0f;

// Magnetometer: ±8 Gauss (= 8000 milligauss).
// Earth field is < 1 G; the margin covers local magnetic disturbances.
static constexpr float MAG_MAX_ABS_MGAUSS  = 8000.0f;

// Barometer: 300 – 1200 hPa covers Mt. Everest summit (~330 hPa)
// to deep mines (~1100 hPa). Values outside this window are certainly bogus.
static constexpr float BARO_MIN_HPA = 300.0f;
static constexpr float BARO_MAX_HPA = 1200.0f;
static constexpr float BARO_MIN_PA  = BARO_MIN_HPA * 100.0f;
static constexpr float BARO_MAX_PA  = BARO_MAX_HPA * 100.0f;

// True iff every axis is finite AND |axis| <= max_abs.
static inline bool imu_sample_sane(float x, float y, float z, float max_abs)
{
    return PX4_ISFINITE(x) && PX4_ISFINITE(y) && PX4_ISFINITE(z)
        && fabsf(x) <= max_abs
        && fabsf(y) <= max_abs
        && fabsf(z) <= max_abs;
}

// True iff pressure (Pa) is finite and within the realistic atmospheric range.
static inline bool baro_pa_sane(float pa)
{
    return PX4_ISFINITE(pa) && pa >= BARO_MIN_PA && pa <= BARO_MAX_PA;
}

// True iff pressure (hPa) is finite and within the realistic atmospheric range.
static inline bool baro_hpa_sane(float hpa)
{
    return PX4_ISFINITE(hpa) && hpa >= BARO_MIN_HPA && hpa <= BARO_MAX_HPA;
}
