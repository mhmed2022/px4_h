# تقرير تدفق البيانات: محاكاة SITL للصاروخ M130

## نظام محاكاة 6DOF_v2_130 مع PX4 — التوثيق الكامل

---

## 1. نظرة عامة على البنية

نظام المحاكاة SITL للصاروخ M130 يتكون من عمليتين مستقلتين تتواصلان عبر بروتوكول MAVLink HIL فوق اتصال TCP:

```
┌──────────────────────────┐         TCP:4560          ┌──────────────────────────┐
│   6DOF_v2_130 (Python)   │ ◄══════ MAVLink ════════► │     PX4 SITL (C++)       │
│   محرك الفيزياء          │       HIL Protocol        │   rocket_gnc + EKF2      │
│   (دفع + ديناميكا هوائية │                           │   (توجيه + تحكم آلي)     │
│    + نموذج كتلة)         │                           │                          │
└──────────────────────────┘                           └──────────────────────────┘
```

**المبدأ الأساسي:** البنية مطابقة تماماً لنموذج Gazebo ↔ PX4. المحاكي يعمل كمحرك فيزياء فقط (بدون GNC داخلي)، و PX4 يوفر التوجيه والتحكم عبر وحدة `rocket_gnc`.

### معلومات الصاروخ

| الخاصية | القيمة |
|---------|--------|
| الكتلة الجافة | 11.11 kg |
| كتلة الوقود | 1.63 kg |
| القطر | 130 mm |
| موقع الإطلاق | 16.457°N, 44.115°E |
| ارتفاع الإطلاق | 1200 m MSL |
| الهدف | 3000 m شمال (bearing 0°) |
| أقصى تسارع | ~15g (147 m/s²) |

---

## 2. ملفات النظام الرئيسية

### جانب المحاكي (6DOF_v2_130)

| الملف | الوصف |
|-------|-------|
| `6DOF_v2_130/rocket_6dof_sim.py` | محرك الفيزياء الرئيسي — معادلات الحركة 6DOF |
| `6DOF_v2_130/run_px4_hil.py` | مُشغّل SITL — يربط المحاكي بـ PX4 عبر TCP |
| `6DOF_v2_130/run_px4_hitl.py` | مُشغّل HITL — يربط بهاردوير حقيقي عبر USB Serial |
| `6DOF_v2_130/mavlink_bridge/px4_hitl_bridge.py` | طبقة بروتوكول MAVLink |
| `6DOF_v2_130/config/6dof_config_advanced.yaml` | إعدادات الفيزياء والتوجيه |

### جانب PX4

| الملف | الوصف |
|-------|-------|
| `ROMFS/px4fmu_common/init.d-posix/airframes/22000_m130_rocket` | إعدادات إطار SITL |
| `ROMFS/px4fmu_common/init.d/airframes/22001_m130_rocket_hitl` | إعدادات إطار HITL |
| `ROMFS/px4fmu_common/init.d/rc.rocket_defaults` | ضبط EKF2 والمعاملات للصاروخ |
| `ROMFS/px4fmu_common/init.d/rc.rocket_apps` | بدء تطبيقات الصاروخ (rocket_gnc) |
| `ROMFS/px4fmu_common/init.d-posix/rcS` | سكربت بدء POSIX/SITL الرئيسي |
| `ROMFS/px4fmu_common/init.d-posix/px4-rc.simulator` | اختيار نوع المحاكي |
| `ROMFS/px4fmu_common/init.d-posix/px4-rc.mavlinksim` | بدء simulator_mavlink |
| `src/modules/simulation/simulator_mavlink/SimulatorMavlink.cpp` | واجهة MAVLink HIL في PX4 |
| `src/modules/rocket_gnc/RocketGNC.cpp` | وحدة التوجيه والتحكم |
| `src/modules/rocket_gnc/RocketGNC.hpp` | رأسية وحدة GNC |

---

## 3. تسلسل بدء SITL بالتفصيل

### الخطوة 1: إطلاق PX4 SITL

```bash
cd PX4-Autopilot
PX4_SYS_AUTOSTART=22000 ./build/px4_sitl_default/bin/px4
```

تسلسل التنفيذ في `rcS`:

1. **قراءة متغير البيئة** (سطر 41-44): `PX4_SYS_AUTOSTART=22000`
2. **البحث عن ملف الإطار** (سطر 218-233): يبحث عن `22000_*` في `init.d-posix/airframes/` ← يجد `22000_m130_rocket`
3. **تنفيذ ملف الإطار** (سطر 233): `. "$autostart_file"` الذي بدوره:
   - ينفّذ `rc.rocket_defaults` — ضبط EKF2 والحساسات الوهمية
   - يضبط `SYS_HITL=1` — تفعيل وضع HIL
   - يضبط معاملات GNC (الهدف، التوجيه، حدود الزعانف)
4. **بدء المحاكي** (سطر 246): `. px4-rc.simulator` ← بما أن النموذج ليس gz/sih/jmavsim، يسقط في الفرع `else` الذي ينفّذ `px4-rc.mavlinksim`
5. **بدء simulator_mavlink** (px4-rc.mavlinksim سطر 18): يستمع على `TCP port 4560`
6. **بدء EKF2** (سطر 266-269): مقدّر الحالة بتردد تنبؤ 250 Hz
7. **وضع HIL** (سطر 281-284): `SYS_HITL=1` → `pwm_out_sim start -m hil`
8. **بدء تطبيقات الصاروخ** (سطر 297): `rc.vehicle_setup` → يكتشف `VEHICLE_TYPE=rocket` → `rc.rocket_apps` → `rocket_gnc start`

### الخطوة 2: إطلاق المحاكي

```bash
cd 6DOF_v2_130
python3 run_px4_hil.py
```

تسلسل التنفيذ في `run_px4_hil.py`:

1. **الاتصال بـ PX4** (سطر 515): `connect_px4()` — TCP server على المنفذ 4560، ينتظر حتى 120 ثانية
2. **تحميل محرك الفيزياء** (سطر 520-531): يقرأ `6dof_config_advanced.yaml`، يعطل وضع `long_range`، يحمّل `Rocket6DOFSimulation`
3. **تعطيل GNC الداخلي** (سطر 536-537): `sim.use_autopilot = False` و `sim.use_guidance = False` — المحاكي يصبح محرك فيزياء صرف
4. **إنشاء GazeboStylePlugin** (سطر 547-553): يهيئ الاتصال مع إحداثيات الإطلاق
5. **تسخين EKF2** (سطر 569): `plugin.warmup(elevation_deg, duration=12.0)` — يرسل بيانات ثابتة (صاروخ على المنصة) لمدة 12 ثانية بتردد 50 Hz حتى يهيئ EKF2 حالته
6. **حفظ المعاملات** (سطر 570): `plugin.param_save()`
7. **التسليح القسري** (سطر 571): `plugin.force_arm()` — ضروري لأن `simulator_mavlink` لا يمرر أوامر المشغلات إلا والمركبة مسلحة
8. **بدء المحاكاة** (سطر 586-590): `sim.simulate(duration, dt, control_function=plugin)`

---

## 4. تدفق البيانات التفصيلي

### 4.1 بيانات الحساسات: 6DOF → PX4 (100 Hz)

المحاكي يحسب حالة الصاروخ فيزيائياً في كل خطوة ثم يرسل ثلاث رسائل MAVLink:

#### رسالة HIL_SENSOR (100 Hz)

```python
# من run_px4_hil.py سطر 419-429
conn.mav.hil_sensor_send(
    t_usec,                                          # الزمن بالمايكروثانية
    float(sf_body[0]), float(sf_body[1]), float(sf_body[2]),  # تسارع IMU (FRD body) m/s²
    float(omega[0]),   float(omega[1]),   float(omega[2]),    # جايروسكوب (FRD body) rad/s
    0.2, 0.0, 0.4,                                   # مغناطيسي (Gauss) — رمزي
    _alt_to_pressure(alt_msl) * 0.01,                # ضغط مطلق (hPa) — من ISA
    0.0,                                             # ضغط تفاضلي
    alt_msl,                                         # ارتفاع ضغطي MSL (m)
    25.0,                                            # درجة حرارة (°C)
    0x1FFF,                                          # جميع الحقول صالحة
)
```

**حساب القوة النوعية (Specific Force):**
```python
# سطر 405-407
dt_acc = t - self._prev_t
a_ned  = (vel - self._prev_vel) / dt_acc        # تسارع في NED
sf_body = _rotate_ned_to_body(q, a_ned - GRAVITY_NED)  # تحويل إلى إطار الجسم FRD
```

يحسب القوة النوعية (ما يقيسه مقياس التسارع الحقيقي) عبر:
- حساب التسارع الخطي من تغير السرعة
- طرح الجاذبية (لأن IMU لا يقيس الجاذبية في السقوط الحر)
- تحويل من إطار NED إلى إطار الجسم FRD باستخدام مرافق الرباعي (quaternion conjugate)

#### رسالة HIL_STATE_QUATERNION (100 Hz)

```python
# سطر 435-452
conn.mav.hil_state_quaternion_send(
    t_usec,
    [w, x, y, z],                    # رباعي التوجه body→NED
    omega[0], omega[1], omega[2],    # سرعة زاوية (body FRD) rad/s
    int(lat_deg * 1e7),              # خط عرض degE7
    int(lon_deg * 1e7),              # خط طول degE7
    int(alt_msl * 1000),             # ارتفاع mm MSL
    int(vel[0] * 100), int(vel[1] * 100), int(vel[2] * 100),  # سرعة NED cm/s
    0,                               # سرعة هواء مبينة
    int(spd_3d * 100),              # سرعة هواء حقيقية cm/s
    xacc_mG, yacc_mG, zacc_mG,     # تسارع body mG
)
```

**الغرض:** يُنشر كـ ground truth في PX4:
- `vehicle_attitude_groundtruth` — التوجه الحقيقي
- `vehicle_local_position_groundtruth` — الموقع الحقيقي

يمكن لـ `rocket_gnc` قراءة هذه البيانات مباشرة (تجاوز EKF2) في وضع SITL.

#### رسالة HIL_GPS (5 Hz — مُحددة المعدل)

```python
# سطر 455-469 — كل 20 خطوة فيزيائية (100Hz/20 = 5Hz)
self._gps_counter += 1
if self._gps_counter % 20 == 0:
    conn.mav.hil_gps_send(
        t_usec,
        3,                                # 3D fix
        int(lat_deg * 1e7),               # خط عرض degE7
        int(lon_deg * 1e7),               # خط طول degE7
        int(alt_msl * 1000),              # ارتفاع mm MSL
        50, 50,                           # دقة أفقية/رأسية (cm)
        int(ground_speed * 100),          # سرعة أرضية cm/s
        int(vel[0] * 100),               # سرعة شمال cm/s
        int(vel[1] * 100),               # سرعة شرق cm/s
        int(vel[2] * 100),               # سرعة أسفل cm/s
        0, 12,                            # اتجاه المسار، عدد الأقمار
    )
```

**لماذا 5 Hz؟** GPS الحقيقي لا يعمل بـ 100 Hz. تحديد المعدل يحاكي سلوكاً واقعياً.

### 4.2 المعالجة داخل PX4

```
HIL_SENSOR ──────────► simulator_mavlink ──► sensor_combined (uORB)
                          │                         │
HIL_STATE_QUATERNION ────►│──► vehicle_attitude_groundtruth (uORB)
                          │──► vehicle_local_position_groundtruth (uORB)
                          │                         │
HIL_GPS ─────────────────►│──► sensor_gps (uORB)    │
                                    │               │
                                    ▼               ▼
                              ┌──────────────────────────┐
                              │         EKF2             │
                              │  (250 Hz prediction)     │
                              │  (EKF2_PREDICT_US=4000)  │
                              └──────────────────────────┘
                                    │               │
                                    ▼               ▼
                          vehicle_attitude   vehicle_local_position
                              (uORB)              (uORB)
                                    │               │
                                    └───────┬───────┘
                                            ▼
                                   ┌────────────────┐
                                   │  rocket_gnc    │
                                   │  (100 Hz)      │
                                   └────────────────┘
                                            │
                                            ▼
                                 actuator_outputs_sim (uORB)
                                            │
                                            ▼
                                    simulator_mavlink
                                            │
                                            ▼
                              HIL_ACTUATOR_CONTROLS (MAVLink)
                                            │
                                       TCP:4560
                                            │
                                            ▼
                                   6DOF_v2_130 physics
```

#### ما يفعله `simulator_mavlink`:

- **`handle_message_hil_sensor()`**: يستقبل HIL_SENSOR ← يحقن في أجهزة حساسات وهمية (`_px4_accel`, `_px4_gyro`, `_px4_mag`, `_px4_baro`) ← يُنشر كـ `sensor_combined` عبر uORB ← **يُقدّم الزمن في lockstep scheduler**
- **`handle_message_hil_gps()`**: يستقبل HIL_GPS ← يُنشئ `sensor_gps_s` ← يُنشر على uORB
- **`handle_message_hil_state_quaternion()`**: يستقبل الحالة الحقيقية ← يُنشر كـ groundtruth topics
- **`send_controls()`**: يقرأ `actuator_outputs_sim` من uORB ← يُحوّل إلى `HIL_ACTUATOR_CONTROLS` ← يرسل للمحاكي

#### ما يفعله `rocket_gnc`:

**المدخلات (اشتراكات uORB):**
- `sensor_combined` — بيانات IMU
- `vehicle_attitude` أو `vehicle_attitude_groundtruth` — التوجه
- `vehicle_local_position` أو `vehicle_local_position_groundtruth` — الموقع والسرعة
- `vehicle_air_data` — بيانات الغلاف الجوي
- `parameter_update` — تحديثات المعاملات

**المخرجات (نشر uORB):**
- `actuator_outputs_sim` — أوامر الزعانف الأربعة (تذهب لـ simulator_mavlink ثم للمحاكي)
- `actuator_servos` — أوامر السيرفو (تُستخدم في HITL مع `xqpower_can`)
- `rocket_gnc_status` — حالة GNC الداخلية للمراقبة

**الخوارزمية:**
1. قراءة الحالة المقدرة (التوجه + الموقع + السرعة)
2. حساب التوجيه بملاحة تناسبية مُنحازة (Biased Proportional Navigation)
3. حساب أوامر الزعانف الأربعة
4. نشر الأوامر

### 4.3 أوامر الزعانف: PX4 → 6DOF (حتى 100 Hz)

```python
# في run_px4_hil.py — خيط الاستقبال الخلفي (سطر 369-391)
def _recv_loop(self):
    while self._running:
        msg = self._conn.recv_match(
            type=['HIL_ACTUATOR_CONTROLS', 'HEARTBEAT'],
            blocking=True, timeout=0.05)
        if msg is None:
            continue
        if msg.get_type() == 'HIL_ACTUATOR_CONTROLS':
            controls = list(msg.controls)[:4]   # 4 زعانف فقط
            fins = np.array([0.0 if (f is None or isnan(f)) else float(f)
                             for f in controls])
            with self._fins_lock:
                self._fins = fins               # تحديث ذري محمي بقفل
```

```python
# في run_px4_hil.py — دالة التحكم (سطر 300-363)
def __call__(self, state_dict, t):
    if self._rk4_idx == 0:                      # k1 فقط من RK4
        self._send_hil(state_dict, t)           # إرسال حساسات
    self._rk4_idx = (self._rk4_idx + 1) % 4

    with self._fins_lock:
        return self._fins.copy()                # إرجاع آخر أوامر (non-blocking)
```

**النقاط المهمة:**
- الاستقبال في **خيط خلفي مستقل** — لا يُعيق محرك الفيزياء أبداً
- حماية المتغير المشترك `_fins` بـ **threading.Lock** لأمان الخيوط
- إرسال HIL يحدث فقط في **مرحلة k1** من RK4 (ليس k2/k3/k4) لتجنب إرسال 4 أضعاف
- المحاكي يقرأ **آخر قيمة متاحة** بدون انتظار — تماماً كما يفعل Gazebo

---

## 5. آلية التزامن الزمني (Lockstep)

PX4 SITL يستخدم **lockstep scheduler** — لا يتقدم الزمن داخل PX4 حتى يستلم رسالة `HIL_SENSOR` جديدة من المحاكي.

```
الزمن ──►

6DOF:   [حساب فيزياء] → [إرسال HIL_SENSOR] → [حساب فيزياء] → [إرسال HIL_SENSOR] → ...
                              │                                      │
PX4:    [ينتظر...] ← [يتقدم خطوة] → [EKF2+GNC] → [ينتظر...] ← [يتقدم خطوة] → ...
                              │                                      │
                    └── HIL_ACTUATOR_CONTROLS ──►          └── HIL_ACTUATOR_CONTROLS ──►
```

**الفوائد:**
- تطابق زمني مثالي بين المحاكاة و PX4
- لا تضيع أي خطوة فيزيائية
- قابلية التكرار (reproducibility) في الاختبارات
- يمكن تشغيل المحاكاة أسرع أو أبطأ من الزمن الحقيقي

**التوقيت الحقيقي (Real-time Pacing):**
```python
# سطر 307-314
wall_elapsed = time.monotonic() - self._wall_start
sleep_needed = t - wall_elapsed
if sleep_needed > 0.001:
    time.sleep(sleep_needed)   # ينتظر ساعة الحائط
```

المحاكي ينتظر حتى يلحق الزمن الحقيقي بزمن المحاكاة — هذا يمنع المحاكاة من الجريان أسرع من الواقع.

---

## 6. مرحلة تسخين EKF2

قبل بدء المحاكاة، يُرسل المحاكي **12 ثانية** من بيانات ثابتة تمثل الصاروخ على منصة الإطلاق:

```python
# سطر 217-244
def warmup(self, elevation_deg=15.0, duration=12.0):
    elev = math.radians(elevation_deg)
    # رباعي لزاوية ارتفاع (أنف لأعلى)، yaw=0, roll=0
    q = [cos(elev/2), 0.0, sin(elev/2), 0.0]
    pos = vel = omega = [0, 0, 0]   # ثابت على المنصة

    for i in range(int(duration / 0.02)):   # 50 Hz لمدة 12 ثانية
        self._send_hil(state, t=i*0.02)
        time.sleep(0.02)

    self._t_offset = duration   # زمن المحاكاة يبدأ من 12 ثانية
```

**لماذا التسخين ضروري؟**
- EKF2 يحتاج وقتاً لتهيئة التغايرات (covariances)
- يحتاج لتثبيت الاتجاه من المغناطيسي
- يحتاج لمعايرة bias الجايروسكوب ومقياس التسارع
- بدون تسخين، أول ثوانٍ من الطيران ستكون بتقديرات غير مستقرة

---

## 7. التسليح القسري

```python
# سطر 269-294
def force_arm(self):
    conn.mav.command_long_send(
        1, 1,    # target_system, target_component
        MAV_CMD_COMPONENT_ARM_DISARM,
        0,       # confirmation
        1.0,     # arm
        21196.0, # magic force-arm (يتجاوز فحوصات ما قبل الطيران)
        0, 0, 0, 0, 0,
    )
```

**لماذا التسليح القسري ضروري في SITL؟**
`simulator_mavlink` لا ينسخ `actuator_outputs` إلى `HIL_ACTUATOR_CONTROLS` إلا عندما تكون حالة المركبة `ARMING_STATE_ARMED`. بدون تسليح، الزعانف تبقى صفرية بغض النظر عن مخرجات `rocket_gnc`.

---

## 8. ضبط EKF2 للصاروخ

ملف `rc.rocket_defaults` يحتوي ضبطاً حرجاً لأن ديناميكيات الصاروخ تختلف جذرياً عن الطائرات:

### 8.1 حدود السرعة

| المعامل | القيمة | الافتراضي | السبب |
|---------|--------|-----------|-------|
| `EKF2_VEL_LIM` | 2000 m/s | 100 m/s | الصاروخ يتجاوز سرعة الصوت (~340 m/s). القيمة الافتراضية ترفض GPS وتقيد حالة السرعة بعد Mach 0.3 |

### 8.2 التحكم بمصادر الدمج

| المعامل | القيمة | السبب |
|---------|--------|-------|
| `EKF2_HGT_REF` | 1 | البارومتر كمرجع ارتفاع |
| `EKF2_GPS_CTRL` | 7 | GPS لموقع + سرعة |
| `EKF2_BARO_CTRL` | 1 | البارومتر مُفعّل |
| `EKF2_IMU_CTRL` | 3 | تعطيل دمج الجاذبية (bit2=0). التسارع يتجاوز 1.1g خلال 0.1 ثانية من الإشعال ويبقى فوقه طوال 4.8 ثانية احتراق. بوابة `gravity_fusion.cpp` تقبل فقط [0.9g, 1.1g] |
| `EKF2_MAG_TYPE` | 6 | مغناطيسي للتهيئة فقط (Init). تيار المحرك (~عشرات الأمبير) يُنشئ تشويش hard-iron يُفسد دمج 3-axis mag ويُحرف الـ yaw |

### 8.3 بوابات GPS وتشويش

| المعامل | القيمة | الافتراضي | السبب |
|---------|--------|-----------|-------|
| `EKF2_GPS_P_GATE` | 10.0 | 5.0 | بوابة واسعة للتسارع العالي |
| `EKF2_GPS_V_GATE` | 10.0 | 5.0 | عند 15g مع jitter GPS 20-30ms → ابتكار سرعة ~4.4 m/s. البوابة 10-sigma = 15 m/s |
| `EKF2_GPS_V_NOISE` | 1.5 m/s | 0.3 m/s | عند سرعات عابرة/فوق صوتية، دقة GPS تتدهور بسبب: scintillation أيونوسفيري، إجهاد ديناميكي على الهوائي، jerk عالي |
| `EKF2_BARO_NOISE` | 12.0 m | 3.5 m | عند سرعات عابرة/فوق صوتية، موجات الصدم تُنشئ أخطاء ضغط ساكن بعشرات الأمتار. 12 m يجعل EKF يعتمد على GPS للارتفاع |
| `EKF2_ASPD_MAX` | 50.0 m/s | — | الحد الأقصى في الكود. فوق هذا التعويض يتشبع، لكن BARO_NOISE=12 يُقلل وزن البارومتر أصلاً |

### 8.4 سرعة التنبؤ

| المعامل | القيمة | الافتراضي | السبب |
|---------|--------|-----------|-------|
| `EKF2_PREDICT_US` | 4000 µs (250 Hz) | 10000 µs (100 Hz) | عند 15g، خطأ الموقع لكل خطوة: 100 Hz = 0.0074 m، 250 Hz = 0.0012 m. السرعة الأعلى تتتبع الحالة المتغيرة بدقة أكبر |
| `EKF2_NOAID_TOUT` | 2000000 µs (2 s) | 5000000 µs (5 s) | عند 500+ m/s، 5 ثوانٍ dead-reckoning تراكم انحراف بمقياس كيلومترات. 2 ثانية تكتشف فقدان GPS أسرع |

### 8.5 الحساسات الوهمية

```
SENS_EN_GPSSIM  = 1   # محاكاة GPS
SENS_EN_BAROSIM = 1   # محاكاة بارومتر
SENS_EN_MAGSIM  = 1   # محاكاة مغناطيسي
SENS_EN_ARSPDSIM = 0  # بدون محاكاة سرعة هواء
```

### 8.6 تعطيل الفحوصات

```
COM_ARM_CHK_ESCS = 0   # لا محركات ESC في صاروخ
FD_ESCS_EN       = 0   # لا كشف خلل ESC
FD_FAIL_P        = 0   # لا كشف خلل pitch
FD_FAIL_R        = 0   # لا كشف خلل roll
COM_ARM_HFLT_CHK = 0   # لا فحص خلل ارتفاع
CBRK_SUPPLY_CHK  = 894281  # تعطيل فحص مزود الطاقة
COM_DISARM_PRFLT = -1  # لا نزع تسليح تلقائي
```

### 8.7 تعطيل الـ Failsafe

```
COM_ACT_FAIL_ACT = 0   # لا إجراء عند خلل المشغل
COM_LOW_BAT_ACT  = 0   # لا إجراء عند بطارية منخفضة
NAV_DLL_ACT      = 0   # لا إجراء عند فقدان رابط البيانات
NAV_RCL_ACT      = 0   # لا إجراء عند فقدان RC
GF_ACTION        = 0   # لا إجراء عند اختراق السياج الجغرافي
```

---

## 9. معاملات GNC الصاروخي

من ملف الإطار `22000_m130_rocket`:

### 9.1 مراحل الطيران

| المعامل | القيمة | الوصف |
|---------|--------|-------|
| `ROCKET_SET_ALT` | 120.0 m | ارتفاع هدف المرحلة 1 |
| `ROCKET_T_STG1` | 15.0 s | زمن حرق المحرك / نهاية المرحلة 1 |
| `ROCKET_T_CTRL` | 0.2 s | تأخير بدء التحكم بعد الإطلاق |

### 9.2 التوجيه (Proportional Navigation)

| المعامل | القيمة | الوصف |
|---------|--------|-------|
| `ROCKET_NPN` | 2.7 | ثابت ملاحة PN |
| `ROCKET_IMP_ANG` | -30.0° | زاوية اصطدام نهائية (سالب = غوص) |
| `ROCKET_TAU_PN1` | 20.0 s | ثابت زمني للانحياز |
| `ROCKET_K_YAW` | 0.008 | كسب توجيه yaw |
| `ROCKET_K_VZ` | 14.0 | كسب خطأ السرعة |

### 9.3 حدود التسارع

| المعامل | القيمة | الوصف |
|---------|--------|-------|
| `ROCKET_AYC_LIM` | 2.0 g | حد تسارع yaw |
| `ROCKET_APC_LIM` | 8.0 g | حد تسارع pitch |

### 9.4 الهدف والزعانف

| المعامل | القيمة | الوصف |
|---------|--------|-------|
| `ROCKET_XTRGT` | 3000.0 m | هدف شمال (NED) |
| `ROCKET_YTRGT` | 0.0 m | هدف شرق |
| `ROCKET_ZTRGT` | 0.0 m | هدف أسفل |
| `ROCKET_MAX_DEFL` | 0.436 rad (25°) | أقصى انحراف زعنفة |

---

## 10. رسائل MAVLink — ملخص التبادل

### 6DOF → PX4

| الرسالة | المحتوى | الوحدات | المعدل |
|---------|---------|---------|--------|
| `HIL_SENSOR` | تسارع IMU (FRD body) | m/s² | 100 Hz |
| | جايروسكوب (FRD body) | rad/s | 100 Hz |
| | مغناطيسي | Gauss | 100 Hz |
| | ضغط بارومتري | hPa | 100 Hz |
| `HIL_STATE_QUATERNION` | رباعي التوجه (ground truth) | — | 100 Hz |
| | موقع NED | degE7, mm | 100 Hz |
| | سرعة NED | cm/s | 100 Hz |
| | سرعة زاوية body | rad/s | 100 Hz |
| `HIL_GPS` | موقع (lat, lon, alt) | degE7, mm | 5 Hz |
| | سرعة NED | cm/s | 5 Hz |
| | دقة أفقية/رأسية | cm | 5 Hz |
| `HEARTBEAT` | نبضة حياة | — | 1 Hz |

### PX4 → 6DOF

| الرسالة | المحتوى | الوحدات | المعدل |
|---------|---------|---------|--------|
| `HIL_ACTUATOR_CONTROLS` | `controls[0..3]` = زعانف 1-4 | rad | حتى 100 Hz |
| | `mode` = حالة التسليح | — | — |
| `HEARTBEAT` | نبضة حياة PX4 | — | 1 Hz |

---

## 11. حلقة المحاكاة الكاملة — خطوة واحدة (10 ms)

```
الخطوة 1: 6DOF يحسب الفيزياء
  ├── دفع المحرك (thrust profile)
  ├── قوى ديناميكا هوائية (سحب + رفع من الجسم والزعانف)
  ├── جاذبية
  ├── قوى الزعانف (من آخر أوامر PX4)
  └── تكامل RK4 → حالة جديدة (pos, vel, quat, omega)

الخطوة 2: GazeboStylePlugin.__call__() — مرحلة k1 فقط
  ├── Real-time pacing: sleep إذا المحاكاة أسرع من الزمن الحقيقي
  ├── حساب القوة النوعية: sf_body = Rᵀ(a_ned - g)
  ├── إرسال HIL_SENSOR (IMU + baro)
  ├── إرسال HIL_STATE_QUATERNION (ground truth)
  ├── إرسال HIL_GPS (كل 20 خطوة = 5 Hz)
  └── تسجيل CSV (كل 10 خطوات = 10 Hz)

الخطوة 3: PX4 يتقدم (lockstep triggered بـ HIL_SENSOR)
  ├── simulator_mavlink يحقن البيانات في uORB
  ├── EKF2 يُقدّر الحالة (250 Hz prediction)
  │    ├── دمج IMU (تنبؤ)
  │    ├── دمج GPS (تحديث) — كل 200 ms
  │    └── دمج بارومتر (تحديث)
  ├── rocket_gnc يقرأ الحالة المقدرة
  │    ├── حساب التوجيه (Proportional Navigation)
  │    ├── حساب أوامر الزعانف
  │    └── نشر actuator_outputs_sim
  └── simulator_mavlink يرسل HIL_ACTUATOR_CONTROLS

الخطوة 4: 6DOF يستقبل الزعانف (خيط خلفي)
  ├── _recv_loop() يقرأ HIL_ACTUATOR_CONTROLS
  ├── يحدّث _fins[0..3] محمياً بقفل
  └── الخطوة التالية تستخدم هذه القيم

→ العودة للخطوة 1
```

---

## 12. الفرق بين SITL و HITL

| الجانب | SITL | HITL |
|--------|------|------|
| **وسيلة الاتصال** | TCP localhost:4560 | USB Serial `/dev/ttyACM0` |
| **الهاردوير** | لا يوجد — كل شيء على الحاسوب | MicoAir H743 حقيقي |
| **ملف المُشغّل** | `run_px4_hil.py` | `run_px4_hitl.py` |
| **ملف الإطار** | `22000_m130_rocket` | `22001_m130_rocket_hitl` |
| **الزعانف** | رقمية فقط (تعود للمحاكي) | `xqpower_can` يحرك سيرفو حقيقي عبر CAN 500 kbps |
| **التزامن** | lockstep (حتمي) | وقت حقيقي (real-time) |
| **EKF2** | يعمل على الحاسوب | يعمل على H743 |
| **rocket_gnc** | يعمل على الحاسوب | يعمل على H743 |

---

## 13. مخرجات المحاكاة

### ملف CSV

يُسجل كل 10 خطوات فيزيائية (~10 Hz) في `6DOF_v2_130/results/sitl/run_<timestamp>/`:

```
time_s, altitude_m, ground_range_m,
pos_north_m, pos_east_m, pos_down_m,
vel_north_m_s, vel_east_m_s, vel_down_m_s,
roll_deg, pitch_deg, yaw_deg,
omega_x_deg_s, omega_y_deg_s, omega_z_deg_s,
accel_north_m_s2, accel_east_m_s2, accel_down_m_s2,
fin1_rad, fin2_rad, fin3_rad, fin4_rad,
fin1_deg, fin2_deg, fin3_deg, fin4_deg,
```

### تقرير PDF

يُولّد تلقائياً بعد المحاكاة ويحتوي 6 صفحات:

1. **المسار** — ارتفاع، مدى أرضي، سرعة NED
2. **انحرافات الزعانف** — 4 زعانف مع حدود ±25°
3. **زوايا أويلر** — roll, pitch, yaw
4. **سرعة زاوية** — p, q, r بإطار الجسم
5. **تسارع NED** — North, East, Down
6. **موقع NED** — North, East, Down

---

## 14. أوامر التشغيل السريع

```bash
# الطرفية 1 — بدء PX4 SITL
cd PX4-Autopilot
PX4_SYS_AUTOSTART=22000 ./build/px4_sitl_default/bin/px4

# الطرفية 2 — بدء محاكاة 6DOF
cd PX4-Autopilot/6DOF_v2_130
python3 run_px4_hil.py

# المخرجات تُحفظ في:
# 6DOF_v2_130/results/sitl/run_<timestamp>/sitl_log_<timestamp>.csv
# 6DOF_v2_130/results/sitl/run_<timestamp>/sitl_report_<timestamp>.pdf
```

---

## 15. ملاحظات تصميمية مهمة

1. **فصل الفيزياء عن GNC:** المحاكي `6DOF_v2_130` يعطل GNC الداخلي (`use_autopilot=False`, `use_guidance=False`) ويعمل كمحرك فيزياء صرف. التحكم يأتي بالكامل من PX4 `rocket_gnc`.

2. **نموذج Gazebo:** البنية تحاكي Gazebo تماماً — `GazeboStylePlugin` يقابل Gazebo Update() callback، والخيط الخلفي يقابل Gazebo recv callback.

3. **عدم الحجب:** المحاكي لا ينتظر أبداً أوامر الزعانف من PX4. يقرأ آخر قيمة متاحة ويستمر — هذا يمنع أي تأخر في حلقة الفيزياء.

4. **حماية RK4:** إرسال HIL يحدث فقط في مرحلة k1 من المُكامل RK4، لتجنب إرسال بيانات 4 مرات لكل خطوة فيزيائية واحدة.

5. **تحويل الإحداثيات:** المحاكي يعمل بإحداثيات NED محلية (`long_range_mode=False`) — كافٍ لمدى 3500 م. التحويل لـ lat/lon يتم بتقريب خطي بسيط.

6. **Ground Truth مزدوج:** يُرسل كل من EKF2-processed data (عبر `HIL_SENSOR` → `sensor_combined`) و ground truth (عبر `HIL_STATE_QUATERNION` → `*_groundtruth`). هذا يسمح بمقارنة تقدير EKF2 مع الواقع.
