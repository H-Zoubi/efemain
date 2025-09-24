#include "./NexusBLE.h"
#include "HardwareLayer/HardwareLayer.h"
#include "RTCLib/RTClib.h"

bool validateJSON(const String& jsonStr); // remove
#include "./base64.hpp"
// =============================================================================
// SERVICE UUIDs - MUST MATCH REACT NATIVE APP
// =============================================================================

// Configuration Service (for receiving crop setup)
#define CONFIGURATION_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define INITIAL_CONFIG_CHAR_UUID "6E400101-B5A3-F393-E0A9-E50E24DCCA9E"
#define CROP_GROWTH_STAGES_CHAR_UUID "6E400102-B5A3-F393-E0A9-E50E24DCCA9E"
#define IRRIGATION_TIMING_CHAR_UUID "6E400103-B5A3-F393-E0A9-E50E24DCCA9E"
#define RELATIVE_HUMIDITY_CHAR_UUID "6E400104-B5A3-F393-E0A9-E50E24DCCA9E"
#define TEMPERATURE_REQUIREMENTS_CHAR_UUID "6E400107-B5A3-F393-E0A9-E50E24DCCA9E"

// Real-time Sensor Data Service (for sending live data)
#define SENSOR_DATA_SERVICE_UUID "6E400004-B5A3-F393-E0A9-E50E24DCCA9E"
#define SMT100_MEASUREMENTS_CHAR_UUID "6E400401-B5A3-F393-E0A9-E50E24DCCA9E"
#define BME280_01_MEASUREMENTS_CHAR_UUID "6E400402-B5A3-F393-E0A9-E50E24DCCA9E"
#define BME280_02_MEASUREMENTS_CHAR_UUID "6E400403-B5A3-F393-E0A9-E50E24DCCA9E"

// Historical Data Service (for sending stored data)
#define SENSOR_DATA_SERVICE_PREV_UUID "6E400005-B5A3-F393-E0A9-E50E24DCCA9E"
#define SMT100_HISTORICAL_CHAR_UUID "6E400501-B5A3-F393-E0A9-E50E24DCCA9E"
#define BME280_01_HISTORICAL_CHAR_UUID "6E400502-B5A3-F393-E0A9-E50E24DCCA9E"
#define BME280_02_HISTORICAL_CHAR_UUID "6E400503-B5A3-F393-E0A9-E50E24DCCA9E"

// =============================================================================
// Static VARIABLES
// =============================================================================

// BLE Server components
static BLEServer* pServer = nullptr;
static BLEService* pConfigService = nullptr;
static BLEService* pSensorService = nullptr;
static BLEService* pHistoricalService = nullptr;

// BLE Characteristics for real-time data
static BLECharacteristic* pSMT100Char = nullptr;
static BLECharacteristic* pBME280_01Char = nullptr;
static BLECharacteristic* pBME280_02Char = nullptr;

// remove
ReceivedMessage configMessages[10];
int configMessageIndex = 0;
int totalConfigMessages = 0;
// Crop configuration storage // also remove
String currentCrop = "Unknown";
String currentFarmingType = "Unknown";
bool cropConfigured = false;

// Connection status // remove
static bool m_DeviceConnected;
static bool m_OldDeviceConnected;
static bool m_DataTransmissionEnabled;

void NexusBLE::SetConnected(bool c)
{
    m_DeviceConnected = c;
    m_DataTransmissionEnabled = c;
}
void NexusBLE::Init()
{
    m_DataTransmissionEnabled = false;
    m_DeviceConnected = false;
    m_OldDeviceConnected = false;

    Serial.println("🌱 Nexus ExpertFarmer Device Simulator .. for now");
    Serial.println("======================================");
    Serial.printf("💾 Initial free heap: %d bytes\n", ESP.getFreeHeap());
    // Initialize task watchdog
    // esp_task_wdt_init(30, true);
    // esp_task_wdt_add(NULL); // ??

    // Initialize message arrays
    for (int i = 0; i < 10; i++)
    {
        configMessages[i].characteristic = "";
        configMessages[i].jsonData = "";
        configMessages[i].timestamp = "";
    }

    initBLE();

    // esp_task_wdt_reset();
}
void NexusBLE::initBLE()
{
    Serial.println("📱 Setting up BLE services...");

    // Initialize BLE
    BLEDevice::init("Nexus-TEST");

    // Create BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create Configuration Service
    pConfigService = pServer->createService(CONFIGURATION_SERVICE_UUID);

    // Configuration characteristics (for receiving crop setup)
    BLECharacteristic* pInitialConfig = pConfigService->createCharacteristic(
        INITIAL_CONFIG_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pInitialConfig->setCallbacks(new ConfigCharacteristicCallbacks("INITIAL_CONFIG"));

    BLECharacteristic* pCropGrowthStages = pConfigService->createCharacteristic(
        CROP_GROWTH_STAGES_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pCropGrowthStages->setCallbacks(new ConfigCharacteristicCallbacks("CROP_GROWTH_STAGES"));

    BLECharacteristic* pRelativeHumidity = pConfigService->createCharacteristic(
        RELATIVE_HUMIDITY_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pRelativeHumidity->setCallbacks(new ConfigCharacteristicCallbacks("RELATIVE_HUMIDITY"));

    BLECharacteristic* pTemperatureRequirements = pConfigService->createCharacteristic(
        TEMPERATURE_REQUIREMENTS_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pTemperatureRequirements->setCallbacks(new ConfigCharacteristicCallbacks("TEMPERATURE_REQUIREMENTS"));

    BLECharacteristic* pIrrigationTiming = pConfigService->createCharacteristic(
        IRRIGATION_TIMING_CHAR_UUID, BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR);
    pIrrigationTiming->setCallbacks(new ConfigCharacteristicCallbacks("IRRIGATION_TIMING"));

    // Start configuration service
    pConfigService->start();

    // Create Real-time Sensor Data Service
    pSensorService = pServer->createService(SENSOR_DATA_SERVICE_UUID);

    // Real-time sensor characteristics (for sending live data)
    pSMT100Char =
        pSensorService->createCharacteristic(SMT100_MEASUREMENTS_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
    pSMT100Char->addDescriptor(new BLE2902());

    pBME280_01Char =
        pSensorService->createCharacteristic(BME280_01_MEASUREMENTS_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
    pBME280_01Char->addDescriptor(new BLE2902());

    pBME280_02Char =
        pSensorService->createCharacteristic(BME280_02_MEASUREMENTS_CHAR_UUID, BLECharacteristic::PROPERTY_NOTIFY);
    pBME280_02Char->addDescriptor(new BLE2902());

    // Start sensor service
    pSensorService->start();

    // Create Historical Data Service
    pHistoricalService = pServer->createService(SENSOR_DATA_SERVICE_PREV_UUID);

    // Historical data characteristics (for sending stored data)
    BLECharacteristic* pSMT100Historical =
        pHistoricalService->createCharacteristic(SMT100_HISTORICAL_CHAR_UUID, BLECharacteristic::PROPERTY_READ);

    BLECharacteristic* pBME280_01Historical =
        pHistoricalService->createCharacteristic(BME280_01_HISTORICAL_CHAR_UUID, BLECharacteristic::PROPERTY_READ);

    BLECharacteristic* pBME280_02Historical =
        pHistoricalService->createCharacteristic(BME280_02_HISTORICAL_CHAR_UUID, BLECharacteristic::PROPERTY_READ);

    // Start historical service
    pHistoricalService->start();

    // Start advertising
    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(CONFIGURATION_SERVICE_UUID);
    pAdvertising->addServiceUUID(SENSOR_DATA_SERVICE_UUID);
    pAdvertising->addServiceUUID(SENSOR_DATA_SERVICE_PREV_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}

// TODO: This function is used to recive the config from app on initail connect (l8r problem).
// void pzrocessConfigurationData(const String& characteristic, const String& data)
// {
//     Serial.println("🔄 Processing " + characteristic + "...");
//
//     String decodedData = data;
//
//     // Try to decode Base64 if needed
//     if (data.indexOf("{") == -1 && data.indexOf("}") == -1)
//     {
//         decodedData = decodeBase64(data);
//     }
//
//     Serial.println("📋 Decoded data length: " + String(decodedData.length()));
//
//     // Validate and process JSON
//     if (validateJSON(decodedData))
//     {
//         Serial.println("✅ JSON validation passed");
//
//         // Extract crop information from INITIAL_CONFIG
//         if (characteristic == "INITIAL_CONFIG")
//         {
//             JsonDocument doc;
//             deserializeJson(doc, decodedData);
//
//             if (doc["crop"].is<String>())
//             {
//                 currentCrop = doc["crop"].as<String>();
//                 Serial.println("🌱 Crop configured: " + currentCrop);
//             }
//
//             if (doc["farmingType"].is<String>())
//             {
//                 currentFarmingType = doc["farmingType"].as<String>();
//                 Serial.println("🚜 Farming type: " + currentFarmingType);
//             }
//
//             cropConfigured = true;
//         }
//
//         // Store the configuration message
//         configMessages[configMessageIndex].characteristic = characteristic;
//         configMessages[configMessageIndex].jsonData = decodedData;
//         configMessages[configMessageIndex].timestamp = String(millis() / 1000) + "s ago";
//
//         configMessageIndex = (configMessageIndex + 1) % 10;
//         totalConfigMessages++;
//
//         Serial.println("💾 Configuration stored successfully");
//     }
//     else
//     {
//         Serial.println("❌ Invalid JSON structure");
//     }
//
//     Serial.println("========================================");
// }

void NexusBLE::sendRealTimeData(SensorData& sd)
{
    if (!m_DeviceConnected || !m_DataTransmissionEnabled)
        return;

    // unsigned long currentTime = millis();
    String currentTime = HardwareLayer::GetRTCTime().timestamp(DateTime::TIMESTAMP_TIME);
    // Create and send SMT100 data
    JsonDocument smt100Doc;
    smt100Doc["timestamp"] = currentTime;
    smt100Doc["soil_moisture"] = sd.soilMoisture;

    String smt100Json;
    serializeJson(smt100Doc, smt100Json);
    String smt100Encoded = encodeBase64(smt100Json);

    pSMT100Char->setValue(smt100Encoded.c_str());
    pSMT100Char->notify();

    // Create and send BME280-01 data
    JsonDocument bme01Doc;
    bme01Doc["timestamp"] = currentTime;
    bme01Doc["temperature"] = sd.temperature;
    bme01Doc["humidity"] = sd.humidity;

    String bme01Json;
    serializeJson(bme01Doc, bme01Json);
    String bme01Encoded = encodeBase64(bme01Json);

    pBME280_01Char->setValue(bme01Encoded.c_str());
    pBME280_01Char->notify();

    // Create and send BME280-02 data
    JsonDocument bme02Doc;
    bme02Doc["timestamp"] = currentTime;
    bme02Doc["temperature"] = sd.temperature;
    bme02Doc["humidity"] = sd.humidity;

    String bme02Json;
    serializeJson(bme02Doc, bme02Json);
    String bme02Encoded = encodeBase64(bme02Json);

    pBME280_02Char->setValue(bme02Encoded.c_str());
    pBME280_02Char->notify();

    // Serial.printf("📊 Sensor data sent: Soil=%.1f%%, Temp1=%.1f°C, Temp2=%.1f°C\n", smt100_soilMoisture,
    // bme280_01_temp,
    // bme280_02_temp);
}

bool validateJSON(const String& jsonStr)
{
    JsonDocument testDoc;
    DeserializationError error = deserializeJson(testDoc, jsonStr);
    return !error;
}
