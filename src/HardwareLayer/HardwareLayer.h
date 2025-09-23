#pragma once

#include "RTCLib/RTClib.h"
#include <Arduino.h>

struct PowerData
{
    float shuntVoltage = 0;
    float busVoltage = 0;
    float current_mA = 0;
    float power_mW = 0;
};

class HardwareLayer
{
  public:
    static void Init();
    static PowerData GetPowerMeasurements();
    static DateTime GetRTCTime();
    static int GetBatteryLevel();
    static void SetLED(bool r, bool g, bool b);
};
