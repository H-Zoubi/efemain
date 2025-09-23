
#include <Arduino.h>

#include "HardwareLayer/HardwareLayer.h"
#include <log/log.h>

#include "NexusBLEServices.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include <BLE2902.h>

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer)
    {
        Log::DBG_LogInfo("CONNECTED");
        // deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer)
    {
        Log::DBG_LogInfo("DISCONNECTED");
        // deviceConnected = false;
    }
};

void setup()
{
    // init
    Log::Init(true, true, "");
    delay(3000);
    // HardwareLayer::Init();
    Log::DBG_LogInfo("Test BLE Start");

    BLEDevice::init("Nexus-Board");
    BLEServer* pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // services
    NexusServices::InitAllServices(pServer);

    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(CONFIG_SERVICE_UUID);
    pAdvertising->addServiceUUID(NOTIFICATIONS_SERVICE_UUID);
    pAdvertising->addServiceUUID(CROP_STATUS_SERVICE_UUID);
    pAdvertising->addServiceUUID(SENSOR_DATA_SERVICE_UUID);
    pAdvertising->addServiceUUID(BATTERY_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {

    // Log::DBG_LogInfo("Cycle");
};
