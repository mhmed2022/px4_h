# وثيقة عمليات الإطلاق — صاروخ M130 (منصة Android)

> **النظام:** PX4 على Android + RocketGNC + XQPOWER CAN Servos + u-blox GPS UBX
> **المنصة:** هاتف Android (Samsung Galaxy S23 Ultra) عبر تطبيق px4phone_native
> **الإطار:** 22002_m130_rocket_real (طيران حقيقي) / 22001_m130_rocket_hitl (محاكاة)
> **التكوين:** rocket_M130_v01_CFD68 (mass_dry=11.11 kg, propellant=1.63 kg, diameter=130 mm)

---

## 1. نظرة عامة على النظام

### 1.1 المكونات

| المكون | الوصف |
|--------|-------|
| **وحدة التحكم** | هاتف Android (arm64-v8a) — PX4 يعمل كـ JNI library |
| **IMU** | حساسات الهاتف عبر ASensorManager (Accel + Gyro) |
| **بارومتر** | حساس الهاتف عبر ASensorManager |
| **مغناطيسي** | حساس الهاتف عبر ASensorManager |
| **GPS** | u-blox NEO-M8N عبر USB (بروتوكول UBX ثنائي عبر PL2303) — 5 Hz / 115200 baud |
| **السيرفوات** | 4x XQPOWER CAN (بروتوكول CANopen SDO) |
| **CAN Bus** | Waveshare USB_CAN_A (CH340) → SLCAN binary protocol → 500 kbps CAN |
| **USB Hub** | OTG Hub يربط: CH340 (CAN) + PL2303 (GPS) |
| **التلمتري** | MAVLink TCP عبر ADB (port 5760) أو WiFi |

### 1.2 البرمجيات

| الموديول | المهمة | التردد | ملاحظة Android |
|----------|--------|--------|----------------|
| `rocket_gnc` | التحكم والتوجيه والملاحة | 100 Hz | مطابق للبوردة |
| `xqpower_can` | تحريك السيرفوات عبر CAN | 50 Hz | SLCAN عبر CH340 USB (بدل FDCAN مباشر) |
| `gps_usb_ubx` | GPS خارجي عبر UBX | 5 Hz | **جديد** — غير موجود في البوردة |
| `servo_usb_output` | سيرفوات عبر CP2102 USB | 100 Hz | **جديد** — احتياطي، مُعطّل عند عدم اتصال USB |
| `ekf2` | تقدير الحالة | 200 Hz | حساسات الهاتف (device_id=SIM) |
| `mavlink` | التلمتري | حسب الرسالة | UDP 14550 → TCP bridge 5760 |
| `native_sensor_reader` | قراءة حساسات الهاتف | 400 Hz | **خاص بـ Android** |
| `android_uorb_publishers` | نشر الحساسات على uORB | 400 Hz | **خاص بـ Android** |

### 1.3 سلسلة التحكم

```
rocket_gnc (100 Hz)
  |
  +-- vehicle_torque_setpoint --> control_allocator --> actuator_servos --> xqpower_can
  |                                                                          |
  +-- actuator_outputs_sim ------------------------------------------------> xqpower_can (أولوية 1)
                                                                             |
                                                                    Waveshare USB_CAN_A (CH340)
                                                                             |
                                                                    SLCAN binary --> CAN 500 kbps
                                                                             |
                                                                    4x XQPOWER Servo
```

### 1.4 سلسلة GPS

```
u-blox NEO-M8N (USB)
  |
  PL2303 USB-Serial
  |
  gps_usb_ubx.cpp (libusb bulk transfer)
  |
  GPSDriverUBX::receive() --> sensor_gps_s --> uORB (sensor_gps)
  |                                              |
  |                                        EKF2 (fusion)
  |
  configure() auto-detect baud --> 115200
  |
  device_id = 0x010000 (DRV_GPS_DEVTYPE_UBX)
```

**ملاحظة:** GPS الهاتف (LocationManager) **مُعطّل** — فقط u-blox الخارجي يُنشر على uORB.

### 1.5 خلط الزعانف (X-Fin Priority Mixer)

```
// Priority mixing: pitch/yaw أولاً، roll يأخذ المتبقي
ROLL_BUDGET = 0.30  (30% من نطاق الزعنفة)

py[0] = -tau_pitch + tau_yaw    // TR: -P+Y
py[1] = -tau_pitch - tau_yaw    // BR: -P-Y
py[2] = +tau_pitch - tau_yaw    // BL: +P-Y
py[3] = +tau_pitch + tau_yaw    // TL: +P+Y

fin[i] = py[i] + clamp(tau_roll, -1-py[i], 1-py[i])

حيث: tau = delta / ROCKET_MAX_DEFL (تطبيع من راديان إلى [-1, +1])
```

### 1.6 الفروقات عن البوردة (MicoAir H743)

| الجانب | البوردة | Android |
|--------|---------|---------|
| **IMU** | BMI088 + BMI270 (SPI) | حساسات الهاتف (ASensorManager) |
| **GPS** | NEO-M8N (UART مباشر) | NEO-M8N (USB عبر PL2303 + libusb) |
| **CAN** | FDCAN1 (سجلات مباشرة) | Waveshare USB_CAN_A (CH340 SLCAN) |
| **CAN delay** | ~0.1ms | ~2ms (USB overhead + 0.5ms inter-frame) |
| **GPS protocol** | يُهيّأ تلقائياً | يُهيّأ تلقائياً (auto-detect baud) |
| **GPS rate** | 1-10 Hz (حسب الإعداد) | 5 Hz (مضبوط من u-center) |
| **Sensor frame** | حسب تركيب البورد | FRD: Android (X,Y,Z) --> (Y,X,-Z) |
| **Clock sync** | HRT مباشر | إعادة مزامنة كل 5 ثوانٍ (drift correction) |

---

## 2. مراحل الطيران

### 2.1 التسلسل الزمني

```
[الأرض]  ARM --> انتظار --> إشعال المحرك
                                  |
                              ax > 1g + Delta-V > 2.0 m/s
                                  |
[المرحلة 0]  LAUNCH DETECTED     t = 0
                                  |
[0 -> T_CTRL]  Rail Time         الدفات = 0 (مرشحات IIR تُحمّل بقيم فعلية)
               (0.1s)
                                  |
[T_CTRL -> ]   التحكم يبدأ       الدفات تستجيب
                                  |
[المرحلة 1]   صعود + تثبيت ارتفاع  حتى |yaw_los| > 11 درجة أو t >= T_STG1
                                  |
[المرحلة 2]   ملاحة تناسبية PN2   حتى t >= 100s
```

### 2.2 شروط انتقال المراحل

| الانتقال | الشرط | الملاحظة |
|----------|-------|----------|
| IDLE --> Launch | `ax > 9.81 m/s2` **و** `Delta-V > 2.0 m/s` | كشف مزدوج (عتبة + تراكم) |
| المرحلة 1 --> 2 | `\|yaw_los_deg\| > 11 درجة` | الصاروخ اتجه نحو الهدف |
| المرحلة 1 --> 2 (احتياطي) | `t >= ROCKET_T_STG1` (4s) | انتهاء وقت المحرك |

### 2.3 التحكم حسب المرحلة

| المرحلة | التحكم بالـ Pitch | التحكم بالـ Yaw | التحكم بالـ Roll |
|---------|-------------------|-----------------|------------------|
| **1** | تثبيت ارتفاع (PID) حد ±10 درجة | توجيه مباشر (K_YAW x خطأ الانحراف) | تثبيت معدل اللف (K_w=0.0001, K_p=0.005) |
| **2** | ملاحة تناسبية منحازة (PN2) | ملاحة تناسبية (LOS rate) | تثبيت معدل اللف |

### 2.4 أنظمة الحماية

| الحماية | الشرط | الإجراء |
|---------|-------|---------|
| **Alpha Protection** | \|alpha\| > 12 درجة | تصفير أمر pitch + reset integrator |
| **Roll Rate Safety** | **غير موجود** في هذه النسخة | — |
| **Launch Guard** | t <= T_CTRL | الدفات = 0 + تحميل مرشحات بقيم فعلية |
| **Roll Budget** | \|tau_roll\| > 0.30 | قص عند 30% من نطاق الزعنفة |

---

## 3. البارامترات

### 3.1 بارامترات التوجيه والتحكم (ROCKET_*)

| البارامتر | القيمة الافتراضية | الوحدة | الوصف |
|-----------|-------------------|--------|-------|
| `ROCKET_SET_ALT` | 100.0 | m | ارتفاع مستهدف في المرحلة 1 |
| `ROCKET_T_STG1` | 4.0 | s | زمن احتراق المحرك / نهاية المرحلة 1 |
| `ROCKET_T_CTRL` | **0.1** | s | تأخير تفعيل التحكم (زمن السكة) |
| `ROCKET_NPN` | **2.7** | -- | ثابت الملاحة التناسبية |
| `ROCKET_IMP_ANG` | **-30.0** | درجة | زاوية الاصطدام النهائية |
| `ROCKET_TAU_PN1` | **20.0** | s | ثابت زمن الانحياز في PN2 |
| `ROCKET_K_YAW` | 0.008 | -- | ربح توجيه الانعراج |
| `ROCKET_K_VZ` | 14.0 | -- | ربح خطأ السرعة |
| `ROCKET_AYC_LIM` | 2.0 | g | حد تسارع الانعراج |
| `ROCKET_APC_LIM` | 8.0 | g | حد تسارع الميلان |
| `ROCKET_XTRGT` | **2600.0** | m | مدى الهدف (مسافة شمال من heading عند التسليح) |
| `ROCKET_YTRGT` | 0.0 | m | هدف الانحراف الجانبي (LOS yaw) |
| `ROCKET_ZTRGT` | 0.0 | m | — |
| `ROCKET_MAX_DEFL` | 0.436 | rad | حد انحراف الزعانف (25 درجة) |
| `ROCKET_GND_TEST` | 0 | -- | **حرج**: 0=طيران، 1=اختبار أرضي، 2=IMU live test |

> **تحذير**: القيم أعلاه هي **الافتراضية في الكود**. القيم الفعلية على الهاتف قد تختلف إذا تم تغييرها من QGC وحُفظت في eeprom.

### 3.2 بارامترات سيرفوات CAN (XQCAN_*)

| البارامتر | القيمة الافتراضية | الوصف |
|-----------|-------------------|-------|
| `XQCAN_ENABLE` | 1 | تفعيل درايفر CAN |
| `XQCAN_NODE1` | 1 | عنوان CAN للسيرفو 1 (TR) |
| `XQCAN_NODE2` | 2 | عنوان CAN للسيرفو 2 (BR) |
| `XQCAN_NODE3` | 3 | عنوان CAN للسيرفو 3 (BL) |
| `XQCAN_NODE4` | 4 | عنوان CAN للسيرفو 4 (TL) |
| `XQCAN_LIMIT` | 25.0 | حد الزاوية (±25 درجة) |
| `XQCAN_REV` | **0** | عكس اتجاه السيرفوات (bitmask) |

> **ملاحظة**: `XQCAN_REV` الافتراضي في Android = **0** (لا عكس). في البوردة كان **12** (0b1100 = عكس سيرفو 2 و 3). **تحقق من هذا قبل الإطلاق.**

### 3.3 بارامترات EKF2 المحفوظة على الهاتف

البارامترات التالية **مُعدّلة ومحفوظة** في eeprom الهاتف (تختلف عن الافتراضي):

#### ضجيج IMU

| البارامتر | القيمة المحفوظة | الوصف |
|-----------|----------------|-------|
| `EKF2_ACC_NOISE` | مُعدّل | ضجيج التسارع (لحساسات الهاتف) |
| `EKF2_GYR_NOISE` | مُعدّل | ضجيج الجايرو |
| `EKF2_ACC_B_NOISE` | مُعدّل | انجراف انحياز التسارع |
| `EKF2_GYR_B_NOISE` | مُعدّل | انجراف انحياز الجايرو |
| `EKF2_ABL_LIM` | مُعدّل | حد تعلّم الانحياز |
| `EKF2_ABL_GYRLIM` | مُعدّل | حد تعلّم أثناء الدوران |
| `EKF2_ABL_TAU` | مُعدّل | ثابت زمن تعلّم الانحياز |

#### GPS

| البارامتر | القيمة المحفوظة | الوصف |
|-----------|----------------|-------|
| `EKF2_GPS_V_NOISE` | مُعدّل | ضجيج سرعة GPS |
| `EKF2_GPS_P_NOISE` | مُعدّل | ضجيج موقع GPS |
| `EKF2_GPS_DELAY` | مُعدّل | تأخر GPS (ms) |
| `EKF2_GPS_P_GATE` | مُعدّل | بوابة ابتكار الموقع |
| `EKF2_GPS_V_GATE` | مُعدّل | بوابة ابتكار السرعة |
| `EKF2_GPS_CHECK` | مُعدّل | فحوصات GPS |

#### بارومتر ومغناطيسي

| البارامتر | القيمة المحفوظة | الوصف |
|-----------|----------------|-------|
| `EKF2_BARO_GATE` | مُعدّل | بوابة البارومتر |
| `EKF2_BARO_NOISE` | مُعدّل | ضجيج البارومتر |
| `EKF2_MAG_TYPE` | مُعدّل | نوع المغناطيسي |
| `EKF2_MAG_CHECK` | مُعدّل | فحص المغناطيسي |
| `EKF2_MAG_ACCLIM` | مُعدّل | حد تسارع المغناطيسي |

#### سرعة وتنبؤ

| البارامتر | القيمة المحفوظة | الوصف |
|-----------|----------------|-------|
| `EKF2_VEL_LIM` | مُعدّل | حد السرعة القصوى |
| `EKF2_PREDICT_US` | مُعدّل | فترة التنبؤ (µs) |
| `EKF2_TAU_VEL` | مُعدّل | تنعيم السرعة |
| `EKF2_TAU_POS` | مُعدّل | تنعيم الموقع |
| `EKF2_NOAID_NOISE` | مُعدّل | ضجيج بدون مساعدة |
| `EKF2_NOAID_TOUT` | مُعدّل | مهلة بدون مساعدة |

### 3.4 بارامترات النظام

| البارامتر | القيمة | الوصف |
|-----------|--------|-------|
| `MAV_TYPE` | 9 | نوع المركبة: صاروخ |
| `SYS_AUTOSTART` | 22002 (حقيقي) / 22001 (HITL) | إطار الطيران |
| `SYS_HITL` | 0 (حقيقي) / 1 (HITL) | يُضبط تلقائياً حسب AUTOSTART |
| `COM_RC_IN_MODE` | 4 | لا RC |
| `COM_DISARM_PRFLT` | مُعدّل | نزع تسليح تلقائي |

### 3.5 ملف البارامترات

```
المسار: /storage/emulated/0/Android/data/com.ardophone.px4v17/files/px4/eeprom/parameters
الحجم: ~2.5 KB
يحفظ: فقط البارامترات المُعدّلة (ليس الافتراضية)
يُحمّل: تلقائياً عند كل تشغيل للتطبيق
```

---

## 4. التوصيلات الفيزيائية

```
                    هاتف Android
                    +--[USB-C OTG]--+
                    |               |
                 USB Hub
                /       \
    [CH340]                [PL2303]
    Waveshare              محول USB-Serial
    USB_CAN_A
      |                       |
    CAN Bus               u-blox NEO-M8N
    500 kbps              115200 baud / UBX
      |
  +---+---+---+
  |   |   |   |
 S1  S2  S3  S4
 TR  BR  BL  TL
(0x601)(0x602)(0x603)(0x604)
```

---

## 5. إجراءات ما قبل الإطلاق

### المرحلة 1: التركيب والتوصيل

- [ ] ركّب الصاروخ على قاعدة الإطلاق
- [ ] وجّه الصاروخ نحو الهدف
- [ ] وصّل USB Hub بالهاتف عبر OTG
- [ ] تأكد من توصيل: CH340 (CAN) + PL2303 (GPS)
- [ ] تأكد من حرية حركة الزعانف الأربع

### المرحلة 2: تشغيل التطبيق والتحقق

#### 2.1 شاشة التطبيق — يجب أن ترى:
```
Sensors:
  GPS: 5Hz (أخضر)     ← u-blox متصل ويعمل
  Mag: 100Hz (أخضر)
  Baro: 25Hz (أخضر)
  IMU: 422Hz (أخضر)

Flight Status:
  EKF: OK (أخضر)      ← يحتاج GPS fix
  Armed: NO
```

#### 2.2 تحقق من GPS (من MAVLink Console):
```
listener sensor_gps
```
- [ ] `device_id: 65536` (UBX حقيقي — ليس 11468804 الهاتف)
- [ ] `fix_type >= 3` (3D Fix)
- [ ] `satellites_used >= 8`
- [ ] `eph < 5.0`
- [ ] `altitude_ellipsoid != altitude_msl` (فرق geoid صحيح)
- [ ] `time_utc_usec != 0` (توقيت UTC حقيقي)

#### 2.3 تحقق من EKF2:
```
listener vehicle_local_position
```
- [ ] `xy_global: True`
- [ ] `z_global: True`
- [ ] `dead_reckoning: False`
- [ ] `ref_lat` و `ref_lon` قيم صحيحة

#### 2.4 اختبار السيرفوات (من المحاكي أو NSH):
```
xqpower_can test
xqpower_can status
```
- [ ] 4/4 سيرفوات ONLINE
- [ ] الحركة بالاتجاه الصحيح
- [ ] الفيدباك يطابق الأمر (خطأ < 1 درجة)

### المرحلة 3: فحص البارامترات الحرجة

```
(من MAVLink Console أو QGC Parameters)
```

| البارامتر | القيمة المتوقعة | تحقق |
|-----------|----------------|------|
| `ROCKET_GND_TEST` | **0** | [ ] |
| `ROCKET_XTRGT` | المدى المطلوب (م) | [ ] |
| `ROCKET_SET_ALT` | 100.0 (أو حسب المهمة) | [ ] |
| `ROCKET_T_CTRL` | 0.1 | [ ] |
| `ROCKET_IMP_ANG` | -30.0 | [ ] |
| `ROCKET_MAX_DEFL` | 0.436 | [ ] |
| `XQCAN_REV` | حسب التوصيل | [ ] |
| `SYS_HITL` | **0** | [ ] |

### المرحلة 4: التسليح (ARM)

#### قبل التسليح:
- [ ] الصاروخ موجّه نحو الهدف
- [ ] المنطقة الأمامية آمنة
- [ ] `ROCKET_GND_TEST = 0`
- [ ] GPS 3D Fix نشط (5 Hz)
- [ ] جميع السيرفوات ONLINE

#### عند التسليح — رسائل متوقعة:
```
rocket_gnc: armed -- origin set to (x.xx, y.yy, z.zz)
rocket_gnc: heading=XXX.X deg  target range=XXXX m
xqpower: armed -- fins zeroed
```

#### تحقق بعد التسليح:
- [ ] heading يطابق اتجاه الهدف
- [ ] target range يطابق المسافة المطلوبة
- [ ] الزعانف لا تتحرك (طبيعي)

### المرحلة 5: الإطلاق

1. إشعال المحرك (خارجي)
2. كشف تلقائي عند `ax > 1g` **و** `Delta-V > 2.0 m/s`
3. رسالة:
```
LAUNCH DETECTED ax=XX.X m/s2
```

---

## 6. سجلات الطيران (ULog)

```
المسار: /storage/emulated/0/Android/data/com.ardophone.px4v17/files/px4/log/
الصيغة: ULog (.ulg)
البدء: تلقائي عند تشغيل التطبيق (boot-to-shutdown)
```

لسحب السجل:
```bash
adb pull /storage/emulated/0/Android/data/com.ardophone.px4v17/files/px4/log/sessXXX/log100.ulg
```

---

## 7. الفروقات الحرجة عن وثيقة البوردة

| الجانب | البوردة (H743) | Android |
|--------|---------------|---------|
| **CAN transport** | FDCAN مباشر (~0.1ms) | SLCAN عبر CH340 USB (~2ms + 0.5ms inter-frame) |
| **GPS** | UART مباشر | USB عبر PL2303 + GPSDriverUBX |
| **GPS phone** | لا يوجد | **مُعطّل** (android_uorb_publishers.cpp) |
| **sensor_combined** | BMI088/BMI270 | حساسات الهاتف |
| **HITL** | simulator_mavlink (TCP 4560) | MAVLink عبر TCP bridge (5760) — **بدون simulator_mavlink** |
| **T_CTRL** | 0.2s | **0.1s** |
| **T_STG1** | 15.0s | **4.0s** |
| **SET_ALT** | 120.0m | **100.0m** |
| **XTRGT** | 3000.0m | **2600.0m** |
| **Launch detection** | `ax > 1g` فقط | `ax > 1g` **+** `Delta-V > 2.0 m/s` (مزدوج) |
| **Roll gains** | K_w=0.08, K_p=0.02, K_i=0.005 | **K_w=0.0001, K_p=0.005, K_i=0.00** |
| **Alpha protection** | لا يوجد | **نعم** عند \|alpha\| > 12 درجة |
| **IMU live test** | لا يوجد | **GND_TEST=2** متاح |
| **Fin mixing** | مباشر (clamp) | **Priority** (ROLL_BUDGET=30%) |
| **Torque publish** | دائماً | **فقط بعد الإطلاق** |
| **Pre-launch filter** | بعد الإطلاق (rail time) | **قبل الإطلاق** (مستمر أثناء ARM) |

---

## 8. مراجع الملفات (Android)

| الملف | المسار |
|-------|--------|
| JNI Bridge | `app/src/main/cpp/px4_jni.cpp` |
| Native Sensor Reader | `app/src/main/cpp/native_sensor_reader.cpp` |
| uORB Publishers | `app/src/main/cpp/android_uorb_publishers.cpp` |
| GPS UBX Driver | `app/src/main/cpp/gps_usb_ubx.cpp` |
| Servo USB Output | `app/src/main/cpp/servo_usb_output.cpp` |
| MAVLink TCP Bridge | `app/src/main/cpp/mavlink_tcp_bridge.cpp` |
| Board Config | `app/src/main/cpp/px4_boardconfig.h` |
| RocketGNC | `app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.cpp` |
| RocketGNC Params | `app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/rocket_gnc_params.c` |
| XqpowerCan | `app/src/main/cpp/PX4-Autopilot/src/drivers/xqpower_can/XqpowerCan.cpp` |
| PX4Bridge (Kotlin) | `app/src/main/java/com/px4phone/flight/bridge/PX4Bridge.kt` |
| USB Device Manager | `app/src/main/java/com/ardophone/px4v17/usb/UsbServoManager.kt` |
| Parameters File | `/storage/emulated/0/Android/data/com.ardophone.px4v17/files/px4/eeprom/parameters` |
| ULog Files | `/storage/emulated/0/Android/data/com.ardophone.px4v17/files/px4/log/` |
