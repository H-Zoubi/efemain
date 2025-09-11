/*!
 * @file readVoltage.ino
 * @brief connect ADS1115 I2C interface with your board (please reference board compatibility)
 * @n The voltage value read by A0 A1 A2 A3 is printed through the serial port.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [luoyufeng](yufeng.luo@dfrobot.com),
 * @version  V1.0
 * @date  2019-06-19
 * @url https://github.com/DFRobot/DFRobot_ADS1115
 *
 * Code Updated by: Khalil Adawi
 * 10/9/2025
 */


#include <Wire.h>
#include <DFRobot_ADS1115.h>

#define IC_ENABLE 21
#define SDA_PIN 1
#define SCL_PIN 2

DFRobot_ADS1115 ads(&Wire);

void setup(void) {
  Serial.begin(115200);
  pinMode(IC_ENABLE, OUTPUT);
  digitalWrite(IC_ENABLE, HIGH);
  Wire.begin(SDA_PIN, SCL_PIN);
  ads.setAddr_ADS1115(0X49);      // 0x48
  ads.setGain(eGAIN_TWOTHIRDS);   // 2/3x gain
  ads.setMode(eMODE_SINGLE);      // single-shot mode
  ads.setRate(eRATE_128);         // 128SPS (default)
  ads.setOSMode(eOSMODE_SINGLE);  // Set to start a single-conversion
  ads.init();

}

void loop(void) {
  if (ads.checkADS1115()) {
    int16_t adc0, adc1, adc2, adc3;
    adc0 = ads.readVoltage(0);
    Serial.print("A0:");
    Serial.print(adc0);
    Serial.print("mV,  ");
    adc1 = ads.readVoltage(1);
    Serial.print("A1:");
    Serial.print(adc1);
    Serial.print("mV,  ");
    adc2 = ads.readVoltage(2);
    Serial.print("A2:");
    Serial.print(adc2);
    Serial.print("mV,  ");
    adc3 = ads.readVoltage(3);
    Serial.print("A3:");
    Serial.print(adc3);
    Serial.println("mV");
  } else {
    Serial.println("ADS1115 Disconnected!");
  }

  delay(1000);
}
