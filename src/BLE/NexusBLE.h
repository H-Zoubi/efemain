#pragma once

#include "HardwareLayer/HardwareLayer.h"
#include <Arduino.h>

#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <ArduinoJson.h>

// Message tracking
struct ReceivedMessage
{
    String characteristic;
    String jsonData;
    String timestamp;
};

// void processConfigurationData(const String& characteristic, const String& data); // change or remove or move
class NexusBLE
{
  public:
    static void Init();
    static void sendRealTimeData(SensorData& sd);

    static void SetConnected(bool c);

  private:
    static void initBLE();

    // Timing for sensor data transmission
    static unsigned long lastSensorUpdate;
    static const unsigned long SENSOR_UPDATE_INTERVAL = 4999; // 5 seconds
};

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer)
    {
        NexusBLE::SetConnected(true);
        Serial.println("📱 Device connected - Starting sensor data transmission");
    };

    void onDisconnect(BLEServer* pServer)
    {
        NexusBLE::SetConnected(false);
        Serial.println("📱 Device disconnected - Stopping sensor data transmission");
    }
};

class ConfigCharacteristicCallbacks : public BLECharacteristicCallbacks
{
  private:
    const char* characteristicName;

  public:
    ConfigCharacteristicCallbacks(const char* name) : characteristicName(name)
    {
    }

    void onWrite(BLECharacteristic* pCharacteristic)
    {
        size_t length = pCharacteristic->getValue().length();

        if (length > 0 && length < 4096)
        {
            String data = String(pCharacteristic->getValue().c_str());
            String characteristic = String(characteristicName);

            Serial.printf("📡 Received %s configuration (%zu bytes)\n", characteristicName, length);
            // processConfigurationData(characteristic, data); // TODO: Add this function l8r
        }
    }
};
