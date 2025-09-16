//By Khalil Adawy 10/9/2025

#include <Wire.h>

#define IC_ENABLE 21
#define SDA_PIN 1
#define SCL_PIN 2


void setup() {
  pinMode(IC_ENABLE, OUTPUT);
  digitalWrite(IC_ENABLE, HIGH);
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to initialize
  Serial.println("I2C Scanner");
}

void loop() {
  Serial.println("\nScanning for I2C devices...");

  int devicesFound = 0;
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      devicesFound++;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (devicesFound == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("Scan complete\n");
  }

  delay(5000); // Wait 5 seconds before the next scan
}
