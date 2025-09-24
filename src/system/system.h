#pragma once

#include "BLE/NexusBLE.h"
#include "log/log.h"
#include <cstdint>
enum class SystemState
{
    NONE = 0,
    SLEEP_LOOP,
    BLE_WAKE,
    ERROR,
};
// const int FiveMinutes = 1000 * 60 * 5; // in ms
// const int FiveSeconds = 1000 * 5; // in ms
class System
{
  public:
    // System() : m_SleepTime(FiveSeconds) {};
    System() {};
    ~System() {};

    void Init();
    void Update();

    inline void SetState(SystemState state)
    {
        m_State = state;
    };
    inline SystemState GetState() const
    {
        return m_State;
    };

    inline int GetSleepTime()
    {
        // wrote a getter so that we can change sleep time in the future based on state or situation eg: lower retry
        // time on ERROR
        return m_SleepTime;
    }

  private:
    void StateSleepLoop();
    void StateBLEWake();
    void StateError();

  private:
    SystemState m_State;
    uint8_t m_CurrentDay = 0; // current day of the week // for detecting new a day // Range:(1-31)
    int m_SleepTime = 5000;   // default sleep time at 5 sec (too little, better change in constructor)
};
