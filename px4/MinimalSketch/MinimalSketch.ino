/**
 * PX4_Arduino_Bridge_Minimal.ino
 * ==========================================
 * نسخة "صامتة" ومُحسّنة للغاية.
 * تستقبل أوامر المحركات من تطبيق PX4 على Android عبر USB Serial.
 * 
 * التحسينات:
 * 1. حذف جميع عمليات Serial.print لضمان عدم تأخير المحركات.
 * 2. استقبال أسرع للبيانات (Zero Latency).
 * 3. دعم إشارة التوقف القوية (0μs - 900μs).
 * 
 * البروتوكول: [0xAA][0x55][N][PWM0_H][PWM0_L]...[Checksum]
 */

#include <Servo.h>

// ===== إعدادات المحركات (Pins) =====
// الترتيب مطابق لـ PX4 actuator_outputs
const uint8_t MOTOR_PINS[] = {7, 11, 8, 10}; // مخرج 0, 1, 2, 3
const uint8_t NUM_MOTORS = 4;
Servo esc[NUM_MOTORS];

// ===== متغيرات البروتوكول (بدون Serial) =====
uint8_t buffer[32];
uint8_t bufferIndex = 0;
bool syncFound = false;

void setup() {
  // بروتوكول USB Serial
  Serial.begin(115200);

  // تهيئة المحركات (وضع آمن)
  for (uint8_t i = 0; i < NUM_MOTORS; i++) {
    esc[i].attach(MOTOR_PINS[i], 900, 2100); // توسيع النطاق للسماح بإشارة التوقف (900)
    esc[i].writeMicroseconds(1000);          // وضعية التوقف
  }

  // تأخير للسماح للمحركات بالتسليح وسماع صوت "البيب" عند البدء
  // ضروري لأن التطبيق قد يبدأ الإرسال فوراً
  delay(1500);
}

void loop() {
  // استقبال البيانات بأسرع ما يمكن
  while (Serial.available() > 0) {
    uint8_t incoming = Serial.read();

    if (!syncFound) {
      // نبحث عن بداية الإطار 0xAA
      if (incoming == 0xAA) {
        syncFound = true;
        bufferIndex = 0;
        buffer[0] = incoming;
      }
    } else {
      buffer[++bufferIndex] = incoming;

      // إذا وصلنا لبايت عدد القنوات (index 2)، نعرف طول الإطار
      if (bufferIndex >= 2) {
        uint8_t numChannels = buffer[2];
        // Header(2) + Count(1) + PWM_Data(N*2) + Checksum(1)
        uint8_t expectedSize = 2 + 1 + (numChannels * 2) + 1; 

        if (bufferIndex >= expectedSize) {
          executeCommand(expectedSize);
          syncFound = false; // إعادة تعيين للبحث عن الإطار التالي
        }
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
    uint8_t numChannels = buffer[2];
    for (uint8_t i = 0; i < NUM_MOTORS && i < numChannels; i++) {
      // استخراج قيمة PWM (Big Endian)
      uint16_t pwm = (buffer[3 + i * 2] << 8) | buffer[3 + i * 2 + 1];

      // تطبيق حدود أمان (السماح بـ 900 لقوة للإيقاف)
      if (pwm < 900) pwm = 900;
      if (pwm > 2100) pwm = 2100;

      // تطبيق القيمة مباشرة
      esc[i].writeMicroseconds(pwm);
    }
  }
  // إذا كان الإطار خاطئاً، نتجاهله تماماً (صمت)
}
