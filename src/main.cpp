#include <Arduino.h>
#include <log/log.h>

void setup()
{
    Log::Init(true, true, "");
    Log::DBG_LogError("Test Error");
    Log::DBG_LogWarning("Test Warning");
    Log::DBG_LogInfo("Test Info");
}
