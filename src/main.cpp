#include <Arduino.h>

#include "HardwareLayer/HardwareLayer.h"
#include "esp32-hal.h"

#include "PinDefenitions.h"
#include <log/log.h>

void setup()
{
    Log::Init(true, true, "");
    delay(3000);
    Log::DBG_LogError("Test Error");
    Log::DBG_LogWarning("Test Warning");
    Log::DBG_LogInfo("Test Info");
    HardwareLayer::Init();
}

void loop()
{

    PowerData pd = HardwareLayer::GetPowerMeasurements();
    DateTime dt = HardwareLayer::GetRTCTime();
    // Full Timestamp
    Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + dt.timestamp(DateTime::TIMESTAMP_FULL));

    // Date Only
    Serial.println(String("DateTime::TIMESTAMP_DATE:\t") + dt.timestamp(DateTime::TIMESTAMP_DATE));

    // Full Timestamp
    Serial.println(String("DateTime::TIMESTAMP_TIME:\t") + dt.timestamp(DateTime::TIMESTAMP_TIME));

    Serial.println("\n");

    // Display readings
    Log::DBG_LogInfo("Test Info#2");
    delay(500);
};
