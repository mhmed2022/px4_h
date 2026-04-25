# توثيق تعديلات xqpower_can - درايفر XQPOWER CAN للأندرويد

**التاريخ:** 2026-03-27
**الهدف:** التحكم بسيرفوهات XQPOWER CAN من هاتف أندرويد عبر محول Waveshare USB_CAN_A (CH340)

---

## الملفات المعدّلة

### 1. `app/src/main/cpp/PX4-Autopilot/src/drivers/xqpower_can/XqpowerCan.hpp`
**التعديل:** إضافة دعم منصة POSIX/Android بجانب NuttX
- إضافة `#ifdef __PX4_POSIX` مع includes لـ libusb, atomic, mutex
- إضافة أعضاء USB-Serial: `libusb_context`, `device_handle`, `bulk_ep_out/in`
- إضافة buffer استقبال Waveshare: `_slcan_rxbuf[512]`, `_slcan_rxpos`
- إضافة دوال: `usb_serial_open()`, `usb_serial_close()`, `ch340_set_baudrate()`
- إضافة `slcan_write()`, `slcan_read()` (وظائف bulk transfer للـ USB)
- إضافة `set_usb_fd()` static و `s_usb_fd` atomic لاستقبال FD من JNI
- نقل أعضاء FDCAN (`_fdcan`, `_rx_fifo0_base`, `_tx_fifo_base`) تحت `#else` (NuttX فقط)

### 2. `app/src/main/cpp/PX4-Autopilot/src/drivers/xqpower_can/XqpowerCan.cpp`
**التعديل:** إعادة كتابة كاملة مع دعم مزدوج (NuttX + Android)

#### قسم Android (`#ifdef __PX4_POSIX`):
- **تهيئة CH340 عبر libusb:**
  - `ch340_set_baudrate(2000000)` — بود ريت 2 ميجا للاتصال التسلسلي
  - تسلسل التهيئة مطابق لمكتبة usb-serial-for-android:
    1. قراءة الإصدار (request 0x5F)
    2. قراءة الحالة (request 0xA1)
    3. ضبط البود ريت (register 0x1312/0x0f2c)
    4. ضبط LCR: 8N1 (register 0x2518 = 0x00C3)
    5. تفعيل DTR+RTS (request 0xA4, wValue=0, wIndex=0xFF9F)
    6. قراءة حالة المودم (register 0x0706)
  - **إصلاح خطأ:** DTR/RTS كانت wValue و wIndex معكوسين

- **بروتوكول Waveshare الثنائي (ليس SLCAN!):**
  - **الإرسال (`can_send`):**
    ```
    0xAA [0xC0|DLC] [ID_LOW] [ID_HIGH] [DATA...] 0x55
    ```
  - **الاستقبال (`can_receive`):**
    ```
    0xAA [INFO] [ID_LOW] [ID_HIGH] [DATA...] 0x55
    INFO: bit5=extended, bit4=remote, bit0-3=DLC
    ```
  - **لا يحتاج أوامر تهيئة** (بدون C\r, S6\r, O\r)

- **`usb_serial_open(fd)`:**
  - يستخدم `libusb_wrap_sys_device()` لتغليف Android USB FD
  - `libusb_claim_interface(0)` للسيطرة على الواجهة
  - اكتشاف تلقائي لنقاط النهاية (bulk IN/OUT endpoints)

- **دعم إعادة الاتصال في `Run()`:**
  - مراقبة تغيير USB FD عبر `s_usb_fd` atomic
  - إعادة فتح USB عند تغير الـ FD

#### قسم NuttX (`#else`):
- كود FDCAN الأصلي محفوظ بدون تغيير

#### كود مشترك:
- جميع دوال بروتوكول XQPOWER (servo_set_position, servo_process_rx, إلخ)
- حلقة Run() مع دعم reconnect لـ POSIX
- custom_command (move, move_all, zero, release, test, sweep, status)

### 3. `app/src/main/cpp/CMakeLists.txt`
**التعديلات:**
- إضافة `XqpowerCan.cpp` لقائمة PX4_SOURCES
- إضافة مسار include لـ xqpower_can
- حذف `xqpower_can_params.c` و `rocket_gnc_params.c` من البناء
  (البارامترات تأتي من `px4_parameters.hpp` المُولّد)

### 4. `app/src/main/cpp/generated/parameters/px4_parameters.hpp`
**التعديل:** إضافة 7 بارامترات جديدة:
| البارامتر | النوع | القيمة الافتراضية | الوصف |
|-----------|-------|-------------------|-------|
| XQCAN_ENABLE | INT32 | 1 | تفعيل/تعطيل الدرايفر |
| XQCAN_LIMIT | FLOAT | 25.0 | حد زاوية الانحراف (درجات) |
| XQCAN_NODE1 | INT32 | 1 | CAN Node ID للسيرفو 1 |
| XQCAN_NODE2 | INT32 | 2 | CAN Node ID للسيرفو 2 |
| XQCAN_NODE3 | INT32 | 3 | CAN Node ID للسيرفو 3 |
| XQCAN_NODE4 | INT32 | 4 | CAN Node ID للسيرفو 4 |
| XQCAN_REV | INT32 | 0 | قناع عكس اتجاه السيرفوهات |

أُضيفت في 3 أماكن: enum, param_info_s array, type array.

### 5. `app/src/main/cpp/apps.cpp`
**التعديل:** تسجيل الموديول في قائمة تطبيقات PX4:
```cpp
extern "C" int xqpower_can_main(int argc, char *argv[]);
apps["xqpower_can"] = xqpower_can_main;
```

### 6. `app/src/main/cpp/px4_jni.cpp`
**التعديل:** إضافة دالة JNI لتمرير USB FD:
```cpp
namespace xqpower_can_jni {
    extern void set_usb_fd(int fd);
}

extern "C" JNIEXPORT void JNICALL
Java_com_ardophone_px4v17_bridge_PX4Bridge_setCanUsbFd(JNIEnv*, jobject, jint fd) {
    xqpower_can_jni::set_usb_fd(fd);
}
```

### 7. `app/src/main/java/com/px4phone/flight/bridge/PX4Bridge.kt`
**التعديل:** إضافة دالة JNI:
```kotlin
external fun setCanUsbFd(fd: Int)
```

### 8. `app/src/main/java/com/ardophone/px4v17/usb/UsbServoManager.kt`
**التعديلات:**
- توجيه USB حسب نوع الشريحة:
  - CH340 (VID=0x1A86) → `PX4Bridge.setCanUsbFd(fd)` (محول CAN)
  - CP2102/STM32 → `PX4Bridge.setServoUsbFd(fd)` (سيرفو مباشر)
- **إصلاح كراش Android 14+:** إضافة `.setPackage(context.packageName)` لـ PendingIntent
  مع implicit Intent (كان يسبب `IllegalArgumentException` مع `FLAG_MUTABLE`)

### 9. `local.properties`
**التعديل:** تغيير مسار SDK من Windows إلى Linux:
```
sdk.dir=/home/sajad/Android/Sdk
```

---

## إعداد بيئة البناء (على اللابتوب)
- تثبيت Android SDK (API 36)
- تثبيت NDK 29.0.14206865
- تثبيت CMake 4.1.2
- المسار: `~/Android/Sdk`

---

## الأوامر المتاحة في PX4 Console
```
xqpower_can start          # تشغيل الدرايفر
xqpower_can stop           # إيقاف الدرايفر
xqpower_can status         # حالة الاتصال والسيرفوهات
xqpower_can test           # اختبار ±10 درجات
xqpower_can sweep          # مسح ذهاب وإياب
xqpower_can move <ch> <deg> # تحريك سيرفو محدد
xqpower_can move_all <deg>  # تحريك جميع السيرفوهات
xqpower_can zero           # تصفير جميع السيرفوهات
xqpower_can release        # تحرير التحكم اليدوي
```

---

## ملاحظات مهمة
1. **البروتوكول:** Waveshare USB_CAN_A يستخدم بروتوكول ثنائي خاص (AA...55)، وليس SLCAN
2. **البود ريت التسلسلي:** 2,000,000 (2 Mbps) — ثابت لمحول Waveshare
3. **بود ريت CAN:** 500,000 (500 kbps) — مضبوط في المحول نفسه
4. **التوقيع:** APK مبني بمفتاح debug مختلف عن الأصلي — يتطلب حذف التطبيق القديم أولاً
