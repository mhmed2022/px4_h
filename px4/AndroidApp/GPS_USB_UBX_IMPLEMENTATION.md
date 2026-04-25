# GPS USB UBX — تنفيذ درايفر u-blox الثنائي

> **التاريخ:** 2026-03-29
> **الهدف:** استبدال مسار NMEA النصي بمسار UBX ثنائي مباشر لمستقبل u-blox عبر USB

---

## المشكلة

التطبيق كان يتعامل مع GPS الخارجي (u-blox) عبر مسار غير مباشر:
```
u-blox → USB → Kotlin (NMEA نصي) → JNI → SharedSensorData → uORB
```

هذا المسار يضيّع أغلب بيانات u-blox ويعطي PX4 بيانات ناقصة:
- `altitude_ellipsoid_m = altitude_msl_m` (خطأ — الفرق 20-100م)
- السرعة العمودية محسوبة من فرق الارتفاع (دقة ±3 م/ث بدل ±0.05 م/ث)
- `time_utc_usec = 0` (لا مزامنة وقت)
- `hdop/vdop = hAcc/5` (تقريب بدل قيم حقيقية)
- `heading = 0` بدل `NAN` (EKF2 قد يستخدمها كقياس)
- لا كشف تشويش (jamming) ولا انتحال (spoofing)

## الحل

مسار C++ مباشر يستخدم بروتوكول UBX الثنائي (نفس ما يفعله PX4 على Pixhawk):
```
u-blox → USB → C++ (UBX ثنائي عبر GPSDriverUBX) → sensor_gps uORB مباشرة
```

## الملفات المُضافة / المُعدَّلة

### ملفات جديدة:

| الملف | الوصف |
|-------|-------|
| `app/src/main/cpp/gps_usb_ubx.h` | هيدر — 3 دوال: start/stop/set_fd |
| `app/src/main/cpp/gps_usb_ubx.cpp` | التنفيذ — thread مخصص يقرأ UBX عبر libusb وينشر على uORB |

### ملفات مُعدَّلة:

| الملف | التعديل |
|-------|---------|
| `app/src/main/cpp/CMakeLists.txt` | إضافة include paths لـ GPS driver + مصادر ubx.cpp/gps_helper.cpp/rtcm.cpp/crc.cpp + إضافة gps_usb_ubx.cpp |
| `app/src/main/cpp/px4_jni.cpp` | إضافة `#include "gps_usb_ubx.h"` + استدعاء start/stop + دالة JNI `setGpsUsbFd` |
| `app/src/main/java/.../bridge/PX4Bridge.kt` | إضافة `external fun setGpsUsbFd(fd: Int)` |
| `app/src/main/java/.../usb/UsbServoManager.kt` | إضافة VID_UBLOX=0x1546 + توجيه fd + دعم أجهزة USB متعددة |

---

## التدفق التقني

### 1. اكتشاف الجهاز (Kotlin)
```
UsbServoManager يكتشف USB بـ VID=0x1546 (u-blox)
    → يفتح الجهاز ويأخذ file descriptor
    → يستدعي PX4Bridge.setGpsUsbFd(fd)
    → JNI يمرر fd لـ gps_usb_set_fd()
```

### 2. قراءة UBX (C++)
```
gps_usb_ubx.cpp thread:
    1. يستلم fd → يفتح عبر libusb_wrap_sys_device
    2. يكتشف bulk IN/OUT endpoints (CDC-ACM)
    3. ينشئ GPSDriverUBX (dynamic_model=8: Airborne <4g)
    4. يستدعي configure() — يكتشف الإصدار ويفعّل:
       - NAV-PVT (موقع + سرعة + وقت UTC)
       - NAV-DOP (HDOP/VDOP حقيقية)
       - MON-RF (كشف تشويش)
       - NAV-STATUS (كشف انتحال)
    5. حلقة: receive(200ms) → إذا وصل position update:
       - GPSDriverUBX يملأ sensor_gps_s بكل الحقول
       - نضبط device_id = 0x010000 (DRV_GPS_DEVTYPE_UBX)
       - ننشر على uORB: sensor_gps
```

### 3. وصول البيانات لـ EKF2
```
sensor_gps uORB → EKF2::UpdateGpsSample() (سطر 2430)
    → يبني gnssSample بكل الحقول:
       lat, lon, alt, vel_ned, hacc, vacc, sacc,
       fix_type, nsats, pdop, yaw, spoofed
    → _ekf.setGpsData(gnss_sample)
    → يحسب geoid_height = altitude_ellipsoid - altitude_msl
```

---

## GPS الهاتف كـ Fallback

GPS الهاتف (GpsReader.kt → feedGPS → SharedSensorData → android_uorb_publishers.cpp)
يبقى يعمل كما هو. عندما USB GPS متصل:
- PX4 sensors module يرى مصدرين GPS (device_id مختلف)
- EKF2 يستخدم المصدر ذو الجودة الأفضل (USB UBX)
- عند فصل USB GPS → يعود تلقائياً لـ GPS الهاتف

---

## مقارنة الحقول: قبل وبعد

| الحقل | قبل (NMEA) | بعد (UBX) |
|-------|-----------|----------|
| latitude_deg | NMEA 7 خانات | UBX 9 خانات (10× أدق) |
| longitude_deg | NMEA 7 خانات | UBX 9 خانات |
| altitude_msl_m | من GGA | من NAV-PVT hMSL |
| altitude_ellipsoid_m | = MSL (خطأ!) | من NAV-PVT height (صحيح) |
| vel_n/e_m_s | speed×cos/sin(bearing) | Doppler مباشرة (±0.05 م/ث) |
| vel_d_m_s | فرق ارتفاع (±3 م/ث) | Doppler مباشرة (±0.05 م/ث) |
| eph / epv | من hAcc Android | من NAV-PVT hAcc/vAcc (حقيقية) |
| hdop / vdop | hAcc/5 (تقريب) | من NAV-DOP (حقيقية) |
| time_utc_usec | 0 | نانوثانية من NAV-PVT |
| c_variance_rad | 0 | من NAV-PVT headAcc |
| heading | 0 (يعني شمال!) | NAN أو dual antenna |
| s_variance_m_s | hAcc×0.1 (تقريب) | من NAV-PVT sAcc (حقيقية) |
| jamming_indicator | 0 | من MON-RF jamInd |
| jamming_state | UNKNOWN | من MON-RF flags |
| spoofing_state | UNKNOWN | من NAV-STATUS flags2 |
| device_id | 0xAF0004 (SIM) | 0x010000 (UBX حقيقي) |

---

## دعم الأجهزة المتعددة

تم تحديث UsbServoManager لدعم أجهزة USB متعددة في نفس الوقت عبر USB Hub:
- **CP2102/STM32** → servo_usb_output (سيرفوات XQPOWER)
- **CH340** → xqpower_can (SLCAN)
- **u-blox (VID=0x1546)** → gps_usb_ubx (UBX binary)

التغيير: من `connectedDevice` واحد إلى `Map<deviceId, device/connection>` يدعم التعدد.

---

## ملاحظات تقنية

1. **dynamic_model = 8** (Airborne <4g) — مناسب للصاروخ M130
2. **USB CDC-ACM**: u-blox عبر USB يظهر كـ CDC-ACM مع interface 0 (control) + interface 1 (data)
3. **setBaudrate = no-op**: عبر USB البيانات تتدفق بسرعة USB الأصلية
4. **GPSDriverUBX**: نفس كود PX4 الرسمي (~2500 سطر) — لا تعديل عليه
5. **Thread مستقل**: لا يتداخل مع sensor publisher thread الحالي
