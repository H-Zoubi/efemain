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
    HardwareLayer::Buzzer(false);
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
        m_SleepTime = 5 * 60000; // set at 5min for saving to SD Card mode
        StateSleepLoop();
        return;
    case SystemState::BLE_WAKE:
        m_SleepTime = 5000; // set at 5 sec for bluetooth communication
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
    SensorData* sensorData = HardwareLayer::GetSensorData();
    Log::CheckForDataFile();
    Log::LogData(sensorData);
}

void System::StateBLEWake()
{
    Log::DBG_LogInfo("BLE");
    // #ifdef DUMMYDATA
    //     SensorData dummyData;
    //     dummyData.busVoltage = 11111;
    //     dummyData.temperature = 22222;
    //     dummyData.humidity = 33333;
    //     dummyData.soilMoisture = 88888;
    //     NexusBLE::sendRealTimeData(dummyData);
    // #else
    // #endif // !DUMMYDATA
    SensorData* sensorData = HardwareLayer::GetSensorData();
    NexusBLE::sendRealTimeData(*sensorData);
    delete sensorData;
}

void System::StateError()
{
}
