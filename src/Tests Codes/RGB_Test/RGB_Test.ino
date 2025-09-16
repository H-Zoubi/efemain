/*
  ESP32-S3 RGB LED Controller

  
  Sequence:
  - 2 minutes: Green, blinking once every 10 seconds.
  - 30 seconds: Yellow, blinking every second.
  - 1 minute:  Red, blinking every second.
  - 5 minutes: Mixed colors, smoothly FADING through a rainbow sequence.
  The entire cycle then repeats.

  Hardware:
  - ESP32-S3
  - Common Cathode RGB LED

  Connections:
  - RED Pin   -> GPIO 17 (via transistor )
  - GREEN Pin -> GPIO 16 (via transistor )
  - BLUE Pin  -> GPIO 15 (via transistor )
  - CATHODE   -> GND


*by Khalil Adawi 11/9/2025

*/

const int RED_PIN = 17;
const int GREEN_PIN = 16;
const int BLUE_PIN = 15;


const int LED_ON = 255;
const int LED_OFF = 0;

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void turnOff() {
  setColor(0, 0, 0);
}


void setup() {

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

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
    setColor(LED_OFF, LED_ON, LED_OFF);
    delay(500);
    turnOff();
    delay(9500);
  }
}


void yellowPhase() {
  for (int i = 0; i < 30; i++) {
    setColor(LED_ON, LED_ON, LED_OFF);
    delay(500);
    turnOff();
    delay(500);
  }
}


void redPhase() {
  for (int i = 0; i < 60; i++) {
    setColor(LED_ON, LED_OFF, LED_OFF);
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