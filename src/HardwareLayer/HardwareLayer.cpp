#include "HardwareLayer.h"
#include <Adafruit_INA219.h>
#include <PinDefenitions.h>
#include <Wire.h>
#include <cstdint>

#include "RTCLib/RTClib.h"
#include "Wire.h"

const uint8_t LED_ON = 255;
const uint8_t LED_OFF = 0;

static void setColor(int red, int green, int blue)
{
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

#ifdef DEBUG
static void DBG_PrintPowerData(SensorData& pd)
{

    Serial.print("Bus Voltage:   ");
    Serial.print(pd.busVoltage);
    Serial.println(" V");
    Serial.print("Shunt Voltage: ");
    Serial.print(pd.shuntVoltage);
    Serial.println(" V");
    Serial.print("Current:       ");
    Serial.print(pd.current_mA);
    Serial.println(" mA");
    Serial.print("Power:         ");
    Serial.print(pd.power_mW);
    Serial.println(" mW");
    Serial.println("");
}
#endif // DEBUG

static Adafruit_INA219 ina219;
static RTC_DS1307 rtc;

void HardwareLayer::Init()
{
    pinMode(IC_ENABLE, OUTPUT);
    digitalWrite(IC_ENABLE, HIGH);
    Wire.begin(SDA_PIN, SCL_PIN);

    // ina==================================================================
    // Initialize INA219 sensor
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip. Check wiring!");
        while (1)
            ; // Stop execution
    }
    Serial.println("INA219 initialized!");

    // rtc==================================================================
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
    }

    // rgb leds=============================================================
}

SensorData HardwareLayer::GetSensorData()
{

    SensorData sd;
    sd.shuntVoltage = ina219.getShuntVoltage_mV() / 1000.0;
    // Read bus voltage (in volts)
    sd.busVoltage = ina219.getBusVoltage_V();
    // Read current (in mA)
    sd.current_mA = ina219.getCurrent_mA();
    // Read power (in mW)
    sd.power_mW = ina219.getPower_mW();

#ifdef DEBUG
    // DBG_PrintPowerData(sd); // temp
#endif // DEBUG

    return sd;
}

DateTime HardwareLayer::GetRTCTime()
{
    return rtc.now();
    // DateTime time = rtc.now();
}

void HardwareLayer::LEDSetColor(int red, int green, int blue)
{
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

void HardwareLayer::LEDTurnOff()
{
    LEDSetColor(LED_OFF, LED_OFF, LED_OFF);
}
