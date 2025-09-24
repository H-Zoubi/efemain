#include "system.h"
#include "Arduino.h"
#include "esp32-hal.h"

#include <BLE/NexusBLE.h>
#include <HardwareLayer/HardwareLayer.h>
#include <log/log.h>

void System::Init()
{
    HardwareLayer::Init();
#ifdef DEBUG
    delay(3000); // for propper serial communication
#endif           // DEBUG

    Log::Init(true, true, "");
    NexusBLE::Init();
}
void System::Update()
{
    switch (m_State)
    {
    case SystemState::NONE:
        Log::DBG_LogError("State is None");
        SetState(SystemState::ERROR);
        return;
    case SystemState::SLEEP_LOOP:
        m_SleepTime = 60000; // set at 1min for saving to SD Card mode
        StateSleepLoop();
        return;
    case SystemState::BLE_WAKE:
        m_SleepTime = 100; // set at .1 sec for bluetooth communication
        StateBLEWake();
        return;
    case SystemState::ERROR:
    default:
        break;
    }
}

void System::StateSleepLoop()
{
    Log::DBG_LogInfo("Cycle");
    SensorData sensorData = HardwareLayer::GetSensorData();
    Log::CheckForDataFile();
    Log::LogData(sensorData);
}

void System::StateBLEWake()
{
    Log::DBG_LogInfo("BLE");
    SensorData sensorData = HardwareLayer::GetSensorData();

    SensorData dummyData;
    dummyData.busVoltage = HardwareLayer::readVolt();
    dummyData.temperature = HardwareLayer::readTemp();
    dummyData.humidity = HardwareLayer::readHum();
    dummyData.soilMoisture = HardwareLayer::readSoilMoisture();

    if (dummyData.temperature > 28 && dummyData.temperature < 30)
    {
        NexusBLE::SendNotification("ventemp02");
        HardwareLayer::LEDSetColor(255, 255, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
        delay(200);
        HardwareLayer::LEDSetColor(255, 255, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
    }
    else if (dummyData.temperature > 30)
    {
        NexusBLE::SendNotification("irrsm01");
        HardwareLayer::LEDSetColor(255, 0, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
        delay(200);
        HardwareLayer::LEDSetColor(255, 0, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
    }
    else
    {
        NexusBLE::SendNotification("cirtemp03");
        HardwareLayer::LEDSetColor(0, 255, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 255, 0);
        delay(200);
        HardwareLayer::LEDSetColor(0, 0, 0);
    }
    NexusBLE::sendRealTimeData(dummyData);

    // SensorData sensorData = HardwareLayer::GetSensorData();
    // NexusBLE::sendRealTimeData(sensorData);
}

void System::StateError()
{
}
