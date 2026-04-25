# دليل استبدال مجلد PX4-Autopilot

> هذا الدليل يشرح كيفية استبدال مجلد PX4-Autopilot بنسخة أخرى مع ضمان عمل التطبيق.
> آخر تحديث: 2026-03-28

---

## المتطلبات

- **Python 3.8+** مع الحزم التالية:
  ```bash
  pip install empy==3.3.4 pyros-genmsg pyyaml pymavlink
  ```
- **Android Studio** مع NDK 29 و CMake 4.1.2
- **Java** (يأتي مع Android Studio في `jbr/`)

---

## خطوات الاستبدال

### الخطوة 1: نسخة احتياطية
```bash
cp -r AndroidApp/app/src/main/cpp/PX4-Autopilot  PX4-Autopilot_BACKUP
```

### الخطوة 2: استبدال المجلد
```bash
rm -rf AndroidApp/app/src/main/cpp/PX4-Autopilot
cp -r /path/to/new/PX4-Autopilot  AndroidApp/app/src/main/cpp/PX4-Autopilot
```

### الخطوة 3: تطبيق تعديلات Android (5 ملفات)

هذه التعديلات ضرورية لأن Android لا يدعم بعض وظائف POSIX:

#### 3.1 `platforms/common/px4_log.cpp`
- إضافة `#include <android/log.h>` بعد includes
- إضافة logcat output في `px4_log_modulename()` و `px4_log_raw()`
- تغيير `#if defined(__PX4_POSIX)` إلى `#if defined(__PX4_POSIX) && !defined(__PX4_ANDROID)`
- إضافة `bool use_color = false;` في بداية `px4_log_raw()`

#### 3.2 `platforms/common/px4_work_queue/WorkQueueManager.cpp`
- `SCHED_FIFO` → `SCHED_OTHER` داخل `#ifdef __PX4_ANDROID`
- `sched_priority = 0` داخل `#ifdef __PX4_ANDROID`

#### 3.3 `platforms/common/shutdown.cpp`
- إضافة `extern "C" void px4_android_request_restart();`
- إضافة block إعادة التشغيل عبر JNI: `#if defined(__PX4_ANDROID)`

#### 3.4 `platforms/posix/src/px4/common/tasks.cpp`
- `pthread_attr_setinheritsched` → `rv = 0` داخل `#ifndef __PX4_ANDROID`
- `pthread_attr_setschedpolicy` → `rv = 0` داخل `#ifndef __PX4_ANDROID`
- `pthread_cancel` → `pthread_kill(pid, SIGTERM)` داخل `#ifdef __PX4_ANDROID`

#### 3.5 `platforms/posix/src/px4/common/px4_daemon/server.cpp`
- `pthread_cancel` → `pthread_kill(thread->second, SIGTERM)` داخل `#ifdef __PX4_ANDROID`

### الخطوة 4: تعديل HITL + إزالة حماية الإطلاق
في `src/modules/rocket_gnc/RocketGNC.cpp`:
- إزالة `#ifdef __PX4_NUTTX` و `#endif` حول كود HITL (runtime check via SYS_HITL)
- إزالة `if (_launched)` من نشر `actuator_outputs_sim` — الأوامر تُنشر دائماً

### الخطوة 5: نسخ درايفر XQPOWER CAN
```bash
# نسخ الدرايفر من النسخة الحالية
cp -r PX4-Autopilot_BACKUP/src/drivers/xqpower_can \
      AndroidApp/app/src/main/cpp/PX4-Autopilot/src/drivers/xqpower_can
```
تأكد أن `CMakeLists.txt` يحتوي:
```cmake
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/drivers/xqpower_can/XqpowerCan.cpp
)
```

### الخطوة 6: تحديث المعاملات الجديدة
إذا أضاف PX4 الجديد معاملات جديدة:
1. أضفها في enum `params` في `generated/parameters/px4_parameters.hpp`
2. أضف metadata (name + default value) في نفس الملف
3. أضف `PARAM_TYPE_INT32` أو `PARAM_TYPE_FLOAT` في `parameters_type[]` array

### الخطوة 7: Build & Install
```bash
cd AndroidApp
./gradlew assembleDebug
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

---

## بنية النظام

### سلسلة أوامر السيرفو (مساران متوازيان)

```
                                    ┌→ simulator_mavlink → HIL_ACTUATOR_CONTROLS → PC (6DOF)
RocketGNC → actuator_outputs_sim ──┤
                                    └→ xqpower_can → CAN bus → XQPOWER Servos (real hardware)

RocketGNC → vehicle_torque_setpoint → Control Allocator → actuator_servos → servo_usb_output → USB → سيرفو (fallback)

QGC → actuator_test → servo_usb_output → USB → سيرفو (اختبار أرضي)
```

**xqpower_can (CAN — المسار الرئيسي للسيرفوهات):**
- يقرأ `actuator_outputs_sim` مباشرة من rocket_gnc
- يرسل عبر SLCAN → Waveshare USB_CAN_A → CAN bus → سيرفوهات XQPOWER
- تردد: 50 Hz
- فيدباك: DEBUG_FLOAT_ARRAY "SRV_FB" (cmd + actual + error)

**servo_usb_output (USB — مسار بديل عبر CP2102):**
- Priority 1: `actuator_servos` (من Control Allocator) — فقط عند التسليح
- Priority 2: `actuator_outputs_sim` (من RocketGNC) — فقط عند التسليح
- Priority 3: `actuator_test` (اختبار أرضي) — يعمل دائماً
- عند نزع التسليح → الزعانف تُصفَّر تلقائياً (0°)

### اتصال المحطة الأرضية
```
داخل الهاتف:
  PX4 modules <-> MAVLink UDP (127.0.0.1:14550 <-> 14551)

للاتصال عبر USB/ADB:
  QGC → localhost:5760 → [adb forward] → TCP bridge (5760) <-> UDP (14550)

للاتصال عبر Ethernet:
  MAVLink instance 2 → UDP مباشر إلى IP الكمبيوتر:14550 → QGC
```

### نظام Airframe

| Airframe | الوضع | SYS_HITL | الحساسات | الاستخدام |
|----------|-------|----------|----------|----------|
| 22000 | SITL | 1 | محاكي خارجي (6DOF PC) | محاكاة كاملة على PC |
| 22001 | HITL | 1 (تلقائي) | محاكي خارجي (6DOF PC) + سيرفوهات حقيقية | محاكاة مع أجهزة |
| 22002 | Real Flight | 0 | حساسات الهاتف + ضبط EKF2 | طيران حقيقي |

---

## معاملات CAN (XQPOWER)

| المعامل | النوع | الافتراضي | الوصف |
|---------|-------|-----------|-------|
| `XQCAN_ENABLE` | int | 1 | تفعيل/تعطيل الدرايفر |
| `XQCAN_LIMIT` | float | 25.0 | حد الزاوية بالدرجات |
| `XQCAN_NODE1..4` | int | 1..4 | عناوين CAN للسيرفوهات |
| `XQCAN_REV` | int | 0 | bitmask عكس اتجاه السيرفوهات |

---

## ملفات التوليد التلقائي

| ما يُولّد | المصدر | السكربت |
|----------|--------|---------|
| uORB topics (.h + .cpp) | `PX4-Autopilot/msg/*.msg` + `msg/versioned/*.msg` | `px_generate_uorb_topic_files.py` |
| MAVLink headers | `mavlink/message_definitions/v1.0/*.xml` | `mavgen.py` |
| FlightTasks_generated | `flight_mode_manager/Templates/*.em` | `generate_flight_tasks.py` |
| build_git_version.h | `git describe/rev-parse` | CMake `execute_process` |

---

## ملفات تبقى يدوية

| الملف | السبب |
|-------|-------|
| `generated/parameters/px4_parameters.hpp` | يحتاج إعادة توليد إذا أُضيفت معاملات جديدة |
| `generated/events/events_generated.h` | يحتاج إعادة توليد إذا أُضيفت أحداث جديدة |
| `generated/mixer_module/output_functions.hpp` | ثابت عادةً |
| `generated/uORBMessageFieldsGenerated.cpp/hpp` | بيانات مضغوطة لرسائل uORB |
| `apps.cpp` + `apps.h` | تسجيل الوحدات والأوامر (28 أمر مع xqpower_can) |
| `platforms/posix/apps.cpp` | نسخة مطابقة من apps.cpp |

---

## الأوامر المتاحة من MAVLink Console (28 أمر)

**Modules:**
`ekf2`, `commander`, `sensors`, `navigator`, `mavlink`, `logger`, `rocket_gnc`, `xqpower_can`, `control_allocator`, `land_detector`, `flight_mode_manager`, `manual_control`, `mc_att_control`, `mc_pos_control`, `mc_rate_control`, `fw_att_control`, `fw_rate_control`, `dataman`, `load_mon`

**System Commands:**
`param`, `uorb`, `ver`, `top`, `perf`, `listener`, `actuator_test`, `reboot`

---

## استكشاف الأخطاء

| الخطأ | الحل |
|-------|------|
| `Python not found` | تأكد من وجود Python في PATH |
| `uORB header generation failed` | تحقق من حزمتي `empy` و `pyros-genmsg` |
| `pthread_cancel undeclared` | تعديل Android مفقود في tasks.cpp أو server.cpp |
| `SCHED_FIFO` crash | تعديل Android مفقود في WorkQueueManager.cpp |
| `PARAM_XYZ not found` | أضف المعامل في px4_parameters.hpp (3 أماكن) |
| `PARAM_DEFINE_FLOAT` error | لا تُترجم rocket_gnc_params.c — البارامترات في px4_parameters.hpp |
| CAN لا يعمل | تأكد من توصيل USB_CAN_A (CH340) و XQCAN_ENABLE=1 |
| QGC لا يتصل عبر USB | `adb forward tcp:5760 tcp:5760` ثم اتصل بـ `localhost:5760` TCP |

---

## إضافة module جديد

1. أضف مسار السورس في `CMakeLists.txt` ضمن `PX4_SOURCES`
2. أضف مسار الـ include إذا لزم
3. سجّل الـ module في `apps.cpp` (extern + apps map)
4. انسخ `apps.cpp` لـ `platforms/posix/apps.cpp`
5. إذا كان الموديول يحتاج JNI (مثل xqpower_can)، أضف الربط في `px4_jni.cpp`
6. أعد البناء
