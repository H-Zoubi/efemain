/* Timestamp functions using a DS1307 RTC connected via I2C and Wire lib
** Created: 2015-06-01 by AxelTB
** Update: 2025-09-09 by Khalil Adawi
*/

#include "RTClib.h"
#include "Wire.h"
RTC_DS1307 rtc;

#define IC_ENABLE 21
#define SDA_PIN 1
#define SCL_PIN 2


void setup() {
  Serial.begin(115200);

#ifndef ESP8266
  while (!Serial)
    ;
#endif
  pinMode(IC_ENABLE, OUTPUT);
  digitalWrite(IC_ENABLE, HIGH);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");

  }

}

void loop() {
  DateTime time = rtc.now();

  //Full Timestamp
  Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + time.timestamp(DateTime::TIMESTAMP_FULL));

  //Date Only
  Serial.println(String("DateTime::TIMESTAMP_DATE:\t") + time.timestamp(DateTime::TIMESTAMP_DATE));

  //Full Timestamp
  Serial.println(String("DateTime::TIMESTAMP_TIME:\t") + time.timestamp(DateTime::TIMESTAMP_TIME));

  Serial.println("\n");

  //Delay 5s
  delay(5000);
}
