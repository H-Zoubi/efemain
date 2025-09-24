#include "system.h"
#include "Arduino.h"

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

    // SensorData dummyData;
    // dummyData.busVoltage = 1;
    // dummyData.temperature = 2;
    // dummyData.humidity = 3;
    // dummyData.soilMoisture = 8;
    // NexusBLE::sendRealTimeData(dummyData);

    SensorData sensorData = HardwareLayer::GetSensorData();
    NexusBLE::sendRealTimeData(sensorData);
}

void System::StateError()
{
}
