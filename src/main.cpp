#include <Arduino.h>

#include "esp32-hal.h"

#include "PinDefenitions.h"
#include "system/system.h"
#include <cstdlib>
// sd
#include "FS.h"
#include "SD.h"
#include "SPI.h"

System syst;
void setup()
{
    syst.Init();
}

void loop()
{
    syst.Update();

    delay(syst.GetSleepTime()); // change to a mopre mature value or change to deep sleep
};
