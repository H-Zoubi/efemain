#pragma once

class Log
{
  public:
    static void Init(bool LogFile, bool LogSerial, const char* RootLogDirPath);

    static void DBG_LogError(const char* str);
    static void DBG_LogWarning(const char* str);
    static void DBG_LogInfo(const char* str);

  private:
};
