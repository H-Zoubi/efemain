/*
  Single sensor test

  In this example an esp32S3 is connected to a ttl to RS485 converter module (THVD1400). 
  On the RS485 a Trubner's SMT100 soil moisture sensor is connected. SerialPort(2)
  is used to view .

  Created 18/3/2021
  By Ghaith Alshishani

  Updated 9/9/2025
  By Khalil Adawi

*/

#include "SMT100_MODBUS.h"

#include <HardwareSerial.h>

#define RE_PIN 38
#define DE_PIN 39

#define RX_PIN 40
#define TX_PIN 41

#define IC_ENABLE 21
#define VCC_ENABLE 14


HardwareSerial SerialPort(2);  // use UART2

SMT100_MODBUS smt100(&SerialPort, RE_PIN, DE_PIN);  // create an SMT100_MODBUS on Serial port (i.e: 0(RX),1(TX) pins or hardware Serial1)

void setup() {
  // initialize serial transmission with even parity
  SerialPort.begin(9600, SERIAL_8E1, RX_PIN, TX_PIN);
  // initialize serial transmission using softwareSerial
  Serial.begin(115200);

  pinMode(IC_ENABLE, OUTPUT);
  pinMode(VCC_ENABLE, OUTPUT);

  digitalWrite(IC_ENABLE, HIGH);
  digitalWrite(VCC_ENABLE, HIGH);
}

void loop() {
  // ReadAddress read and retuns the ID address of the SMT100 sensor on the RS485 MODBUS. There should only be one SMT100 sensor connected to the bus at a time because this function uses the broadcast ID address
  int readaddress = smt100.ReadAddress();
  Serial.print("readaddress=" + String(readaddress));
  delay(10);
  // SetAddress returns a boolean to determine succeful operation. This function Sets the ID address of the SMT100 sensor on the RS485 MODBUS. There should only be one SMT100 sensor connected to the bus at a time because this function uses the broadcast ID address
  bool setaddress = smt100.SetAddress(0);
  Serial.print("  setaddress=" + String(setaddress));
  delay(10);
  // CheckAddress returns a boolean and checks if the Sensor ID is present on RS485 MODBUS
  bool checkaddress = smt100.CheckAddress(0);
  Serial.print("  checkaddress=" + String(checkaddress));
  delay(10);
  // reads the data registers from SMT100 sensors with a specified id. returns (-255) when wrong id is passed (-254) when wrong address is passed (-253 & -252) when no correct bytes were recieved
  int addressID = smt100.ReadSMT100(0x00, ADDRESS_REG);
  Serial.print("  addressID=" + String(addressID));
  delay(10);
  float temp = smt100.ReadSMT100(0x00, TEMP_REG);
  Serial.print("  temp=" + String(temp));
  delay(10);
  float waterContent = smt100.ReadSMT100(0x00, WATER_CONTENT_REG);
  Serial.print("  waterContent=" + String(waterContent));
  delay(10);
  float permittivity = smt100.ReadSMT100(0x00, PERMITTIVITY_REG);
  Serial.print("  permittivity=" + String(permittivity));
  delay(10);
  // ChangeAddress changes the id address of a given SMT100 id address if the new address is not already taken on the RS485 MODBUS
  bool changeaddress = smt100.ChangeAddress(0x00, 11);
  Serial.print("  changeaddress=" + String(changeaddress));
  delay(10);
  int counts = smt100.ReadSMT100(0x0B, COUNTS_REG);
  Serial.print("  counts=" + String(counts));
  Serial.println("");
  delay(10);
}