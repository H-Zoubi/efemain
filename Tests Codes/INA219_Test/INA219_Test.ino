#include <Wire.h>
#include <Adafruit_INA219.h>

#define IC_ENABLE 21
#define SDA_PIN 1
#define SCL_PIN 2

Adafruit_INA219 ina219;
// Create an INA219 instance
  float shuntVoltage = 0;
  float busVoltage = 0;
  float current_mA = 0;
  float power_mW = 0;
void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor to open
  pinMode(IC_ENABLE, OUTPUT);
  digitalWrite(IC_ENABLE, HIGH);
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize INA219 sensor
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip. Check wiring!");
    while (1); // Stop execution
  }
  Serial.println("INA219 initialized!");
}

void loop() {


  // Read shunt voltage (in volts)
  shuntVoltage = ina219.getShuntVoltage_mV() / 1000.0;
  // Read bus voltage (in volts)
  busVoltage = ina219.getBusVoltage_V();
  // Read current (in mA)
  current_mA = ina219.getCurrent_mA();
  // Read power (in mW)
  power_mW = ina219.getPower_mW();

  // Display readings
  Serial.print("Bus Voltage:   "); Serial.print(busVoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntVoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(1000); // Wait 1 second before next reading
}