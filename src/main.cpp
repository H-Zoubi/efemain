#include <Arduino.h>

#include "esp32-hal.h"

#include "PinDefenitions.h"
#include "system/system.h"
#include <cstdlib>
// sd
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "log/log.h"
System syst;
void setup()
{
    syst.Init();
    syst.SetState(SystemState::SLEEP_LOOP); // for Saving Sensor Data To SDCard
    syst.SetState(SystemState::BLE_WAKE);   // for bluetooth communication
    pinMode(5, OUTPUT);
    digitalWrite(5, LOW);
}

void loop()
{
    syst.Update();
    delay(syst.GetSleepTime()); // change to a mopre mature value or change to deep sleep
    delay(1000);               // short delay to allow tasks to run
};
