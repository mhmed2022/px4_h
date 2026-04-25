# شرح تفصيلي: كيف تصل البيانات إلى `actuator_outputs` في نظام PX4

---

## 📋 مقدمة

هذا الشرح يتناول **المسار الكامل** للبيانات داخل نظام **PX4** - من وحدة التحكم (Joystick/Android) حتى تصل إلى **منفذ الإخراج** الذي يُرسل إلى Arduino عبر USB Serial.

---

## 🗺️ خريطة المسار الكامل

```
┌──────────────────────────────────────────────────────────────────────┐
│                     نظام PX4 الكامل                                  │
│                                                                      │
│  ┌─────────────┐      ┌──────────────┐      ┌─────────────────────┐ │
│  │ وحدة التحكم │      │  Mixer        │      │  Arduino Bridge    │ │
│  │ (Joystick/  │─────▶│  (خلاط       │─────▶│  (جسر USB Serial)  │ │
│  │  Android)   │      │   التوزيع)   │      │                    │ │
│  └─────────────┘      └──────────────┘      └─────────────────────┘ │
│       │                      │                        │             │
│       ▼                      ▼                        ▼             │
│  ┌─────────────┐      ┌──────────────┐      ┌─────────────────────┐ │
│  │ Vehicle     │      │ Actuator     │      │  ESCs → Motors     │ │
│  │ Commands    │─────▶│ Outputs      │─────▶│  (المحركات)        │ │
│  │ (الأوامر)   │      │ (المخرجات)   │      │                    │ │
│  └─────────────┘      └──────────────┘      └─────────────────────┘ │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 🔍 الجزء الأول: وحدة التحكم (Input Source)

### ما هي وحدة التحكم؟

وحدة التحكم هي **المصدر الأول** للأوامر. يمكن أن تكون:

| نوع وحدة التحكم | الوصف |
|-----------------|-------|
| **Joystick/RC Receiver** | جهاز تحكم لاسلكي تقليدي متصل بمستقبل RC |
| **Android App (PX4)** | تطبيق يعمل على هاتف Android متصل عبر USB |
| **Ground Station** | محطة أرضية ترسل أوامر عبر MAVLink |
| **Mission Planner** | برنامج تخطيط المهام |

### كيف تعمل وحدة التحكم؟

#### 1️⃣ Joystick/RC

```
عصا التحكم (Joystick)
        ↓
إشارة RC (Radio Control)
        ↓
مستقبل RC (Receiver)
        ↓
rc_input → topic: input_rc
```

**التفصيل:**

| الخطوة | الوصف |
|--------|-------|
| 1 | المستخدم يحرك عصا التحكم (الارتفاع، الانحراف، الميلان، الدوران) |
| 2 | جهاز الإرسال (Transmitter) يُرسل الإشارة لاسلكياً |
| 3 | المستقبل (Receiver) المتصل بالطائرة يستقبل الإشارة |
| 4 | PX4 يقرأ البيانات عبر topic اسمه `input_rc` |

#### 2️⃣ Android App

```
تطبيق Android (PX4)
        ↓
يُولد قيم PWM للمحركات
        ↓
USB Serial → Arduino
        ↓
Arduino يُطبق القيم على ESCs
```

**التفصيل:**

| الخطوة | الوصف |
|--------|-------|
| 1 | المستخدم يتحكم عبر واجهة التطبيق |
| 2 | التطبيق يحسب قيم PWM المطلوبة لكل محرك |
| 3 | البيانات تُرسل عبر USB Serial إلى Arduino |
| 4 | Arduino يستقبل ويطبق القيم مباشرة |

---

## 🔍 الجزء الثاني: Vehicle Commands (الأوامر)

### ما هي Vehicle Commands؟

هي **الأوامر المُوحدة** التي تُرسل من وحدة التحكم إلى نظام PX4.

### Topics المستخدمة

| Topic | النوع | الوصف |
|-------|-------|-------|
| `vehicle_command` | `vehicle_command_s` | أوامر التحكم (الإقلاع، الهبوط، RTL) |
| `vehicle_local_position_setpoint` | `vehicle_local_position_setpoint_s` | نقاط التحكم بالموقع المحلي |
| `manual_control_setpoint` | `manual_control_setpoint_s` | قيم التحكم اليدوي (Joystick) |

### كيف تصل البيانات؟

```
Manual Control (Joystick/Android)
        ↓
manual_control_setpoint
        ↓
┌────────────────────────────────────────────┐
│ قيم التحكم الأربعة الرئيسية:                │
│                                            │
│ roll    = -1.0 إلى +1.0  (الميلان)        │
│ pitch   = -1.0 إلى +1.0  (الانحراف)       │
│ yaw     = -1.0 إلى +1.0  (الدوران)        │
│ thrust  =  0.0 إلى +1.0  (الارتفاع)       │
└────────────────────────────────────────────┘
        ↓
Multicopter Position Control
        ↓
Vehicle Rates Setpoint
```

**مثال على القيم:**

```
roll    = 0.0   (لا ميلان)
pitch   = 0.5   (ميلان للأمام 50%)
yaw     = 0.0   (لا دوران)
thrust  = 0.7   (ارتفاع 70%)
```

---

## 🔍 الجزء الثالث: المتحكم (Controller)

### ما هو المتحكم؟

المتحكم هو **العقل** الذي يحسب كيف يجب أن تتحرك المحركات لتحقيق الأمر المطلوب.

### أنواع المتحكمات في PX4

| النوع | الوصف |
|-------|-------|
| **MC_POSITION_CONTROL** | تحكم بالموقع (للطائرات الرباعية) |
| **MC_ATTITUDE_CONTROL** | تحكم بالاتجاه (Attitude) |
| **MC_RATE_CONTROL** | تحكم بسرعة الدوران |

### كيف يعمل المتحكم؟

#### مثال: تحكم بالارتفاع

```
الهدف: ارتفاع 10 متر
        ↓
الارتفاع الحالي: 5 متر
        ↓
الخطأ = 10 - 5 = 5 متر
        ↓
PID Controller يحسب thrust المطلوب
        ↓
thrust = 0.7 (70% قوة)
        ↓
يُرسل إلى Mixer
```

**التفصيل الرياضي:**

```
error = setpoint - current_value
P_term = Kp * error
I_term = Ki * ∫error dt
D_term = Kd * d(error)/dt

output = P_term + I_term + D_term
```

---

## 🔍 الجزء الرابع: Mixer (الخلاط)

### ما هو Mixer؟

**Mixer = خالط/موزع** - هو الجزء الذي يقرر **كيف توزع قوة المحركات** لتحقيق الأمر المطلوب.

### لماذا نحتاج Mixer؟

| السيناريو | بدون Mixer | مع Mixer |
|-----------|-----------|----------|
| دوران يمين | المحركات كلها بنفس السرعة | المحركات اليمنى أبطأ، اليسرى أسرع |
| ميلان أمام | كل المحركات بنفس القوة | المحركات الأمامية أبطأ |

### كيف يعمل Mixer؟

#### 1️⃣ تحديد نوع الطائرة

PX4 يدعم أنواع مختلفة:

| النوع | المحركات | الوصف |
|-------|----------|-------|
| **QUADROTOR_X** | 4 | طائرة رباعية (شكل X) |
| **QUADROTOR_+** | 4 | طائرة رباعية (شكل +) |
| **HEXAROTOR** | 6 | طائرة سداسية |
| **OCTOROTOR** | 8 | طائرة ثمانية |
| **FIX_WING** | 1-2 | طائرة ثابتة الجناحين |

#### 2️⃣ توزيع القيم على المحركات

**مثال لـ QUADROTOR_X:**

```
المحركات:
   M0 (أمامي يمين - CW)     M1 (خلفي يسار - CCW)
   
   M2 (أمامي يسار - CCW)    M3 (خلفي يمين - CW)
```

**صيغة التوزيع:**

```
motor_0 = thrust - roll + pitch - yaw
motor_1 = thrust + roll - pitch - yaw
motor_2 = thrust + roll + pitch + yaw
motor_3 = thrust - roll - pitch + yaw
```

**مثال عملي:**

```
القيم المدخلة:
thrust = 0.7
roll = 0.0
pitch = 0.2
yaw = 0.0

الحساب:
motor_0 = 0.7 - 0.0 + 0.2 - 0.0 = 0.9  (90%)
motor_1 = 0.7 + 0.0 - 0.2 - 0.0 = 0.5  (50%)
motor_2 = 0.7 + 0.0 + 0.2 + 0.0 = 0.9  (90%)
motor_3 = 0.7 - 0.0 - 0.2 + 0.0 = 0.5  (50%)
```

#### 3️⃣ ملف Mixer

ملف Mixer يحدد كيف توزع القيم. مثال:

```
# Multirotor Mixer Definition
M: Q
O:      10000   10000   10000    500
S:  0   0  10000  10000      0  10000
S:  0   1  10000  10000      0  10000
S:  0   2  10000  10000      0  10000
S:  0   3  10000  10000      0  10000
```

| الرمز | المعنى |
|-------|--------|
| `M: Q` | نوع Multirotor |
| `O:` | إزاحة (Offset) |
| `S:` | معامل كل محرك (Scale factor) |

---

## 🔍 الجزء الخامس: Actuator Outputs (المخرجات)

### ما هو `actuator_outputs`؟

هو **Topic نهائي** في نظام PX4 يحتوي على **قيم PWM الفعلية** التي تُرسل لكل محرك.

### تعريف Topic

```
Topic: actuator_outputs
Type:  actuator_outputs_s
```

### بنية البيانات

```cpp
struct actuator_outputs_s {
    uint64_t timestamp;           // وقت الإنشاء (microseconds)
    uint32_t noutputs;            // عدد المخارج (مثلاً 4)
    float output[MAX_ACTUATOR_OUTPUTS];  // قيم الخرج (-1.0 إلى 1.0)
    // أو
    uint16_t pwm[MAX_ACTUATOR_OUTPUTS];  // قيم PWM (900-2100)
};
```

**التفصيل:**

| الحقل | النوع | الوصف |
|-------|-------|-------|
| `timestamp` | `uint64_t` | وقت الإنشاء بالميكروثانية |
| `noutputs` | `uint32_t` | عدد المخارج الفعلية (مثلاً 4) |
| `output[i]` | `float` | قيمة الخرج من -1.0 إلى +1.0 |
| `pwm[i]` | `uint16_t` | قيمة PWM بالميكروثانية (900-2100) |

### كيف تُنشأ القيم؟

```
Mixer Output (-1.0 إلى +1.0)
        ↓
تحويل إلى PWM
        ↓
PWM = 1500 + (output * 500)
        ↓
┌────────────────────────────────┐
│ output = -1.0  →  PWM = 1000  │
│ output =  0.0  →  PWM = 1500  │
│ output = +1.0  →  PWM = 2000  │
└────────────────────────────────┘
        ↓
actuator_outputs topic
```

**صيغة التحويل:**

```
PWM = center + (output * range)

حيث:
center = 1500 μs (القيمة المتوسطة)
range = 500 μs (نطاق التغير)

مثال:
output = 0.5
PWM = 1500 + (0.5 * 500) = 1750 μs
```

### كيف تُنشأ البيانات في الكود؟

```cpp
// في ملف: src/modules/mc_att_control/MulticopterAttitudeControl.cpp

// 1. المتحكم يحسب rates المطلوبة
VehicleRatesSetpoint rates_sp;

// 2. Mixer يوزع القيم على المحركات
_actuator_controls.update();

// 3. Mixer output يُحوَّل إلى PWM
for (int i = 0; i < _params.motor_count; i++) {
    float output = mixer->mix(i, actuator_controls);
    
    // تحويل إلى PWM
    uint16_t pwm = 1500 + (output * 500);
    
    // تخزين في actuator_outputs
    actuator_outputs.output[i] = output;
    actuator_outputs.pwm[i] = pwm;
}

// 4. نشر topic
_actuator_outputs_pub.publish(actuator_outputs);
```

---

## 🔍 الجزء السادس: Arduino Bridge (الجسر)

### كيف تستقبل Arduino البيانات؟

#### 1️⃣ PX4 يُنشئ `actuator_outputs`

```
PX4 System
        ↓
Mixer يوزع القيم
        ↓
actuator_outputs = {
    pwm[0] = 1500,
    pwm[1] = 1500,
    pwm[2] = 1500,
    pwm[3] = 1500
}
```

#### 2️⃣ تطبيق Android يقرأ البيانات

```
Android App (PX4)
        ↓
يقرأ actuator_outputs topic
        ↓
يُشكل البروتوكول:
[0xAA][0x55][4][PWM0_H][PWM0_L][PWM1_H][PWM1_L]...
        ↓
يُرسل عبر USB Serial
```

#### 3️⃣ Arduino يستقبل

```
Arduino (هذا الكود)
        ↓
Serial.read() → يستقبل بايت بايت
        ↓
يتعرف على الإطار (Frame)
        ↓
يتحقق من Checksum
        ↓
يستخرج قيم PWM
        ↓
esc[i].writeMicroseconds(pwm)
```

---

## 🔍 الجزء السابع: ESCs والمحركات

### ما هو ESC؟

**ESC = Electronic Speed Controller** - جهاز يتحكم بسرعة المحرك بناءً على إشارة PWM.

### كيف يعمل ESC؟

```
إشارة PWM من Arduino (900-2100 μs)
        ↓
ESC يقرأ العرض (Pulse Width)
        ↓
يُحول إلى سرعة
        ↓
المحرك يدور بالسرعة المطلوبة
```

| قيمة PWM | المعنى | سرعة المحرك |
|----------|--------|-------------|
| 900 | إيقاف قوي | 0% |
| 1000 | إيقاف | 0% |
| 1100 | بطيء جداً | ~10% |
| 1500 | متوسط | ~50% |
| 1900 | سريع | ~90% |
| 2000 | أقصى سرعة | 100% |
| 2100 | أقصى حد | 100%+ |

---

## 🗺️ المسار الكامل من البداية للنهاية

```
┌─────────────────────────────────────────────────────────────────────┐
│                        المسار الكامل للبيانات                       │
└─────────────────────────────────────────────────────────────────────┘

1️⃣ المستخدم يتحكم عبر Android App
        ↓
   [Joystick في التطبيق]
        ↓
   thrust = 0.7, roll = 0, pitch = 0.2, yaw = 0

2️⃣ PX4 يستقبل الأوامر
        ↓
   [manual_control_setpoint topic]
        ↓
   Roll = 0.0, Pitch = 0.2, Yaw = 0.0, Thrust = 0.7

3️⃣ المتحكم يحسب المطلوب
        ↓
   [MC_ATTITUDE_CONTROL]
        ↓
   PID Controller → VehicleRatesSetpoint

4️⃣ Mixer يوزع القيم
        ↓
   [Mixer: QUADROTOR_X]
        ↓
   Motor 0: 0.9
   Motor 1: 0.5
   Motor 2: 0.9
   Motor 3: 0.5

5️⃣ إنشاء actuator_outputs
        ↓
   [Actuator Outputs Module]
        ↓
   PWM[0] = 1950 μs
   PWM[1] = 1750 μs
   PWM[2] = 1950 μs
   PWM[3] = 1750 μs

6️⃣ Android App يقرأ البيانات
        ↓
   [يقرأ actuator_outputs]
        ↓
   يُشكل البروتوكول:
   [0xAA][0x55][4][0x07][0x9E][0x06][0xDE]...

7️⃣ إرسال عبر USB Serial
        ↓
   [USB Cable]
        ↓
   البيانات تصل إلى Arduino

8️⃣ Arduino يستقبل ويُطبق
        ↓
   [executeCommand()]
        ↓
   esc[0].writeMicroseconds(1950)
   esc[1].writeMicroseconds(1750)
   esc[2].writeMicroseconds(1950)
   esc[3].writeMicroseconds(1750)

9️⃣ ESCs تتحكم بالمحركات
        ↓
   [ESC → Motor]
        ↓
   Motor 0: ~90% سرعة
   Motor 1: ~50% سرعة
   Motor 2: ~90% سرعة
   Motor 3: ~50% سرعة

🔟 الطائرة تتحرك
        ↓
   الميلان للأمام (Pitch)
```

---

## 🔍 الجزء الثامن: تفاصيل Topics في PX4

### قائمة Topics الكاملة

| Topic | النوع | الاتجاه | الوصف |
|-------|-------|---------|-------|
| `input_rc` | `input_rc_s` | RC → PX4 | بيانات RC الخام |
| `manual_control_setpoint` | `manual_control_setpoint_s` | Input → Ctrl | أوامر التحكم اليدوي |
| `vehicle_rates_setpoint` | `vehicle_rates_setpoint_s` | Ctrl → Mixer | معدلات الدوران المطلوبة |
| `actuator_controls` | `actuator_controls_s` | Ctrl → Mixer | عناصر التحكم بالمشغلات |
| `actuator_outputs` | `actuator_outputs_s` | Mixer → Output | قيم PWM النهائية |

### مثال على `actuator_outputs`

```cpp
// مثال فعلي لبيانات actuator_outputs
actuator_outputs_s outputs = {
    .timestamp = 1234567890,           // وقت الإنشاء
    .noutputs = 4,                      // 4 محركات
    .output = {0.5f, 0.3f, 0.5f, 0.3f}, // قيم الخرج
    .pwm = {1750, 1650, 1750, 1650}     // قيم PWM
};
```

---

## 🔍 الجزء التاسع: الأوامر (Commands)

### أنواع الأوامر

| الأمر | الوصف |
|-------|-------|
| `VEHICLE_CMD_ARM` | تسليح المحركات |
| `VEHICLE_CMD_DISARM` | إيقاف المحركات |
| `VEHICLE_CMD_TAKEOFF` | إقلاع |
| `VEHICLE_CMD_LAND` | هبوط |
| `VEHICLE_CMD_DO_SET_MODE` | تغيير الوضع |

### كيف تُرسل الأوامر؟

```
Android App → Vehicle Command
        ↓
vehicle_command topic
        ↓
Commander Module
        ↓
تنفيذ الأمر (تسليح/إقلاع/هبوط)
        ↓
تغيير حالة النظام
```

---

## 📊 جدول القيم الشاملة

| المرحلة | نوع البيانات | النطاق | الوصف |
|---------|-------------|--------|-------|
| Manual Control | float | -1.0 إلى +1.0 | قيم التحكم |
| Actuator Controls | float | -1.0 إلى +1.0 | عناصر التحكم |
| Mixer Output | float | -1.0 إلى +1.0 | خرج الخلاط |
| PWM | uint16_t | 900-2100 | إشارة ESC |
| Arduino Serial | byte | 0-255 | بيانات USB |

---

## 📖 ملخص الأجزاء الرئيسية

| الجزء | الوصف | الوظيفة |
|-------|-------|---------|
| **1. وحدة التحكم** | Joystick/Android | إدخال الأوامر |
| **2. Vehicle Commands** | أوامر موحدة | ترجمة الأوامر |
| **3. المتحكم** | PID Controller | حساب المطلوب |
| **4. Mixer** | خالط القيم | توزيع على المحركات |
| **5. Actuator Outputs** | قيم PWM النهائية | الخرج الفعلي |
| **6. Arduino Bridge** | استقبال USB | تحويل البيانات |
| **7. ESCs** | متحكمات السرعة | التحكم بالمحركات |
| **8. المحركات** | محركات Brushless | الحركة الفعلية |

---

## ⚠️ ملاحظات هامة

1. **`actuator_outputs` ليس نهاية المسار** - هو يُنشأ في PX4 لكن البيانات الفعلية تصل إلى Arduino عبر بروتوكول مختلف.

2. **Android App يعمل كوسيط** - يقرأ `actuator_outputs` من PX4 ثم يُرسلها عبر USB إلى Arduino.

3. **البروتوكول مُخصص** - `[0xAA][0x55]...` هو بروتوكول مُصمم خصيصاً وليس بروتوكول MAVLink قياسي.

4. **السرعة مهمة** - بدون `Serial.print` في Arduino لضمان السرعة القصوى.

---

## 🔗 المراجع

| المرجع | الوصف |
|--------|-------|
| [PX4 Documentation](https://docs.px4.io/) | التوثيق الرسمي |
| [PX4 Source Code](https://github.com/PX4/PX4-Autopilot) | الكود المصدري |
| [uORB Topics](https://docs.px4.io/main/en/middleware/uORB.html) | نظام Topics |
| [Mixer Documentation](https://docs.px4.io/main/en/config/actuators.html) | توثيق Mixer |

---

**نهاية الشرح**

*تم إعداد هذا الشرح لفهم المسار الكامل للبيانات في نظام PX4*
