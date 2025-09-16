#include "log.h"
#include "HardwareSerial.h"
#include <Arduino.h>

bool s_LogFile;
bool s_LogSerial;

void Log::Init(bool LogFile, bool LogSerial, const char* RootLogDirPath)
{
    s_LogFile = LogFile;
    s_LogSerial = LogSerial;
    if (s_LogSerial)
    {
        Serial.begin(115200); // 115200 baud const for all esp boards
        while (!Serial)
            delay(10);
    }
}

void Log::DBG_LogError(const char* str)
{
    if (s_LogSerial)
    {
        Serial.print("[ERROR]: ");
        Serial.println(str);
    }
    if (s_LogFile)
    {
    }
}

void Log::DBG_LogWarning(const char* str)
{
    if (s_LogSerial)
    {
        Serial.print("[WARNING]: ");
        Serial.println(str);
    }
}
void Log::DBG_LogInfo(const char* str)
{
    if (s_LogSerial)
    {
        Serial.print("[INFO]: ");
        Serial.println(str);
    }
}
