//by Khalil Adawi 10/9/2025


#define TOUCH_PAD_8_PIN   8
#define TOUCH_PAD_9_PIN   9
#define REFERENCE_PAD_PIN 7

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-S3 Touch Sensor with Reference Calibration");
  delay(1000);
}

void loop() {
  int touch_val_8 = touchRead(TOUCH_PAD_8_PIN);
  int touch_val_9 = touchRead(TOUCH_PAD_9_PIN);
  int ref_val     = touchRead(REFERENCE_PAD_PIN);


  int delta_8 = ref_val - touch_val_8;
  int delta_9 = ref_val - touch_val_9;

  Serial.print("Ref: ");
  Serial.print(ref_val);

  Serial.print(" | Pad 8: ");
  Serial.print(touch_val_8);
  Serial.print(" (Delta: ");
  Serial.print(delta_8);
  Serial.print(")");

  Serial.print(" | Pad 9: ");
  Serial.print(touch_val_9);
  Serial.print(" (Delta: ");
  Serial.print(delta_9);
  Serial.println(")");

  if (touch_val_8> ref_val +5000) {
    Serial.println("********** TOUCH DETECTED ON PAD 8 **********");
  }

  if (abs(delta_9) > 5000) {
    Serial.println("********** TOUCH DETECTED ON PAD 9 **********");
  }

  delay(200);
}