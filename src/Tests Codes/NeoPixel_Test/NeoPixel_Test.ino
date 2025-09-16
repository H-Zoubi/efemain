/*
  ESP32-S3 NeoPixel Sequence Controller

  Sequence:
  - 2 minutes: Green, blinking once every 10 seconds.
  - 30 seconds: Yellow, blinking every second.
  - 1 minute:  Red, blinking every second.
  - 5 minutes: Mixed colors, smoothly FADING through a rainbow sequence.
  The entire cycle then repeats.

  Hardware:
  - 1x NeoPixel

  Connections:
  - NeoPixel 5V/VCC -> ESP32 5V
  - NeoPixel GND    -> ESP32 GND
  - NeoPixel RGB_DI -> ESP32 GPIO 4
*/

#include <Adafruit_NeoPixel.h>

const int NEOPIXEL_PIN = 4;
const int NUM_PIXELS = 1;

Adafruit_NeoPixel pixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setColor(uint8_t r, uint8_t g, uint8_t b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}

void turnOff() {
  pixel.clear();
  pixel.show();
}

void setup() {
  pixel.begin();
  pixel.setBrightness(80);
  turnOff();
}

void loop() {
  greenPhase();
  yellowPhase();
  redPhase();
  mixingColorsPhase();
}

void greenPhase() {
  for (int i = 0; i < 12; i++) {
    setColor(0, 255, 0);
    delay(500);
    turnOff();
    delay(9500);
  }
}

void yellowPhase() {
  for (int i = 0; i < 30; i++) {
    setColor(255, 255, 0);
    delay(500);
    turnOff();
    delay(500);
  }
}

void redPhase() {
  for (int i = 0; i < 60; i++) {
    setColor(255, 0, 0);
    delay(500);
    turnOff();
    delay(500);
  }
}

void mixingColorsPhase() {
  
  unsigned long startTime = millis();
  const unsigned long duration = 5 * 60 * 1000;
  int fadeDelay = 10;

  while (millis() - startTime < duration) {
    for (int g = 0; g <= 255; g++) {
      if (millis() - startTime >= duration) break;
      setColor(255, g, 0);
      delay(fadeDelay);
    }
    if (millis() - startTime >= duration) break;

    for (int r = 255; r >= 0; r--) {
      if (millis() - startTime >= duration) break;
      setColor(r, 255, 0);
      delay(fadeDelay);
    }
    if (millis() - startTime >= duration) break;

    for (int b = 0; b <= 255; b++) {
      if (millis() - startTime >= duration) break;
      setColor(0, 255, b);
      delay(fadeDelay);
    }
    if (millis() - startTime >= duration) break;

    for (int g = 255; g >= 0; g--) {
      if (millis() - startTime >= duration) break;
      setColor(0, g, 255);
      delay(fadeDelay);
    }
    if (millis() - startTime >= duration) break;
    
    for (int r = 0; r <= 255; r++) {
      if (millis() - startTime >= duration) break;
      setColor(r, 0, 255);
      delay(fadeDelay);
    }
    if (millis() - startTime >= duration) break;

    for (int b = 255; b >= 0; b--) {
      if (millis() - startTime >= duration) break;
      setColor(255, 0, b);
      delay(fadeDelay);
    }
  }

  turnOff();
}
