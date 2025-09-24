#include "system.h"
#include "Arduino.h"

#include <BLE/NexusBLE.h>
#include <HardwareLayer/HardwareLayer.h>
#include <log/log.h>

void System::Init()
{
    HardwareLayer::Init();
#ifdef DEBUG
    delay(3000);
#endif // DEBUG

    Log::Init(true, true, "");
    NexusBLE::Init();
    // SensorData sd = HardwareLayer::GetSensorData();
    // DateTime dt = HardwareLayer::GetRTCTime();
}
void System::Update()
{
    switch (m_State)
    {
    case SystemState::NONE:
        Log::DBG_LogError("State Is None");
        SetState(SystemState::ERROR);
        return;
    case SystemState::SLEEP_LOOP:
        StateSleepLoop();
        return;
    case SystemState::BLE_WAKE:
        StateBLEWake();
        return;
    case SystemState::ERROR:
    default:
        break;
    }
    Log::DBG_LogInfo("3");
}

void System::StateSleepLoop()
{

    SensorData sensorData = HardwareLayer::GetSensorData();
    Log::CheckForDataFile();
    Log::LogData(sensorData);
    Log::DBG_LogInfo("Cycle.");
}

void System::StateBLEWake()
{
    SensorData sensorData = HardwareLayer::GetSensorData();
    Log::DBG_LogInfo("BLE");
    NexusBLE::sendRealTimeData();
}

void System::StateError()
{
}
// Full Timestamp
// Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + dt.timestamp(DateTime::TIMESTAMP_FULL));

// Date Only
// Serial.println(String("DateTime::TIMESTAMP_DATE:\t") + dt.timestamp(DateTime::TIMESTAMP_DATE));

// Full Timestamp
// Serial.println(String("DateTime::TIMESTAMP_TIME:\t") + dt.timestamp(DateTime::TIMESTAMP_TIME));

// Serial.println("\n");

// Display readings
