#pragma once

#include "HardwareSerial.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

namespace NexusServices
{
// config=========================================================================================
// |-INITIAL_CONFIG
// |-CROP_GROWTH_STAGE
// |-RELATIVE_HUMIDITY
// |-TEMPRETURE_REQUIREMENTS
// |-IRRIGATION_TIMING
// |-FARMING_NOTIFICATIONS_EN
// |-FARMING_NOTIFICATIONS_AR
#define CONFIG_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
BLEService* ConfigService;

#define INITIAL_CONFIG_CHARACTERISTIC_UUID "6E400101-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* InitialConfigCharacteristic;

#define CROP_GROWTH_STAGE_CHARACTERISTIC_UUID "6E400102-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* CropGrowthStageCharacteristic;

#define RELATIVE_HUMIDITY_CHARACTERISTIC_UUID "6E400104-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* RelativeHumidityCharacteristic;

#define TEMPRETURE_REQUIREMENTS_CHARACTERISTIC_UUID "6E400107-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* TempRequirementsCharacteristic;

#define IRIGATION_TIMING_CHARACTERISTIC_UUID "6E400103-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* IrigationTimingCharacteristic;

#define FARMING_NOTIFICATIONS_EN_CHARACTERISTIC_UUID "6E400106-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* FarmingNotificationsEnCharacteristic;

#define FARMING_NOTIFICATIONS_AR_CHARACTERISTIC_UUID "6E400105-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* FarmingNotificationsArCharacteristic;

// Notifications =================================================================================
// |-
#define NOTIFICATIONS_SERVICE_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
BLEService* NotificationsService;
#define NOTIFICATIONS_CHARACTERISTIC_UUID "6E400201-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* NotificationsCharacteristic;

// CROP_STATUS_SERICE ============================================================================
// |-GrowthStageUpdate
#define CROP_STATUS_SERVICE_UUID "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
BLEService* CropStatusService;
#define GROWTH_STAGE_UPDATE_CHARACTERISTIC_UUID "6E400301-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* GrowthStageUpdateCharacteristic;

// SensorDataService =============================================================================
// |-SMT100
// |-BME280.1
// |-BME280.2
#define SENSOR_DATA_SERVICE_UUID "6E400004-B5A3-F393-E0A9-E50E24DCCA9E"
BLEService* SensorDataService;

#define SMT100_MEASUREMENTS_CHARACTERISTIC_UUID "6E400401-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* SMT100_Characteristic;

#define BME280_01_MEASUREMENTS_CHARACTERISTIC_UUID "6E400402-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* bme280_1_Characteristic;

#define BME280_02_MEASUREMENTS_CHARACTERISTIC_UUID "6E400403-B5A3-F393-E0A9-E50E24DCCA9E"
BLECharacteristic* bme280_2_Characteristic;
// // |-
// #define _SERVICE_UUID ""
// BLEService* Service;
// #define _CHARACTERISTIC_UUID ""
// BLECharacteristic* Characteristic;

// BatteryService============================================================================
// |- BettryLevel
#define BATTERY_SERVICE_UUID "0000180F-0000-1000-8000-00805F9B34FB"
BLEService* BatteryService;
#define BATTERY_LEVEL_CHARACTERISTIC_UUID "00002A19-0000-1000-8000-00805F9B34FB"
BLECharacteristic* BatteryLevelCharacteristic;

// Hardware Errors============================================================================
// |- HardwareErrors
#define HARDWARE_ERROR_SERVICE_UUID "0000180F-0000-1000-8000-00805F9B34FB"
BLEService* HardwareErrorService;
#define HARDWARE_ERROR_CHARACTERISTIC_UUID "00002A19-0000-1000-8000-00805F9B34FB"
BLECharacteristic* HardwareErrorCharacteristic;

void InitAllServices(BLEServer* pServer)
{
    // Initial Configuration
    ConfigService = pServer->createService(CONFIG_SERVICE_UUID);

    InitialConfigCharacteristic =
        ConfigService->createCharacteristic(INITIAL_CONFIG_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    InitialConfigCharacteristic->setValue("init");

    GrowthStageUpdateCharacteristic =
        ConfigService->createCharacteristic(GROWTH_STAGE_UPDATE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    GrowthStageUpdateCharacteristic->setValue("germenation");

    RelativeHumidityCharacteristic =
        ConfigService->createCharacteristic(RELATIVE_HUMIDITY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    RelativeHumidityCharacteristic->setValue("30%");

    TempRequirementsCharacteristic = ConfigService->createCharacteristic(TEMPRETURE_REQUIREMENTS_CHARACTERISTIC_UUID,
                                                                         BLECharacteristic::PROPERTY_READ);
    TempRequirementsCharacteristic->setValue("12c");

    IrigationTimingCharacteristic =
        ConfigService->createCharacteristic(IRIGATION_TIMING_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    IrigationTimingCharacteristic->setValue("2hrs");

    FarmingNotificationsEnCharacteristic = ConfigService->createCharacteristic(
        FARMING_NOTIFICATIONS_EN_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    FarmingNotificationsEnCharacteristic->setValue("enable");

    FarmingNotificationsArCharacteristic = ConfigService->createCharacteristic(
        FARMING_NOTIFICATIONS_AR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    FarmingNotificationsArCharacteristic->setValue("disable");
    ConfigService->start();

    // notifications
    NotificationsService = pServer->createService(NOTIFICATIONS_SERVICE_UUID);
    NotificationsCharacteristic =
        NotificationsService->createCharacteristic(NOTIFICATIONS_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);

    NotificationsCharacteristic->setValue("NOTIFY:ERRORTEST");
    NotificationsService->start();

    // Sensors
    SensorDataService = pServer->createService(SENSOR_DATA_SERVICE_UUID);

    SMT100_Characteristic = SensorDataService->createCharacteristic(SMT100_MEASUREMENTS_CHARACTERISTIC_UUID,
                                                                    BLECharacteristic::PROPERTY_READ);
    SMT100_Characteristic->setValue("SMT:7");

    bme280_1_Characteristic = SensorDataService->createCharacteristic(BME280_01_MEASUREMENTS_CHARACTERISTIC_UUID,
                                                                      BLECharacteristic::PROPERTY_READ);
    bme280_1_Characteristic->setValue("BME1:9");

    bme280_2_Characteristic = SensorDataService->createCharacteristic(BME280_02_MEASUREMENTS_CHARACTERISTIC_UUID,
                                                                      BLECharacteristic::PROPERTY_READ);
    bme280_2_Characteristic->setValue("BME2:6");

    SensorDataService->start();

    // Battery
    BatteryService = pServer->createService(BATTERY_SERVICE_UUID);
    BatteryLevelCharacteristic =
        BatteryService->createCharacteristic(BATTERY_LEVEL_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);

    // BatteryLevelCharacteristic->setValue(HardwareLayer::GetBatteryLevel());
    BatteryLevelCharacteristic->setValue("BATLVL:56");
    BatteryService->start();

    // HardwareErrors
    HardwareErrorService = pServer->createService(HARDWARE_ERROR_SERVICE_UUID);
    HardwareErrorCharacteristic = HardwareErrorService->createCharacteristic(HARDWARE_ERROR_CHARACTERISTIC_UUID,
                                                                             BLECharacteristic::PROPERTY_READ);

    // BatteryLevelCharacteristic->setValue(HardwareLayer::GetBatteryLevel());
    HardwareErrorCharacteristic->setValue("ERR:43");
    HardwareErrorService->start();
}

} // namespace NexusServices
