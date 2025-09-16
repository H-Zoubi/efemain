#include "HardwareLayer.h"
#include <Adafruit_INA219.h>
#include <PinDefenitions.h>
#include <Wire.h>

#ifdef DEBUG
static void DBG_PrintPowerData(PowerData& pd)
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

void HardwareLayer::Init()
{
    pinMode(IC_ENABLE, OUTPUT);
    digitalWrite(IC_ENABLE, HIGH);
    Wire.begin(SDA_PIN, SCL_PIN);

    // Initialize INA219 sensor
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip. Check wiring!");
        while (1)
            ; // Stop execution
    }
    Serial.println("INA219 initialized!");
}

PowerData HardwareLayer::GetPowerMeasurements()
{

    PowerData pd;
    pd.shuntVoltage = ina219.getShuntVoltage_mV() / 1000.0;
    // Read bus voltage (in volts)
    pd.busVoltage = ina219.getBusVoltage_V();
    // Read current (in mA)
    pd.current_mA = ina219.getCurrent_mA();
    // Read power (in mW)
    pd.power_mW = ina219.getPower_mW();

#ifdef DEBUG
    DBG_PrintPowerData(pd); // temp
#endif                      // DEBUG

    return pd;
}
