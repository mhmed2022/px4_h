/**
 * PX4 Android Bridge — Arduino Receiver
 * ========================================
 * يستقبل أوامر المحركات من تطبيق PX4 على Android عبر USB Serial
 * ويرسل إشارات PWM إلى ESCs للتحكم بالمحركات.
 * 
 * البروتوكول:
 *   [0xAA][0x55]  — Sync header (2 bytes)
 *   [N]           — عدد القنوات (1 byte)
 *   [ch0_H][ch0_L] — قناة 0: PWM microseconds (2 bytes, big-endian)
 *   [ch1_H][ch1_L] — قناة 1
 *   ...
 *   [checksum]    — XOR checksum (1 byte)
 * 
 * التوصيلات:
 *   Motor 1 → Pin 3  → ESC 1
 *   Motor 2 → Pin 5  → ESC 2
 *   Motor 3 → Pin 6  → ESC 3
 *   Motor 4 → Pin 9  → ESC 4
 * 
 * معدل الإرسال: 50 Hz (كل 20ms)
 * باود ريت: 115200
 */

#include <Servo.h>

// ===== إعدادات المحركات =====
// الترتيب مطابق لـ PX4 4001_quad_x الرسمي!
// من ملف: ROMFS/px4fmu_common/init.d/airframes/4001_quad_x
// CA_ROTOR0_PX=1, PY=1  → Front Right (CW)
// CA_ROTOR1_PX=-1, PY=-1 → Back Left  (CW)
// CA_ROTOR2_PX=1, PY=-1  → Front Left  (CCW, KM=-0.05)
// CA_ROTOR3_PX=-1, PY=1  → Back Right  (CCW, KM=-0.05)
static const int MOTOR_PINS[4] = {7, 11, 8, 10};  // [output0, output1, output2, output3]
// MOTOR_PINS[0] = Pin 7  → output[0] → Front Right  (CW)
// MOTOR_PINS[1] = Pin 11 → output[1] → Back Left    (CW)
// MOTOR_PINS[2] = Pin 8  → output[2] → Front Left   (CCW)
// MOTOR_PINS[3] = Pin 10 → output[3] → Back Right   (CCW)
static const int NUM_MOTORS = 4;

// ===== إعدادات البروتوكول =====
static const byte SYNC_H = 0xAA;
static const byte SYNC_L = 0x55;
static const byte MAX_CHANNELS = 16;

// ===== إعدادات PWM =====
static const uint16_t PWM_MIN = 1000;    // أقل قيمة (متوقف)
static const uint16_t PWM_MAX = 2000;    // أعلى قيمة (كامل)
static const uint16_t PWM_DISARMED = 1000; // قيمة المحركات عند نزع التسليح

// ===== Watchdog — إيقاف المحركات عند انقطاع الاتصال =====
static const unsigned long FRAME_TIMEOUT_MS = 300; // 300ms ≈ 15 إطار @50Hz
unsigned long lastFrameTime = 0;

// ===== المتغيرات العامة =====
Servo motors[NUM_MOTORS];
byte receiveBuffer[2 + 1 + (MAX_CHANNELS * 2) + 1]; // buffer كامل
int bufferIndex = 0;
bool syncing = true;  // نبحث عن sync bytes حالياً

// ===== إحصائيات للتشخيص =====
unsigned long packetsReceived = 0;
unsigned long packetsInvalid = 0;
unsigned long lastStatusTime = 0;

void setup() {
  // بدء الاتصال التسلسلي (نفس المعدل المُعد في Android: 115200)
  Serial.begin(115200);
  
  Serial.println("========================================");
  Serial.println("  PX4 Android Bridge — Arduino Receiver");
  Serial.println("========================================");
  Serial.print("  Motors: ");
  Serial.print(NUM_MOTORS);
  Serial.println(" channels");
  Serial.println("  Baud: 115200");
  Serial.println("  Waiting for PX4 commands...");
  Serial.println("========================================");
  
  // تهيئة المحركات (توصيل Servo بأرقام الدبابيس)
  for (int i = 0; i < NUM_MOTORS; i++) {
    motors[i].attach(MOTOR_PINS[i], PWM_MIN, PWM_MAX);
    motors[i].writeMicroseconds(PWM_DISARMED);
  }
  
  Serial.println("========================================");
  Serial.println("  Ready! ✓");
  Serial.println("========================================");
  
  // إرسال رسالة جاهزية (اختياري — التطبيق لا ينتظرها)
  Serial.println("STATUS: Arduino ready, waiting for frames...");
}

void loop() {
  // قراءة البيانات التسلسلية المتاحة
  while (Serial.available() > 0) {
    byte incoming = Serial.read();
    
    if (syncing) {
      // === مرحلة المزامنة: نبحث عن 0xAA ثم 0x55 ===
      if (incoming == SYNC_H) {
        bufferIndex = 0;
        receiveBuffer[0] = SYNC_H;
        bufferIndex = 1;
      } else if (incoming == SYNC_L && bufferIndex == 1) {
        receiveBuffer[1] = SYNC_L;
        bufferIndex = 2;
        syncing = false;  // وجدنا Sync! نبدأ قراءة البيانات
      } else {
        // لم نجد Sync — نعيد المحاولة
        bufferIndex = 0;
      }
    } else {
      // === مرحلة قراءة البيانات ===
      receiveBuffer[bufferIndex] = incoming;
      bufferIndex++;
      
      // نتحقق إذا استلمنا الإطار كاملاً
      // الحجم = 2(sync) + 1(count) + N*2(channels) + 1(checksum)
      if (bufferIndex >= 3) {
        int numChannels = receiveBuffer[2];
        int expectedSize = 2 + 1 + (numChannels * 2) + 1;
        
        if (bufferIndex >= expectedSize) {
          // الإطار كامل — نتحقق منه
          if (validateAndProcessFrame(expectedSize)) {
            packetsReceived++;
            lastFrameTime = millis();
          } else {
            packetsInvalid++;
          }
          // نعود لمرحلة المزامنة
          syncing = true;
          bufferIndex = 0;
        }
      }
    }
  }
  
  // ===== Watchdog: إذا لم نستلم إطار خلال FRAME_TIMEOUT_MS → إيقاف المحركات =====
  if (lastFrameTime != 0 && (millis() - lastFrameTime) > FRAME_TIMEOUT_MS) {
    for (int i = 0; i < NUM_MOTORS; i++) {
      motors[i].writeMicroseconds(PWM_DISARMED);
    }
  }

  // طباعة حالة تشخيصية كل 5 ثوانٍ
  if (millis() - lastStatusTime > 5000) {
    printStatus();
    lastStatusTime = millis();
  }
}

/**
 * التحقق من صحة الإطار ومعالجته
 * @param frameSize حجم الإطار الكامل
 * @return true إذا كان صحيحاً
 */
bool validateAndProcessFrame(int frameSize) {
  // 1. التحقق من Sync bytes
  if (receiveBuffer[0] != SYNC_H || receiveBuffer[1] != SYNC_L) {
    return false;
  }
  
  // 2. التحقق من عدد القنوات
  int numChannels = receiveBuffer[2];
  if (numChannels == 0 || numChannels > MAX_CHANNELS) {
    return false;
  }
  
  // 3. التحقق من الحجم
  int expectedSize = 2 + 1 + (numChannels * 2) + 1;
  if (frameSize != expectedSize) {
    return false;
  }
  
  // 4. التحقق من Checksum (XOR)
  byte checksum = 0;
  int dataEnd = frameSize - 1; // آخر بايت هو checksum
  for (int i = 0; i < dataEnd; i++) {
    checksum ^= receiveBuffer[i];
  }
  
  byte receivedChecksum = receiveBuffer[dataEnd];
  if (checksum != receivedChecksum) {
    Serial.print("CHECKSUM ERROR: calc=");
    Serial.print(checksum);
    Serial.print(" recv=");
    Serial.println(receivedChecksum);
    return false;
  }
  
  // ✅ الإطار صحيح — نُطبق الأوامر على المحركات
  for (int i = 0; i < NUM_MOTORS; i++) {
    if (i < numChannels) {
      // استخراج قيمة PWM من بايتين (big-endian)
      uint16_t pwmValue = (receiveBuffer[3 + i * 2] << 8) | receiveBuffer[3 + i * 2 + 1];
      
      // التأكد أن القيمة ضمن النطاق الآمن
      if (pwmValue < PWM_MIN) pwmValue = PWM_MIN;
      if (pwmValue > PWM_MAX) pwmValue = PWM_MAX;
      
      // تطبيق القيمة على المحرك
      motors[i].writeMicroseconds(pwmValue);
    } else {
      // قناة غير موجودة — نضع قيمة آمنة
      motors[i].writeMicroseconds(PWM_DISARMED);
    }
  }
  
  return true;
}

/**
 * طباعة حالة تشخيصية
 */
void printStatus() {
  Serial.print("STATUS: pkts=");
  Serial.print(packetsReceived);
  Serial.print("  invalid=");
  Serial.print(packetsInvalid);
  Serial.print("  motors=[");
  
  // طباعة قيم PWM الحالية لكل محرك
  for (int i = 0; i < NUM_MOTORS; i++) {
    // ملاحظة: لا توجد دالة readMicroseconds في Servo library
    // نستخدم قيمة تقديرية أو نتجاهل هذا الجزء
    Serial.print("motor");
    Serial.print(i + 1);
    if (i < NUM_MOTORS - 1) Serial.print(",");
  }
  Serial.println("]");
}
