// اختبار مباشر للمحرك الرابع فقط — بدون USB
#include <Servo.h>

static const int MOTOR4_PIN = 10; // Motor 4 فقط
Servo motor4;

void setup() {
  Serial.begin(115200);
  Serial.println("=== Motor 4 Only Test ===");
  
  // لا نُفعّل أي محرك آخر: نُوصل Servo واحد فقط للمحرك الرابع
  motor4.attach(MOTOR4_PIN, 1000, 2000);
  motor4.writeMicroseconds(1000);
  
  Serial.println("Sending 1000us to all motors for 5 seconds...");
  Serial.println("ESCs should beep and arm during this time");
  delay(5000);  // ESCs تحتاج تشوف 1000µs لمدة كافية للتسليح

  Serial.print("Testing only Motor 4 on pin ");
  Serial.println(MOTOR4_PIN);
  Serial.println("Pattern: ON 2s -> OFF 2s (repeats)");
}

void loop() {
  motor4.writeMicroseconds(1200);
  Serial.println("Motor 4 ON (1200us) for 2 seconds");
  delay(2000);

  motor4.writeMicroseconds(1000);
  Serial.println("Motor 4 OFF (1000us) for 2 seconds");
  delay(2000);
}
