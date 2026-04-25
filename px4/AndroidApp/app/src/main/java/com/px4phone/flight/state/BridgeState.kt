package com.ardophone.px4v17.state

import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import java.util.concurrent.atomic.AtomicInteger
import java.util.concurrent.atomic.AtomicLong

object BridgeState {

    private val _running = MutableStateFlow(false)
    val running: StateFlow<Boolean> = _running.asStateFlow()

    val imuCount = AtomicLong(0)
    val baroCount = AtomicLong(0)
    val magCount = AtomicLong(0)
    val gpsCount = AtomicLong(0)
    val errorCount = AtomicInteger(0)

    private val _hasAccel = MutableStateFlow(false)
    val hasAccel: StateFlow<Boolean> = _hasAccel.asStateFlow()

    private val _hasGyro = MutableStateFlow(false)
    val hasGyro: StateFlow<Boolean> = _hasGyro.asStateFlow()

    private val _hasMag = MutableStateFlow(false)
    val hasMag: StateFlow<Boolean> = _hasMag.asStateFlow()

    private val _hasBaro = MutableStateFlow(false)
    val hasBaro: StateFlow<Boolean> = _hasBaro.asStateFlow()

    private val _hasTemp = MutableStateFlow(false)
    val hasTemp: StateFlow<Boolean> = _hasTemp.asStateFlow()

    private val _accelType = MutableStateFlow("")
    val accelType: StateFlow<String> = _accelType.asStateFlow()

    private val _gyroType = MutableStateFlow("")
    val gyroType: StateFlow<String> = _gyroType.asStateFlow()

    private val _magType = MutableStateFlow("")
    val magType: StateFlow<String> = _magType.asStateFlow()

    fun setRunning(value: Boolean) { _running.value = value }

    fun setSensorAvailable(accel: Boolean, gyro: Boolean, mag: Boolean, baro: Boolean, temp: Boolean) {
        _hasAccel.value = accel
        _hasGyro.value = gyro
        _hasMag.value = mag
        _hasBaro.value = baro
        _hasTemp.value = temp
    }

    fun setSensorTypes(accel: String, gyro: String, mag: String) {
        _accelType.value = accel
        _gyroType.value = gyro
        _magType.value = mag
    }

    fun clear() {
        _running.value = false
        imuCount.set(0)
        baroCount.set(0)
        magCount.set(0)
        gpsCount.set(0)
        errorCount.set(0)
        _hasAccel.value = false
        _hasGyro.value = false
        _hasMag.value = false
        _hasBaro.value = false
        _hasTemp.value = false
        _accelType.value = ""
        _gyroType.value = ""
        _magType.value = ""
    }
}
