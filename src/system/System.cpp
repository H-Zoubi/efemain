#include "system.h"
#include "Arduino.h"

#include <HardwareLayer/HardwareLayer.h>
#include <log/log.h>

void System::Init()
{
    HardwareLayer::Init();
#ifdef DEBUG
    delay(3000);
#endif // DEBUG

    Log::Init(true, true, "");
    // Log::DBG_LogError("Test Error");
    // Log::DBG_LogWarning("Test Warning");
    // Log::DBG_LogInfo("Test Info");

    // SensorData sd = HardwareLayer::GetSensorData();
    // DateTime dt = HardwareLayer::GetRTCTime();
}
void System::Update()
{
    // GetSensorData
    // check for new day (package)
    // append data // GetTime
    // loop

    switch (m_State)
    {
    case SystemState::NONE:
        SetState(SystemState::ERROR);
    case SystemState::SLEEP_LOOP:
        StateSleepLoop();
    case SystemState::BLE_WAKE:
    case SystemState::ERROR:
    default:
        break;
    }
}

void System::StateSleepLoop()
{

    SensorData sensorData = HardwareLayer::GetSensorData();
    Log::CheckAndPackageNewDay(m_CurrentDay);
    Log::LogDataToCurrent(sensorData);
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
