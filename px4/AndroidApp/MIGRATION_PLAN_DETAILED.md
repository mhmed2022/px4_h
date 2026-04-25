# خطة استبدال PX4-Autopilot - تفصيلية

> **التاريخ:** 2026-03-26
> **آخر تحديث:** 2026-03-28
> **الهدف:** استبدال مجلد PX4-Autopilot القديم بالجديد (من مجلد 66) مع ضمان نفس التشغيل + أتمتة توليد الملفات
> **ملف التوثيق:** بعد تنفيذ كل خطوة، يُوثّق في `MIGRATION_LOG.md`

---

## المسارات المرجعية

| الوصف | المسار |
|-------|--------|
| PX4 القديم (الحالي) | `AndroidApp/app/src/main/cpp/PX4-Autopilot/` |
| PX4 الجديد (المصدر) | `66/px/px/PX4-Autopilot/` |
| CMakeLists.txt | `AndroidApp/app/src/main/cpp/CMakeLists.txt` |
| الملفات المولّدة القديمة | `AndroidApp/app/src/main/cpp/generated/` |
| رسائل uORB المولّدة القديمة | `AndroidApp/app/src/main/cpp/generated_uorb_topics/` |
| MAVLink المولّدة القديمة | `AndroidApp/app/src/main/cpp/mavlink_generated/` |

---

## ملخص الخطوات

| # | الخطوة | الأولوية | التبعية | الحالة |
|---|--------|---------|---------|--------|
| 1 | نسخة احتياطية | حرجة | لا شيء | تم |
| 2 | استبدال مجلد PX4-Autopilot | حرجة | الخطوة 1 | تم |
| 3 | تطبيق تعديلات Android (5 ملفات) | حرجة | الخطوة 2 | تم |
| 4 | تعديل HITL في RocketGNC | مهمة | الخطوة 2 | تم |
| 5 | أتمتة توليد uORB topics | حرجة | الخطوة 2 | تم |
| 6 | أتمتة توليد الملفات الأخرى (generated/) | حرجة | الخطوة 2 | تم |
| 7 | أتمتة توليد MAVLink headers | حرجة | الخطوة 2 | تم |
| 8 | تحديث CMakeLists.txt | حرجة | الخطوات 5-7 | تم |
| 9 | حذف الملفات المولّدة القديمة | مهمة | الخطوات 5-8 | تم |
| 10 | بناء واختبار | حرجة | كل الخطوات | تم |
| 11 | تنظيف نهائي | عادية | الخطوة 10 | تم |
| 12 | كتابة وثيقة الأتمتة | عادية | الخطوة 10 | تم |
| 13 | إضافة Airframe 22002 (طيران حقيقي) | مهمة | الخطوة 10 | تم |
| 14 | إضافة XQPOWER CAN Servo Driver | مهمة | الخطوة 10 | تم |
| 15 | إصلاح نشر أوامر الزعانف قبل الإطلاق | مهمة | الخطوة 14 | تم |
| 16 | تحديث servo_usb_output (حماية arm) | مهمة | الخطوة 14 | تم |
| 17 | تسجيل 28 أمر MAVLink Console | مهمة | الخطوة 10 | تم |
| 18 | إصلاح كشف الاصطدام بالأرض في HITL | مهمة | الخطوة 4 | تم |

---

## الخطوة 1: نسخة احتياطية

### الهدف
حماية المشروع الحالي قبل أي تعديل.

### الإجراءات
```bash
# 1.1 نسخ PX4 القديم
cp -r AndroidApp/app/src/main/cpp/PX4-Autopilot  PX4-Autopilot_BACKUP

# 1.2 نسخ الملفات المولّدة القديمة
cp -r AndroidApp/app/src/main/cpp/generated        generated_BACKUP
cp -r AndroidApp/app/src/main/cpp/generated_uorb_topics  generated_uorb_topics_BACKUP
cp -r AndroidApp/app/src/main/cpp/mavlink_generated mavlink_generated_BACKUP

# 1.3 نسخ CMakeLists.txt
cp AndroidApp/app/src/main/cpp/CMakeLists.txt  CMakeLists_BACKUP.txt
```

### التحقق
- [x] المجلدات الأربعة موجودة في المسار الجذري
- [x] CMakeLists_BACKUP.txt موجود

### التوثيق
سجّل في `MIGRATION_LOG.md`: تاريخ النسخ، أحجام المجلدات، عدد الملفات.

---

## الخطوة 2: استبدال مجلد PX4-Autopilot

### الهدف
استبدال PX4 القديم بالجديد من مجلد 66.

### الإجراءات
```bash
# 2.1 حذف PX4 القديم
rm -rf AndroidApp/app/src/main/cpp/PX4-Autopilot

# 2.2 نسخ PX4 الجديد
cp -r 66/px/px/PX4-Autopilot  AndroidApp/app/src/main/cpp/PX4-Autopilot
```

### التحقق
- [x] المجلد الجديد موجود في `AndroidApp/app/src/main/cpp/PX4-Autopilot/`
- [x] التحقق من وجود 157 ملف/مجلد مرجعي في CMakeLists.txt — جميعها موجودة

---

## الخطوة 3: تطبيق تعديلات Android (5 ملفات)

### الهدف
PX4 الجديد **لا يحتوي** على تعديلات `__PX4_ANDROID`. يجب تطبيقها يدوياً.

### السبب
بدون هذه التعديلات، التطبيق لن يبني أو سيتعطل عند التشغيل لأن:
- أندرويد لا يدعم `SCHED_FIFO` (real-time scheduling)
- أندرويد لا يدعم `pthread_cancel`
- أندرويد يحتاج توجيه السجلات لـ logcat
- أندرويد يحتاج آلية إعادة تشغيل مختلفة

---

### 3.1 تعديل `platforms/common/px4_log.cpp`

**الملف:** `AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/common/px4_log.cpp`

**التعديل 1:** إضافة header بعد آخر `#include`:
```cpp
#ifdef __PX4_ANDROID
#include <android/log.h>
#endif
```

**التعديل 2:** داخل دالة `px4_log_modulename()` — بعد السطر الذي يحتوي `__px4_log_level_is_print`:
```cpp
#ifdef __PX4_ANDROID
	{
		static const int android_log_level[] = {
			ANDROID_LOG_DEBUG,    // _PX4_LOG_LEVEL_DEBUG
			ANDROID_LOG_INFO,     // _PX4_LOG_LEVEL_INFO
			ANDROID_LOG_WARN,     // _PX4_LOG_LEVEL_WARN
			ANDROID_LOG_ERROR,    // _PX4_LOG_LEVEL_ERROR
			ANDROID_LOG_FATAL     // _PX4_LOG_LEVEL_PANIC
		};
		char tag[32];
		snprintf(tag, sizeof(tag), "PX4.%s", module_name);
		va_list argptr;
		va_start(argptr, fmt);
		__android_log_vprint(android_log_level[level], tag, fmt, argptr);
		va_end(argptr);
	}
#endif
```

**التعديل 3:** نفس المنطق في دالة `px4_log_raw()` إن وُجدت.

**التعديل 4:** تغيير `#if defined(__PX4_POSIX)` إلى `#if defined(__PX4_POSIX) && !defined(__PX4_ANDROID)` لاستبعاد `isatty()`.

**التعديل 5:** إضافة `bool use_color = false;` في بداية `px4_log_raw()`.

**المرجع:** النسخة القديمة في `PX4-Autopilot_BACKUP/platforms/common/px4_log.cpp`

---

### 3.2 تعديل `platforms/common/px4_work_queue/WorkQueueManager.cpp`

**الملف:** `AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/common/px4_work_queue/WorkQueueManager.cpp`

**التعديل:** في دالة `WorkQueueManagerRun` (حوالي سطر 283-319)، استبدل:

**قبل** (الكود الأصلي):
```cpp
int sched_priority = sched_get_priority_max(SCHED_FIFO) + wq->relative_priority;
```

**بعد:**
```cpp
#ifdef __PX4_ANDROID
	// Android: SCHED_FIFO forbidden for normal apps, use SCHED_OTHER with priority 0
	int sched_priority = 0;
	(void)wq->relative_priority;
#else
	int sched_priority = sched_get_priority_max(SCHED_FIFO) + wq->relative_priority;
#endif
```

**وأيضاً** استبدل:
```cpp
int ret_setschedpolicy = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
```

**بـ:**
```cpp
#ifdef __PX4_ANDROID
	int ret_setschedpolicy = pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
#else
	int ret_setschedpolicy = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
#endif
```

---

### 3.3 تعديل `platforms/posix/src/px4/common/tasks.cpp`

**الملف:** `AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/posix/src/px4/common/tasks.cpp`

**التعديل 1:** `pthread_attr_setinheritsched`:
```cpp
#ifndef __PX4_ANDROID
	rv = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
#else
	rv = 0;  // Not supported on Android NDK
#endif
```

**التعديل 2:** `pthread_attr_setschedpolicy`:
```cpp
#ifndef __PX4_ANDROID
	rv = pthread_attr_setschedpolicy(&attr, scheduler);
#else
	rv = 0; (void)scheduler;  // Android: use default scheduler
#endif
```

**التعديل 3:** `pthread_cancel` → `pthread_kill`:
```cpp
#ifndef __PX4_ANDROID
	rv = pthread_cancel(pid);
#else
	rv = pthread_kill(pid, SIGTERM);  // pthread_cancel not available in Android NDK
#endif
```

**ملاحظة:** في الهجرة الأخيرة (2026-03-26)، tasks.cpp كان متطابقاً بين القديم والجديد ولم يحتج تعديل.

---

### 3.4 تعديل `platforms/common/shutdown.cpp`

**الملف:** `AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/common/shutdown.cpp`

**التعديل:** إضافة آلية إعادة التشغيل عبر JNI:
```cpp
#if defined(__PX4_ANDROID)
extern "C" void px4_android_request_restart();
#endif
```

وفي دالة إعادة التشغيل:
```cpp
#if defined(__PX4_ANDROID)
	px4_android_request_restart();
	return;
#elif defined(__PX4_POSIX)
	system_exit(0);
#endif
```

---

### 3.5 تعديل `platforms/posix/src/px4/common/px4_daemon/server.cpp`

**الملف:** `AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/posix/src/px4/common/px4_daemon/server.cpp`

**التعديل:** `pthread_cancel` → `pthread_kill` (نفس المشكلة في tasks.cpp):
```cpp
#ifndef __PX4_ANDROID
				pthread_cancel(thread->second);
#else
				pthread_kill(thread->second, SIGTERM);
#endif
```

### التحقق من الخطوة 3
- [x] كل ملف من الخمسة يحتوي على `__PX4_ANDROID`
- [x] مقارنة التعديلات مع النسخة القديمة (PX4-Autopilot_BACKUP) للتأكد من عدم نسيان شيء

```bash
# هذا الأمر يجب أن يُظهر نتائج في كل ملف
grep -r "__PX4_ANDROID" AndroidApp/app/src/main/cpp/PX4-Autopilot/platforms/
```

---

## الخطوة 4: تعديل HITL في RocketGNC

### الهدف
تمكين وضع HITL على أندرويد عبر تحويل الفحص من compile-time إلى runtime.

### الملف
`AndroidApp/app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.cpp`

### التعديل
**الكود الأصلي:**
```cpp
#ifdef __PX4_NUTTX
	int32_t hitl_val = 0;
	param_get(param_find("SYS_HITL"), &hitl_val);

	if (hitl_val == 1) {
		_vehicle_attitude_sub       = uORB::Subscription{ORB_ID(vehicle_attitude_groundtruth)};
		// ... باقي subscriptions لـ groundtruth
	}
#endif
```

**الكود الجديد:**
```cpp
	// HITL: works on all platforms (Android, NuttX, POSIX)
	int32_t hitl_val = 0;
	param_get(param_find("SYS_HITL"), &hitl_val);

	if (hitl_val == 1) {
		_vehicle_attitude_sub       = uORB::Subscription{ORB_ID(vehicle_attitude_groundtruth)};
		// ... باقي subscriptions لـ groundtruth (نفس الكود الأصلي)
	}
```

**ملخص:** إزالة `#ifdef __PX4_NUTTX` و `#endif` فقط. الكود الداخلي يبقى كما هو.

### التحقق
- [x] لا يوجد `#ifdef __PX4_NUTTX` في RocketGNC.cpp
- [x] كود HITL موجود ويعمل على كل المنصات

---

## الخطوة 5: أتمتة توليد uORB Topics

### الهدف
توليد ملفات uORB تلقائياً من ملفات `.msg`.

### المتطلبات
```bash
pip install empy==3.3.4 pyros-genmsg pyyaml
```

### الآلية
إضافة أوامر CMake في `CMakeLists.txt` تشغّل السكربت Python:

**السكربت:** `${PX4_ROOT}/Tools/msg/px_generate_uorb_topic_files.py`
**المدخلات:** كل ملفات `${PX4_ROOT}/msg/*.msg`
**عدد الملفات المولّدة:** 410 (204 header + 205 source + uORBTopics.hpp)

---

## الخطوة 6: أتمتة توليد الملفات الأخرى (generated/)

### الهدف
توليد كل ملفات مجلد `generated/` تلقائياً.

### الملفات
- FlightTasks_generated.cpp / .hpp
- build_git_version.h
- px4_parameters.hpp و generated_module_params.c
- events_generated.h
- output_functions.hpp (mixer_module)
- apps.cpp (ملف يدوي خاص بأندرويد)

---

## الخطوة 7: أتمتة توليد MAVLink Headers

### الهدف
توليد هيدرات MAVLink من تعريفات XML.

### عدد الهيدرات المولّدة
278 ملف (development + uAvionix dialects)

---

## الخطوة 8: تحديث CMakeLists.txt

### الهدف
تعديل CMakeLists.txt ليستخدم الملفات المولّدة تلقائياً بدل المنسوخة يدوياً.

---

## الخطوة 9: حذف الملفات المولّدة القديمة

### الهدف
إزالة المجلدات الثلاثة التي لم تعد مطلوبة.

```bash
rm -rf AndroidApp/app/src/main/cpp/generated_uorb_topics
rm -rf AndroidApp/app/src/main/cpp/generated
rm -rf AndroidApp/app/src/main/cpp/mavlink_generated
```

---

## الخطوة 10: بناء واختبار

### 10.1 بناء المشروع
```bash
cd AndroidApp
./gradlew assembleDebug
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

### 10.2 التحقق من البناء
- [x] لا أخطاء compilation
- [x] APK يُنشأ بنجاح

### 10.3 اختبار على الهاتف
- [x] التطبيق يُثبّت ويفتح
- [x] PX4 يبدأ
- [x] MAVLink bridge يعمل (QGC يتصل)
- [x] RocketGNC يبدأ بدون أخطاء

---

## الخطوة 11: تنظيف نهائي

تم حذف النسخ الاحتياطية بعد التأكد من نجاح البناء.

---

## الخطوة 12: كتابة وثيقة الأتمتة

تم كتابة `HOW_TO_REPLACE_PX4.md` — دليل شامل للاستبدال المستقبلي.

---

## الخطوة 13: إضافة Airframe 22002 (طيران حقيقي)

### الهدف
دعم وضع الطيران الحقيقي بجانب SITL و HITL.

### Airframes المدعومة

| Airframe | الوضع | SYS_HITL | الحساسات | الاستخدام |
|----------|-------|----------|----------|----------|
| 22000 | SITL | 1 | محاكي خارجي (6DOF PC) | محاكاة كاملة على PC |
| 22001 | HITL | 1 (تلقائي) | محاكي خارجي (6DOF PC) + سيرفوهات حقيقية | محاكاة مع أجهزة |
| 22002 | Real Flight | 0 | حساسات الهاتف + ضبط EKF2 | طيران حقيقي |

---

## الخطوة 14: إضافة XQPOWER CAN Servo Driver

### الهدف
إضافة درايفر سيرفوهات XQPOWER عبر CAN bus (SLCAN عبر Waveshare USB_CAN_A).

### الملفات المضافة
- `PX4-Autopilot/src/drivers/xqpower_can/XqpowerCan.cpp` — درايفر ثنائي المنصة (NuttX FDCAN + Android SLCAN)
- `PX4-Autopilot/src/drivers/xqpower_can/XqpowerCan.hpp` — يشمل USB-Serial members + JNI entry point

### التسجيل
- `apps.cpp` — إضافة `xqpower_can_main`
- `CMakeLists.txt` — إضافة مسار السورس
- `px4_jni.cpp` — إضافة `setCanUsbFd()` JNI
- `PX4Bridge.kt` — إضافة `setCanUsbFd()`
- `UsbServoManager.kt` — توجيه CH340 لدرايفر CAN

### البارامترات (7 معاملات)

| المعامل | النوع | الافتراضي | الوصف |
|---------|-------|-----------|-------|
| `XQCAN_ENABLE` | int | 1 | تفعيل/تعطيل الدرايفر |
| `XQCAN_LIMIT` | float | 25.0 | حد الزاوية بالدرجات |
| `XQCAN_NODE1..4` | int | 1..4 | عناوين CAN للسيرفوهات |
| `XQCAN_REV` | int | 0 | bitmask عكس اتجاه السيرفوهات |

### البروتوكول
- **SLCAN** عبر USB-Serial (CH340 @ 2Mbps)
- **التردد:** 50 Hz لكل من TX و feedback
- **الفيدباك:** DEBUG_FLOAT_ARRAY "SRV_FB" يحتوي cmd_deg[4] + act_deg[4] + err_deg[4]

### سلسلة أوامر السيرفو (مساران متوازيان)

```
                                    +-> simulator_mavlink -> HIL_ACTUATOR_CONTROLS -> PC (6DOF)
RocketGNC -> actuator_outputs_sim --+
                                    +-> xqpower_can -> CAN bus -> XQPOWER Servos (real hardware)

RocketGNC -> vehicle_torque_setpoint -> Control Allocator -> actuator_servos -> servo_usb_output -> USB -> سيرفو (fallback)

QGC -> actuator_test -> servo_usb_output -> USB -> سيرفو (اختبار أرضي)
```

---

## الخطوة 15: إصلاح نشر أوامر الزعانف قبل الإطلاق

### الهدف
إزالة حماية `if (_launched)` من نشر `actuator_outputs_sim` حتى تبدأ أوامر الزعانف من اللحظة الأولى.

### السبب
في HITL، التأخير في كشف الإطلاق يعني أن الصاروخ يطير بدون تحكم لأول ثانية.

### التعديل
في `RocketGNC.cpp`:
```cpp
// قبل:
if (_launched) {
    actuator_outputs_s ao{};
    // ... نشر أوامر الزعانف
}

// بعد:
// Always publish (even before launch) so the HITL bridge receives
// fin commands from the first moment of flight.
{
    actuator_outputs_s ao{};
    ao.timestamp = now;
    ao.noutputs  = 4;
    ao.output[0] = fin[0];
    ao.output[1] = fin[1];
    ao.output[2] = fin[2];
    ao.output[3] = fin[3];
    _actuator_outputs_sim_pub.publish(ao);
}
```

### النتيجة
أوامر الزعانف تبدأ من t=0.4s بدلاً من t=2s.

### الملفات المعدّلة
- `PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.cpp` (نسخة SITL)
- `AndroidApp/.../PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.cpp` (نسخة Android)

---

## الخطوة 16: تحديث servo_usb_output (حماية arm + diagnostics)

### الهدف
إضافة حماية التسليح لمنع السيرفوهات من التحرك بدون تسليح.

### التعديلات
- إضافة قراءة `actuator_armed` topic
- أوامر التحكم (Priority 1 + 2) تُرسل **فقط عند التسليح**
- اختبار السيرفو (Priority 3: actuator_test) يعمل **بدون تسليح**
- عند نزع التسليح → الزعانف تُصفَّر تلقائياً (0 درجة)
- إضافة diagnostic logging كل ثانية

### نظام الأولوية في servo_usb_output

| الأولوية | المصدر | الشرط | الاستخدام |
|----------|--------|-------|----------|
| 1 | `actuator_servos` (Control Allocator) | مسلّح فقط | تحكم طبيعي |
| 2 | `actuator_outputs_sim` (RocketGNC) | مسلّح فقط | تحكم مباشر |
| 3 | `actuator_test` (QGC) | دائماً | اختبار أرضي |

---

## الخطوة 17: تسجيل 28 أمر MAVLink Console

### الهدف
تمكين كل أوامر MAVLink Console المطلوبة للتشغيل والتشخيص.

### الملف
`apps.cpp` + `platforms/posix/apps.cpp` (نسخة مطابقة)

### الأوامر المسجلة (28 أمر)

**Modules (19):**
`ekf2`, `commander`, `sensors`, `navigator`, `mavlink`, `logger`, `rocket_gnc`, `xqpower_can`, `control_allocator`, `land_detector`, `flight_mode_manager`, `manual_control`, `mc_att_control`, `mc_pos_control`, `mc_rate_control`, `fw_att_control`, `fw_rate_control`, `dataman`, `load_mon`

**System Commands (9):**
`param`, `uorb`, `ver`, `top`, `perf`, `listener`, `actuator_test`, `reboot`

### الملفات في CMakeLists.txt
```cmake
# ===== System commands (MAVLink console) =====
list(APPEND PX4_SOURCES
    ${PX4_ROOT}/src/systemcmds/actuator_test/actuator_test.cpp
    ${PX4_ROOT}/src/systemcmds/param/param.cpp
    ${PX4_ROOT}/src/systemcmds/ver/ver.cpp
    ${PX4_ROOT}/src/systemcmds/top/top.cpp
    ${PX4_ROOT}/src/systemcmds/perf/perf.cpp
    ${PX4_ROOT}/src/systemcmds/topic_listener/listener_main.cpp
    ${PX4_ROOT}/src/systemcmds/reboot/reboot.cpp
)
```

---

## الخطوة 18: إصلاح كشف الاصطدام بالأرض في HITL

### الهدف
إيقاف المحاكاة عند اصطدام الصاروخ بالأرض حتى لو لم يصل إلى عتبة الإطلاق.

### السبب
العتبة الأصلية (`liftoff_threshold=10m`) تمنع كشف الاصطدام إذا تحطم الصاروخ قبل الوصول لـ 10م.

### التعديل
في `rocket_6dof_sim.py`: إضافة كشف احتياطي — إذا كان الصاروخ أعمق من 5م تحت الأرض بعد t>1s → إيقاف فوري.

### النتيجة
المحاكاة تتوقف عند ~11s بدلاً من 60s.

---

## ملاحظات تقنية مهمة

### rocket_gnc_params.c
ملف `rocket_gnc_params.c` **لا يُترجم** في بيئة Android. البارامترات الخاصة بـ RocketGNC موجودة مسبقاً في `generated/parameters/px4_parameters.hpp`. محاولة ترجمة هذا الملف مع `set_source_files_properties` لتعطيل ماكرو `PARAM_DEFINE_FLOAT` تفشل على Android NDK clang.

### اتصال المحطة الأرضية
```
داخل الهاتف:
  PX4 modules <-> MAVLink UDP (127.0.0.1:14550 <-> 14551)

للاتصال عبر USB/ADB:
  QGC -> localhost:5760 -> [adb forward] -> TCP bridge (5760) <-> UDP (14550)

للاتصال عبر Ethernet:
  MAVLink instance 2 -> UDP مباشر إلى IP الكمبيوتر:14550 -> QGC
```

### إضافة module جديد
1. أضف مسار السورس في `CMakeLists.txt` ضمن `PX4_SOURCES`
2. أضف مسار الـ include إذا لزم
3. سجّل الـ module في `apps.cpp` (extern + apps map)
4. انسخ `apps.cpp` لـ `platforms/posix/apps.cpp`
5. إذا كان الموديول يحتاج JNI (مثل xqpower_can)، أضف الربط في `px4_jni.cpp`
6. أعد البناء

---

## ملخص المتطلبات الكاملة

### برامج مطلوبة
- Python 3.8+
- Android Studio + NDK 29
- CMake 4.1.2

### حزم Python
```bash
pip install empy==3.3.4 pyros-genmsg pyyaml jsonschema jinja2 pymavlink future lxml
```

### أوامر البناء والرفع
```bash
cd AndroidApp
./gradlew assembleDebug
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

---

## نتائج اختبار HITL (2026-03-28)

### التشغيل 15:59 (بعد كل الإصلاحات)
- **الارتفاع الأقصى:** 94.5 متر
- **مدة الطيران:** 11.4 ثانية (المحاكاة توقفت تلقائياً عند الاصطدام)
- **أوامر الزعانف:** تبدأ من t=0.4s (تحسن كبير — كانت t=2s سابقاً)
- **دقة التتبع:** خطأ < 2 درجة في أول 3 ثوانٍ، يزداد بعد t=7s (مرحلة الهبوط)
