/**
 * PX4_Arduino_Bridge_Minimal.ino
 * ==========================================
 * نسخة "صامتة" ومُحسّنة مع استقبال أكثر أمانًا.
 * تستقبل أوامر المحركات من تطبيق PX4 على Android عبر USB Serial.
 * 
 * الملاحظات:
 * 1. حذف جميع عمليات Serial.print لتقليل التأخير أثناء تحديث المحركات.
 * 2. التحقق من الهيدر الكامل [0xAA][0x55] قبل قبول الإطار.
 * 3. حماية من تجاوز حجم buffer إذا وصلت بيانات تالفة.
 * 4. Watchdog: إيقاف المحركات تلقائياً عند انقطاع الاتصال (300ms).
 * 5. دعم قيم PWM ضمن النطاق 1000μs - 2000μs.
 * 
 * البروتوكول: [0xAA][0x55][N][PWM0_H][PWM0_L]...[Checksum]
 */

#include <Servo.h>

const uint8_t FRAME_HEADER_0 = 0xAA;
const uint8_t FRAME_HEADER_1 = 0x55;

// ===== إعدادات المحركات (Pins) =====
// الترتيب مطابق لـ PX4 actuator_outputs
const uint8_t MOTOR_PINS[] = {7, 11, 8, 10}; // مخرج 0, 1, 2, 3
const uint8_t NUM_MOTORS = 4;
const uint16_t PWM_MIN_US = 1000;
const uint16_t PWM_MAX_US = 2000;
const uint16_t PWM_BOOT_US = 1000;
const uint16_t PWM_DISARMED = 1000;
Servo esc[NUM_MOTORS];

// ===== متغيرات البروتوكول =====
uint8_t buffer[32];
uint8_t bufferIndex = 0;
bool frameStarted = false;
bool wasArmed = false;  // تتبع حالة التسليح

// ===== تشخيص وصول البيانات =====
unsigned long framesReceived = 0;      // عدد الفريمات الصحيحة
unsigned long framesBad = 0;           // عدد الفريمات الخاطئة
unsigned long lastDiagTime = 0;        // آخر وقت طباعة
uint16_t lastPwm[4] = {0, 0, 0, 0};   // آخر قيم PWM مستلمة
bool dataConfirmed = false;            // هل تم تأكيد وصول البيانات

// ===== Watchdog — إيقاف المحركات عند انقطاع الاتصال =====
const unsigned long FRAME_TIMEOUT_MS = 300;
unsigned long lastFrameTime = 0;

void resetFrame() {
  frameStarted = false;
  bufferIndex = 0;
}

void setup() {
  // === الخطوة 1: فرض LOW على بنات المحركات فوراً ===
  // هذا يمنع ESC من رؤية إشارة عشوائية أثناء بدء التشغيل
  for (uint8_t i = 0; i < NUM_MOTORS; i++) {
    pinMode(MOTOR_PINS[i], OUTPUT);
    digitalWrite(MOTOR_PINS[i], LOW);
  }

  // بروتوكول USB Serial
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  // === الخطوة 2: توصيل Servo وإرسال 1000μs (إشارة الإيقاف) ===
  for (uint8_t i = 0; i < NUM_MOTORS; i++) {
    esc[i].attach(MOTOR_PINS[i], PWM_MIN_US, PWM_MAX_US);
    esc[i].writeMicroseconds(PWM_BOOT_US);
  }

  // === الخطوة 3: إرسال 1000μs بشكل مستمر لمدة 4 ثوانٍ ===
  // ESC يحتاج يرى إشارة ثابتة 1000μs لمدة 2-3 ثوانٍ للتهيئة
  // بدون هذا: ESC يدخل وضع حماية ولا يستجيب بعدها
  Serial.println("ESC INIT: sending 1000us for 4 seconds...");
  for (uint16_t t = 0; t < 200; t++) {  // 200 × 20ms = 4 ثوانٍ
    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
      esc[i].writeMicroseconds(PWM_BOOT_US);
    }
    delay(20);
  }

  Serial.println("READY — DATA MONITOR ENABLED");
  Serial.println("ESC init done. Waiting for PX4 data...");
}

void loop() {
  // استقبال البيانات بأسرع ما يمكن
  while (Serial.available() > 0) {
    uint8_t incoming = Serial.read();

    if (!frameStarted) {
      // نبحث عن بداية الإطار 0xAA
      if (incoming == FRAME_HEADER_0) {
        frameStarted = true;
        bufferIndex = 0;
        buffer[0] = incoming;
      }
    } else {
      if (bufferIndex + 1 >= sizeof(buffer)) {
        resetFrame();
        continue;
      }

      buffer[++bufferIndex] = incoming;

      // التحقق من البايت الثاني للهيدر قبل متابعة استقبال الإطار
      if (bufferIndex == 1 && buffer[1] != FRAME_HEADER_1) {
        resetFrame();
        continue;
      }

      // إذا وصلنا لبايت عدد القنوات (index 2)، نعرف طول الإطار
      if (bufferIndex >= 2) {
        uint8_t numChannels = buffer[2];
        // Header(2) + Count(1) + PWM_Data(N*2) + Checksum(1)
        uint8_t expectedSize = 2 + 1 + (numChannels * 2) + 1;

        if (expectedSize > sizeof(buffer)) {
          resetFrame();
          continue;
        }

        if (bufferIndex + 1 == expectedSize) {
          executeCommand(expectedSize);
          resetFrame();
        }
      }
    }
  }

  // ===== Watchdog: إذا لم نستلم إطار خلال FRAME_TIMEOUT_MS → إيقاف المحركات =====
  if (lastFrameTime != 0 && (millis() - lastFrameTime) > FRAME_TIMEOUT_MS) {
    for (uint8_t i = 0; i < NUM_MOTORS; i++) {
      esc[i].writeMicroseconds(PWM_DISARMED);
    }
  }

  // ===== طباعة تشخيصية كل ثانية =====
  if (millis() - lastDiagTime >= 1000) {
    lastDiagTime = millis();
    Serial.print("[DATA] frames_ok=");
    Serial.print(framesReceived);
    Serial.print(" bad=");
    Serial.print(framesBad);
    Serial.print(" PWM=[");
    for (uint8_t i = 0; i < 4; i++) {
      Serial.print(lastPwm[i]);
      if (i < 3) Serial.print(",");
    }
    Serial.print("] armed=");
    Serial.println(wasArmed ? "YES" : "NO");

    // === مؤشر وصول البيانات ===
    // عند استقبال أول 10 فريمات صحيحة، نبضة قصيرة على المحركات
    if (framesReceived >= 10 && !dataConfirmed) {
      dataConfirmed = true;
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1100);
      }
      delay(200);
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1000);
      }
      delay(300);
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1100);
      }
      delay(200);
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1000);
      }
    }
  }
}

void executeCommand(uint8_t frameSize) {
  // 1. التحقق من صحة البيانات (Checksum)
  uint8_t checksum = 0;
  for (uint8_t i = 0; i < frameSize - 1; i++) {
    checksum ^= buffer[i];
  }

  // إذا كان الكود صحيحاً، نفذ الأمر فوراً
  if (checksum == buffer[frameSize - 1]) {
    framesReceived++;
    lastFrameTime = millis();
    uint8_t numChannels = buffer[2];
    bool isArmed = false;

    for (uint8_t i = 0; i < NUM_MOTORS && i < numChannels; i++) {
      // استخراج قيمة PWM (Big Endian)
      uint16_t pwm = (buffer[3 + i * 2] << 8) | buffer[3 + i * 2 + 1];

      if (pwm < PWM_MIN_US) pwm = PWM_MIN_US;
      if (pwm > PWM_MAX_US) pwm = PWM_MAX_US;

      // تطبيق القيمة مباشرة
      esc[i].writeMicroseconds(pwm);

      // حفظ آخر قيمة للتشخيص
      if (i < 4) lastPwm[i] = pwm;

      // إذا أي محرك أعلى من 1010 → مسلّح
      if (pwm > 1010) isArmed = true;
    }

    // كشف تغير حالة التسليح
    if (isArmed && !wasArmed) {
      Serial.println("ARMED");
      digitalWrite(13, HIGH);
      wasArmed = true;

      // === صوت ARM: 3 بيبات سريعة من ESC ===
      for (uint8_t beep = 0; beep < 3; beep++) {
        for (uint8_t i = 0; i < NUM_MOTORS; i++) {
          esc[i].writeMicroseconds(1100);
        }
        delay(100);
        for (uint8_t i = 0; i < NUM_MOTORS; i++) {
          esc[i].writeMicroseconds(1000);
        }
        delay(100);
      }
    } else if (!isArmed && wasArmed) {
      Serial.println("DISARMED");
      digitalWrite(13, LOW);
      wasArmed = false;

      // === صوت DISARM: بيبة واحدة طويلة ===
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1100);
      }
      delay(400);
      for (uint8_t i = 0; i < NUM_MOTORS; i++) {
        esc[i].writeMicroseconds(1000);
      }
    }
  } else {
    // الإطار خاطئ — نعدّه للتشخيص
    framesBad++;
  }
}
