package com.ardophone.px4v17.sensor

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorManager
import com.ardophone.px4v17.state.BridgeState
import kotlinx.coroutines.*

/**
 * IMU/Baro/Mag are read natively in C++ (native_sensor_reader.cpp).
 * This class only detects sensor availability for BridgeState UI.
 */
class SensorReader(private val context: Context) {

    private val sensorManager = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager

    private var cpuTempJob: Job? = null
    private var scope = CoroutineScope(Dispatchers.Default + SupervisorJob())

    fun start() {
        detectSensors()
        BridgeState.setRunning(true)
        cpuTempJob = scope.launch(Dispatchers.IO) {
            while (isActive) {
                delay(2000)
            }
        }
    }

    fun stop() {
        cpuTempJob?.cancel()
        cpuTempJob = null
        scope.cancel()
        scope = CoroutineScope(Dispatchers.Default + SupervisorJob())
    }

    private fun detectSensors() {
        val accelUncal = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER_UNCALIBRATED)
        val accel = accelUncal ?: sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)
        val gyroUncal = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE_UNCALIBRATED)
        val gyro = gyroUncal ?: sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE)
        val magUncal = sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD_UNCALIBRATED)
        val mag = magUncal ?: sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD)
        val pressure = sensorManager.getDefaultSensor(Sensor.TYPE_PRESSURE)
        val temp = sensorManager.getDefaultSensor(Sensor.TYPE_AMBIENT_TEMPERATURE)

        BridgeState.setSensorAvailable(
            accel = accel != null, gyro = gyro != null, mag = mag != null,
            baro = pressure != null, temp = temp != null
        )
        BridgeState.setSensorTypes(
            accel = if (accelUncal != null) "Raw" else if (accel != null) "Calibrated" else "",
            gyro = if (gyroUncal != null) "Raw" else if (gyro != null) "Calibrated" else "",
            mag = if (magUncal != null) "Raw" else if (mag != null) "Calibrated" else ""
        )
    }
}
