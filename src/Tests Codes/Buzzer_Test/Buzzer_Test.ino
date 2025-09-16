//by Khalil Adawy 11/9/2025

#include "pitches.h" //for tunes

#define BUZZER_PIN 5
#define _5V_ENABLE 14


const unsigned long ALARM_DURATION = 20 * 1000UL;
const unsigned long TUNE_DURATION = 5 * 60 * 1000UL;

enum State {
  STATE_ALARM,
  STATE_TUNE
};

State currentState = STATE_ALARM;
unsigned long stateStartTime = 0;

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7, 0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0, NOTE_G6, 0, 0, 0
};

int noteDurations[] = {
  12, 12, 12, 12, 12, 12, 12, 12,
  12, 12, 12, 12, 12, 12, 12, 12
};

int tempo = 200;
int currentNoteIndex = 0;
unsigned long noteStartTime = 0;

void setup() {

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(_5V_ENABLE, OUTPUT);
  digitalWrite(_5V_ENABLE, HIGH);

  stateStartTime = millis();
}

void loop() {
  unsigned long currentTime = millis();

  switch (currentState) {
    case STATE_ALARM:
      tone(BUZZER_PIN, 2000);

      if (currentTime - stateStartTime >= ALARM_DURATION) {
        currentState = STATE_TUNE;
        stateStartTime = currentTime;
        currentNoteIndex = 0;
        noteStartTime = currentTime;
      }
      break;

    case STATE_TUNE:
      unsigned long timeInTuneState = currentTime - stateStartTime;
      
      if (timeInTuneState >= TUNE_DURATION) {
        noTone(BUZZER_PIN);
        currentState = STATE_ALARM;
        stateStartTime = currentTime;
      } else {
        int noteDuration = (60000 / tempo) / noteDurations[currentNoteIndex];
        
        if (currentTime - noteStartTime > noteDuration) {
          currentNoteIndex++;
          if (currentNoteIndex >= sizeof(melody) / sizeof(int)) {
            currentNoteIndex = 0;
          }

          int noteToPlay = melody[currentNoteIndex];
          if (noteToPlay == 0) {
            noTone(BUZZER_PIN);
          } else {
            tone(BUZZER_PIN, noteToPlay);
          }
          
          noteStartTime = currentTime;
        }
      }
      break;
  }
}