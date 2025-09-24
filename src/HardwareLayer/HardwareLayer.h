#pragma once

#include "RTCLib/RTClib.h"
#include <Arduino.h>

struct SensorData
{
    // INA
    float shuntVoltage = 0;
    float busVoltage = 0;
    float current_mA = 0;
    float power_mW = 0;

    // BME
    float temperature = 0;
    float humidity = 0;
    float soilMoisture = 0;
};

class HardwareLayer
{
  public:
    static void Init();
    static SensorData GetSensorData();
    static DateTime GetRTCTime();
    static void LEDSetColor(int red, int green, int blue);

  private:
    static float readSoilMoisture();
    static void enableTransmission();
    static void enableReception();
};
