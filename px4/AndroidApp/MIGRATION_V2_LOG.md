# سجل تنفيذ تحديث RocketGNC v2

> **التاريخ:** 2026-03-28
> **المصدر:** `/home/sajad/Desktop/px./px/4/px/PX4-Autopilot/`

---

## الخطوة 1: نسخة احتياطية
- **الحالة:** تم بنجاح
- **المسار:** `AndroidApp/backup_v2_2026-03-28/`
- **الملفات:**
  - `RocketGNC.cpp` (32,426 bytes, 920 سطر)
  - `RocketGNC.hpp` (9,200 bytes, 227 سطر)
  - `rocket_gnc_params.c` (4,970 bytes)
  - `22001_m130_rocket_hitl` (2,113 bytes)
  - `22002_m130_rocket_real` (3,092 bytes)

---

## الخطوة 2: استبدال RocketGNC.cpp
- **الحالة:** تم بنجاح
- **من:** 920 سطر → **إلى:** 960 سطر
- **الإجراء:** نسخ الملف الجديد (962 سطر) + إزالة `#ifdef __PX4_NUTTX` و `#endif` (سطرين) + إضافة تعليق
- **التعديل الوحيد:** سطر 73 — `#ifdef __PX4_NUTTX` → `// HITL: works on all platforms (Android, NuttX, POSIX)` وحذف `#endif` بعد السطر 81
- **التحقق:** `grep __PX4_NUTTX` → لا نتائج (OK)
- **التحقق:** `grep SYS_HITL` → سطر 75 (OK)
- **التحقق:** `diff` مع الجديد → فرقين فقط (التعليق + إزالة ifdef/endif)
- **ما تغير من الكود القديم:**
  - Gains: K_w_roll=0.0001, K_p_integ_roll=0.005, K_i_integ_roll=0.00 (مطابقة Python)
  - Roll_sat: 7.0 → 3.0
  - جديد: Alpha Protection عند 12 درجة
  - جديد: IMU Live Test (GND_TEST==2)
  - جديد: تصفير كل الفلاتر في rail time
  - محذوف: `_hitl_mode` وكشف الإطلاق بالسرعة
  - محذوف: كود velocity-based launch detection (11 سطر)

---

## الخطوة 3: استبدال RocketGNC.hpp
- **الحالة:** تم بنجاح
- **من:** 227 سطر → **إلى:** 225 سطر
- **الإجراء:** نسخ كما هو بدون تعديل
- **التحقق:** `diff` مع الجديد → IDENTICAL
- **ما تغير:** حذف `bool _hitl_mode{false}` + سطر فارغ

---

## الخطوة 4: استبدال rocket_gnc_params.c
- **الحالة:** تم بنجاح
- **الإجراء:** نسخ كما هو بدون تعديل
- **التحقق:** `diff` مع الجديد → IDENTICAL
- **ما تغير:** ROCKET_GND_TEST: `@boolean` → `@min 0 @max 2` (3 أوضاع)
- **ملاحظة:** لا يُترجم على Android — مرجع توثيقي فقط

---

## الخطوة 5: استبدال airframe 22001 (HITL)
- **الحالة:** تم بنجاح
- **الإجراء:** نسخ كما هو بدون تعديل
- **التحقق:** `diff` مع الجديد → IDENTICAL
- **ما تغير:** إضافة `pwm_out_sim stop` (4 أسطر) لمنع تعارض أوامر الزعانف

---

## الخطوة 6: استبدال airframe 22002 (طيران حقيقي)
- **الحالة:** تم بنجاح
- **الإجراء:** نسخ كما هو بدون تعديل
- **التحقق:** `diff` مع الجديد → IDENTICAL
- **ما تغير:**
  - `param set` → `param set-default` (يسمح بتغيير القيم من QGC)
  - وصف GND_TEST: `(0=flight, 1=ground test, 2=IMU live test)`

---

## إضافة: تسجيل pwm_out_sim
- **الحالة:** تم بنجاح
- **السبب:** airframe 22001 يحتوي أمر `pwm_out_sim stop` — يجب أن يكون الأمر متاحاً
- **الملفات المعدّلة:**
  - `apps.cpp` — إضافة `extern + apps["pwm_out_sim"]`
  - `platforms/posix/apps.cpp` — نفس التعديل (نسخة مطابقة)
  - `CMakeLists.txt` — إضافة `PWMSim.cpp` في PX4_SOURCES
- **عدد الأوامر الآن:** 20 module + 8 system commands = 29 أمر

---

## الخطوة 7: بناء واختبار
- **الحالة:** تم بنجاح
- **التاريخ:** 2026-03-28
- **البناء:** BUILD SUCCESSFUL in 11s (لا أخطاء، تحذيرات MAVLINK_UDP فقط)
- **التثبيت:** `adb install -r` → Success

---

## ما لم يُعدّل (يبقى كما هو)

| الملف | السبب |
|-------|-------|
| XqpowerCan.cpp (1555 سطر) | نسختنا تحتوي Android SLCAN — الجديد NuttX فقط (984 سطر) |
| XqpowerCan.hpp | نفس السبب |
| px4_log.cpp | تعديل Android (logcat) |
| WorkQueueManager.cpp | تعديل Android (SCHED_OTHER) |
| shutdown.cpp | تعديل Android (JNI restart) |
| tasks.cpp | تعديل Android (pthread_kill) |
| server.cpp | تعديل Android (pthread_kill) |
| apps.cpp | 19 module + 8 system commands مسجلة |
| CMakeLists.txt | كل المسارات موجودة |
| px4_parameters.hpp | كل البارامترات موجودة |
| px4_jni.cpp | JNI bridge كامل |
| servo_usb_output.cpp | arm-gating + diagnostics |
