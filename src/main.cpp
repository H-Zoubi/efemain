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
    // Display readings
    Log::DBG_LogInfo("Test Info");
    delay(500);
};
