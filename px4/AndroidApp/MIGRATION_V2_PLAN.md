# خطة تحديث RocketGNC v2 + Airframes

> **التاريخ:** 2026-03-28
> **الهدف:** استبدال موديول rocket_gnc وملفات airframe بالنسخة الجديدة من مجلد `4/px/PX4-Autopilot`
> **المصدر:** `/home/sajad/Desktop/px./px/4/px/PX4-Autopilot/`
> **الهدف:** `/home/sajad/Desktop/px./px/AndroidApp/app/src/main/cpp/PX4-Autopilot/`

---

## ملخص التغييرات

| # | الخطوة | الملفات | الإجراء |
|---|--------|---------|---------|
| 1 | نسخة احتياطية | 5 ملفات | نسخ الملفات الحالية قبل التعديل |
| 2 | استبدال RocketGNC.cpp | 1 ملف | نسخ من الجديد + إزالة `#ifdef __PX4_NUTTX` |
| 3 | استبدال RocketGNC.hpp | 1 ملف | نسخ من الجديد كما هو |
| 4 | استبدال rocket_gnc_params.c | 1 ملف | نسخ من الجديد كما هو |
| 5 | استبدال airframe 22001 | 1 ملف | نسخ من الجديد كما هو |
| 6 | استبدال airframe 22002 | 1 ملف | نسخ من الجديد كما هو |
| 7 | بناء واختبار | - | `./gradlew assembleDebug` |

---

## التفاصيل

### الخطوة 1: نسخة احتياطية

نسخ الملفات الخمسة الحالية إلى مجلد backup:

```
AndroidApp/app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.cpp
AndroidApp/app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/RocketGNC.hpp
AndroidApp/app/src/main/cpp/PX4-Autopilot/src/modules/rocket_gnc/rocket_gnc_params.c
AndroidApp/app/src/main/cpp/PX4-Autopilot/ROMFS/px4fmu_common/init.d/airframes/22001_m130_rocket_hitl
AndroidApp/app/src/main/cpp/PX4-Autopilot/ROMFS/px4fmu_common/init.d/airframes/22002_m130_rocket_real
```

### الخطوة 2: استبدال RocketGNC.cpp

- نسخ الملف الجديد (962 سطر) بدل القديم (920 سطر)
- تعديل وحيد: إزالة `#ifdef __PX4_NUTTX` (سطر ~73) و `#endif` (سطر ~86)
- النتيجة: كود HITL يعمل على كل المنصات (Android + NuttX + POSIX)
- **ما يتغير:** gains جديدة، alpha protection، IMU live test، تصفير فلاتر rail time
- **ما لا يتغير:** كل الباقي كما في الجديد

### الخطوة 3: استبدال RocketGNC.hpp

- نسخ كما هو بدون أي تعديل
- الملف الجديد لا يحتوي `_hitl_mode` (225 سطر بدل 227)

### الخطوة 4: استبدال rocket_gnc_params.c

- نسخ كما هو بدون أي تعديل
- التغيير: ROCKET_GND_TEST أصبح 3 أوضاع (0=flight, 1=ground test, 2=IMU live test)
- ملاحظة: هذا الملف لا يُترجم على Android — يُستخدم كمرجع توثيقي فقط

### الخطوة 5: استبدال airframe 22001

- نسخ كما هو بدون أي تعديل
- التغيير: إضافة `pwm_out_sim stop` لمنع تعارض أوامر الزعانف

### الخطوة 6: استبدال airframe 22002

- نسخ كما هو بدون أي تعديل
- التغيير: `param set` → `param set-default` + وصف GND_TEST ثلاثي

### الخطوة 7: بناء واختبار

```bash
cd AndroidApp
./gradlew assembleDebug
```

---

## ما لا نلمسه

| الملف | السبب |
|-------|-------|
| XqpowerCan.cpp/hpp | نسختنا أكمل (فيها Android SLCAN) |
| 5 ملفات platform | تعديلات Android ضرورية |
| apps.cpp | مسجل فيه كل الموديولات |
| CMakeLists.txt | يحتوي كل المسارات |
| px4_parameters.hpp | يحتوي كل البارامترات |
| px4_jni.cpp | يحتوي JNI bridge |
| servo_usb_output.cpp | يحتوي arm-gating |
