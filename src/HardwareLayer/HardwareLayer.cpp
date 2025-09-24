#include "HardwareLayer.h"
#include <Adafruit_INA219.h>
#include <PinDefenitions.h>
#include <Wire.h>
#include <cstdint>

#include "Arduino.h"
#include "RTCLib/RTClib.h"
#include "Wire.h"
#include "esp32-hal-gpio.h"

const uint8_t LED_ON = 255;
const uint8_t LED_OFF = 0;

#include <Adafruit_BME280.h>
static Adafruit_BME280 bme; // I2C
static Adafruit_INA219 ina219;
static RTC_DS1307 rtc;

// SMT
static byte s_Message[] = {0xFD, 0x03, 0x00, 0x01, 0x00, 0x01, 0xC1, 0xF6}; // Master
static HardwareSerial s_Serial2(2);
static int s_Data[8];

static void setColor(int red, int green, int blue)
{
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

#ifdef DEBUG
static void DBG_PrintSensorData(SensorData& pd)
{

    Serial.print("Bus Voltage:   ");
    Serial.print(pd.busVoltage);
    Serial.println(" V");
    Serial.print("Shunt Voltage: ");
    Serial.print(pd.shuntVoltage);
    Serial.println(" V");
    Serial.print("Current:       ");
    Serial.print(pd.current_mA);
    Serial.println(" mA");
    Serial.print("Power:         ");
    Serial.print(pd.power_mW);
    Serial.println(" mW");
    Serial.print("temperature:         ");
    Serial.print(pd.temperature);
    Serial.println(" c");
    Serial.print("humidity:         ");
    Serial.print(pd.humidity);
    Serial.println(" %");
    Serial.print("soilMoisture:         ");
    Serial.print(pd.soilMoisture);
    Serial.println(" ");
    Serial.println("");
}
#endif // DEBUG

void HardwareLayer::Init()
{
    pinMode(IC_ENABLE, OUTPUT);
    digitalWrite(IC_ENABLE, HIGH);
    Wire.begin(SDA_PIN, SCL_PIN);

    // bme ================================================================
    bme.begin(0x76);

    // ina==================================================================
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip. Check wiring!");
        while (1)
            ; // Stop execution
    }
    Serial.println("INA219 initialized!");

    // rtc==================================================================
    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1)
            delay(10);
    }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    if (!rtc.isrunning())
    {
        Serial.println("RTC is NOT running, let's set the time!");
    }

    // rgb leds=============================================================

    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);

    // SMT =================================================================
    s_Serial2.begin(BAUD, SERIAL_8E1, RXD2, TXD2);
    pinMode(14, OUTPUT);

    digitalWrite(14, HIGH);

    // Initialize control pins
    pinMode(RE_PIN, OUTPUT);
    pinMode(DE_PIN, OUTPUT);
    enableTransmission();
    delay(10);

    Serial.println("-- SMT Starting... --");
    // buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(_5V_ENABLE, HIGH);
}

SensorData HardwareLayer::GetSensorData()
{

    SensorData sd;
    sd.shuntVoltage = ina219.getShuntVoltage_mV() / 1000.0;
    // Read bus voltage (in volts)
    sd.busVoltage = ina219.getBusVoltage_V();
    sd.current_mA = ina219.getCurrent_mA();
    sd.power_mW = ina219.getPower_mW();
    sd.temperature = bme.readTemperature();
    sd.humidity = bme.readHumidity();
    sd.soilMoisture = readSoilMoisture();
#ifdef DEBUG
    DBG_PrintSensorData(sd);
#endif // DEBUG

    return sd;
}

DateTime HardwareLayer::GetRTCTime()
{
    return rtc.now();
    // DateTime time = rtc.now();
}

void HardwareLayer::LEDSetColor(int red, int green, int blue)
{
    analogWrite(RED_PIN, red);
    analogWrite(GREEN_PIN, green);
    analogWrite(BLUE_PIN, blue);
}

void HardwareLayer::Buzzer(bool b)
{
    digitalWrite(BUZZER_PIN, b);
}
int HardwareLayer::GetBatteryPercentage(SensorData& sd)
{
    return map((long)(sd.busVoltage * 10), 25, 42, 0, 100);
}

void HardwareLayer::enableTransmission()
{
    digitalWrite(DE_PIN, HIGH); // Enable driver
    digitalWrite(RE_PIN, HIGH); // Disable receiver
    delayMicroseconds(10);      // Small delay for transceiver switching
}

// Enable reception mode
void HardwareLayer::enableReception()
{
    digitalWrite(DE_PIN, LOW); // Disable driver
    digitalWrite(RE_PIN, LOW); // Enable receiver
    delayMicroseconds(10);     // Small delay for transceiver switching
}
static long combineHexValues(int decStr1, int decStr2)
{
    // 1. Convert the input decimal strings into actual integers.
    int num1 = decStr2; // "112" becomes integer 112
    int num2 = decStr1; // "13"  becomes integer 13

    // 2. Create a buffer to hold the combined hexadecimal string.
    // We need 4 characters for hex digits and 1 for the null terminator.
    char combinedHexBuffer[5];

    // 3. Format the two integers into a single hexadecimal string.
    //    - %02X prints an integer as a 2-digit, uppercase hex number (padding with a '0' if needed).
    //    - num2 is placed first to become the high byte, as in your "0d70" example.
    sprintf(combinedHexBuffer, "%02X%02X", num2, num1); // Creates the string "0D70"

    // 4. Convert the combined hex string back to a decimal number.
    //    strtol converts a string to a long, with 16 specifying the base (hexadecimal).
    long finalDecimalValue = strtol(combinedHexBuffer, NULL, 16);

    // 5. Return the final result.
    return finalDecimalValue;
}

float HardwareLayer::readSoilMoisture()
{
    bool flage = false;

    if (true)
    {
        enableTransmission();
        delay(10);
        s_Serial2.write(s_Message[0]);
        delay(3);
        s_Serial2.write(s_Message[1]);
        delay(3);
        s_Serial2.write(s_Message[2]);
        delay(3);
        s_Serial2.write(s_Message[3]);
        delay(3);
        s_Serial2.write(s_Message[4]);
        delay(3);
        s_Serial2.write(s_Message[5]);
        delay(3);
        s_Serial2.write(s_Message[6]);
        delay(3);
        s_Serial2.write(s_Message[7]);
        delay(3);
        // serial2.println();

        flage = false;
    }
    enableReception();
    int i = 0;
    while (s_Serial2.available() > 0)
    {
        s_Data[i] = s_Serial2.read();
        Serial.println(s_Data[i], HEX);
        flage = true;
        i++;
    }

    float soilMoisture = (float)combineHexValues(s_Data[3], s_Data[4]) / 100.0f;

    delay(100);

    return soilMoisture;
}
