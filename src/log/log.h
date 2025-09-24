#pragma once

// SD
#include "FS.h"
#include "HardwareLayer/HardwareLayer.h"
#include "SD.h"
#include "SPI.h"

class Log
{
  public:
    static void Init(bool LogFile, bool LogSerial, const char* RootLogDirPath);

    static void DBG_LogError(const char* str);
    static void DBG_LogWarning(const char* str);
    static void DBG_LogInfo(const char* str);

    static void CheckForDataFile();
    static void LogData(SensorData* data);

  private:
    static void writeFile(const char* path, const char* message);
    static void appendFile(const char* path, const char* message);
    static void renameFile(const char* path1, const char* path2);
};
