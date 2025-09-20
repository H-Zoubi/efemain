#include "log.h"
#include "HardwareLayer/HardwareLayer.h"
#include "RTCLib/RTClib.h"
#include "WString.h"
#include <Arduino.h>
#include <PinDefenitions.h>
#include <cstdio>

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
    if (s_LogFile)
    {

        SPI.begin(sck, miso, mosi, cs);
        if (!SD.begin(cs))
        {
            Serial.println("Card Mount Failed");
            return;
        }
        // fs::FS& fs = SD; // check if cpy is ok? /insteadof &
        uint8_t cardType = SD.cardType();

        if (cardType == CARD_NONE)
        {
            Serial.println("No SD card attached"); // TODO:accomodate for no sd card (ERROR_MODE/STATE)
            return;
        }
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
        String messagae = HardwareLayer::GetRTCTime().timestamp(DateTime::TIMESTAMP_FULL) + " [ERROR] >> " + str;
        appendFile("/error.txt", messagae.c_str());
    }
}

void Log::DBG_LogWarning(const char* str)
{
    if (s_LogSerial)
    {
        Serial.print("[WARNING]: ");
        Serial.println(str);
    }
    if (s_LogFile)
    {
        String messagae = HardwareLayer::GetRTCTime().timestamp(DateTime::TIMESTAMP_FULL) + " [WARNING] >> " + str;
        appendFile("/warning.txt", messagae.c_str());
    }
}
void Log::DBG_LogInfo(const char* str)
{
    if (s_LogSerial)
    {
        Serial.print("[INFO]: ");
        Serial.println(str);
    }
    if (s_LogFile)
    {
        String messagae = HardwareLayer::GetRTCTime().timestamp(DateTime::TIMESTAMP_FULL) + " [INFO] >> " + str;
        appendFile("/info.txt", messagae.c_str());
    }
}
void Log::CheckAndPackageNewDay(uint8_t DayOnLastCheck)
{
    auto time = HardwareLayer::GetRTCTime();
    if (DayOnLastCheck == time.day())
        return;

    // new day detected!
    // must:
    // - rename (current.csv -> [DATE].csv)
    // - create new current.csv
    // - insert csv header

    String NewFileName = time.timestamp(DateTime::TIMESTAMP_DATE);
    renameFile("/current.csv", NewFileName.c_str());
    String Header = String() + "TIMESTAMP," + "SHUNTVOLTAGE," + "BUSVOLTAGE," + "CURRENT," + "POWER" +
                    "\n"; // Change Header when adding adding fields
    writeFile("/current.csv", Header.c_str());
}

// csv format:
// Time Stamp , shunt voltage , bus voltage, current, power, XX
void Log::LogDataToCurrent(SensorData& data)
{
    auto time = HardwareLayer::GetRTCTime();
    String csvFormattedData =
        String(time.timestamp(DateTime::TIMESTAMP_TIME)) + "," //
        + String(data.shuntVoltage) + ","                      //
        + String(data.busVoltage) + ","                        //
        + String(data.current_mA) + ","                        //
        + String(data.power_mW)                                //
                                // Add new Data entries here! (dont forget to change header too! @CheckAndPackageNewDay)
        + "\n"; // END

    appendFile("/current.csv", csvFormattedData.c_str()); // save file to disk
}

void Log::writeFile(const char* path, const char* message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = SD.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void Log::appendFile(const char* path, const char* message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void Log::renameFile(const char* path1, const char* path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (SD.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}
