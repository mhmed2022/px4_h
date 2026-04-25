package com.ardophone.px4v17.bridge

object PX4Bridge {
    init { System.loadLibrary("px4phone_native") }

    // Start / Stop
    external fun startPX4(storagePath: String): Boolean
    external fun stopPX4()
    external fun isRunning(): Boolean

    // Native sensor counts (for UI rate display)
    external fun getNativeImuCount(): Long
    external fun getNativeBaroCount(): Long
    external fun getNativeMagCount(): Long
    external fun getNativeGpsCount(): Long

    // Vehicle state from uORB
    external fun getRoll(): Float
    external fun getPitch(): Float
    external fun getYaw(): Float
    external fun getAltitude(): Float
    external fun isArmed(): Boolean
    external fun getEKFStatus(): Int
    external fun getAirframeId(): Int
    external fun getMotor1(): Float
    external fun getMotor2(): Float
    external fun getMotor3(): Float
    external fun getMotor4(): Float
    external fun getMotorPwm1(): Int
    external fun getMotorPwm2(): Int
    external fun getMotorPwm3(): Int
    external fun getMotorPwm4(): Int
    external fun isMotorUsbConnected(): Boolean

    // Arduino USB Serial (CH340/CDC-ACM) — actuator output bridge
    // Must be called BEFORE startPX4() so the driver can find the PTY path
    external fun setArduinoUsbFd(fd: Int, baud: Int)

    // USB GPS UBX (u-blox binary protocol — reads directly in C++)
    external fun setGpsUsbFd(fd: Int)

    // CP210x telemetry radio (PTY bridge for mavlink -d)
    external fun setMavlinkTelemetryUsbFd(fd: Int)
}
