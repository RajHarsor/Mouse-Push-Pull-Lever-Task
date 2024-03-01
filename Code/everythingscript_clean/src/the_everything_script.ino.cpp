# 1 "C:\\Users\\rdh92\\AppData\\Local\\Temp\\tmpogsmtk96"
#include <Arduino.h>
# 1 "C:/Users/rdh92/Documents/GitHub/Mouse-Push-Pull-Lever-Task/Code/everythingscript_clean/src/the_everything_script.ino"

#include <Arduino.h>
#include <vector>
#include <string>
#include <map>
#include <FastLED.h>
#include "parse_scripts.h"
#include "light_configs.h"
#include "solenoid_open_time.h"
#include "hold_time_mod_visual.h"
#include "hold_time_visual_punishment.h"
#include "reaction_time_mod_visual.h"
#include "WN_punishment.h"
#include "punishment_time_visual_mod.h"
#include "isi_delay_mod_visual.h"
#include "cue_evoked_sound.h"




#define VRX_PIN A0
#define VRY_PIN A1
#define Solenoid 4
#define Puff 10
#define REST_PIN 33
#define PUSH_PIN 34
#define PULL_PIN 35




#define NUM_LEDS 48
#define BRIGHTNESS 5


#define DATA_PIN 8
#define CLOCK_PIN 14
#define NUM_LEDS 48

CRGB leds[NUM_LEDS];
# 60 "C:/Users/rdh92/Documents/GitHub/Mouse-Push-Pull-Lever-Task/Code/everythingscript_clean/src/the_everything_script.ino"
int totalTrials = 1000000;
int trialNumber = 1;
int decision;
int positionA = 0;
int positionB = 0;
int CorrectCounter = 0;
int SOpenTime1;




unsigned long currentMillis = 0;
int reactionTime;
#define timerMillis millis() - currentMillis
unsigned long startTime;
void setup();
void loop();
void printCueEvokedInformation(int isiDelay);
void printVisualInformation(int isiDelay);
void correctSOpen(int var);
void printLightDecision();
void printDecision();
void printCorrectStatus();
void printCorrectStatusMotor();
void printOpenTime();
#line 78 "C:/Users/rdh92/Documents/GitHub/Mouse-Push-Pull-Lever-Task/Code/everythingscript_clean/src/the_everything_script.ino"
void setup() {

  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(Solenoid, OUTPUT);
  pinMode(Puff, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(28, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(REST_PIN, INPUT);
  pinMode(PULL_PIN, INPUT);
  pinMode(PUSH_PIN, INPUT);
  randomSeed(analogRead(9));
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.println("What type of program would you like?");
  Serial.println("Type 1 for Visual Task");
  Serial.println("Type 2 for Motor Task");
  parseProgramTypeInput();
  Serial.print(programType);
  switch (programType) {
    case 1:
      Serial.println("Pick the Stage (1-4)");
      parseVisualStage();
      break;
    case 2:
      Serial.println("What type of Motor Task do you want to do?");
      Serial.println("Type 1 for Cue-Evoked Task (Preset)");
      Serial.println("Type 2 for Cue-Evoked Task (Custom)");
      Serial.println("Type 3 for Single Direction Optogenetic Task");
      parseMotorTaskType();
      break;
  }
}

void loop() {

    currentMillis = millis();
    digitalWrite(29, HIGH);
    if (programType == 1) {
      holdTimeVisualTask();
    }
    while (timerMillis <= holdTime) {
      if (digitalRead(REST_PIN) == HIGH) {
        continue;
      } else {
        if (programType == 1) {
          holdTimeVisualPunishment();
        }
        currentMillis = millis();
        continue;
      }
    }
    digitalWrite(29, LOW);
    digitalWrite(30, HIGH);
    if (programType == 1) {
      reactionTimeWindowModVisual();
      lightArrayModifier();
      turnOnTheLights();
      }
    if (programType == 2) {
      play12kHzSound(soundCueDuration);
    }
    currentMillis = millis();
    startTime = millis();
    while (timerMillis <= timeOutTime) {
      if (digitalRead(PUSH_PIN) == HIGH) {
          decision = 1;
          break;
        }
      if (digitalRead(PULL_PIN) == HIGH) {
          decision = 2;
          break;
        } else {
          decision = 0;
        }
    }
    reactionTime = millis() - startTime;
    lightsOff();
    digitalWrite(30, LOW);


    if (programType == 1) {
      switch (visualStage) {
        case 1:
          if (decision == 1) {
            positionA++;
            digitalWrite(31, HIGH);
            digitalWrite(Solenoid, HIGH);
            solenoidOpenTime();
            digitalWrite(Solenoid, LOW);
            digitalWrite(31, LOW);
            break;
          }
          if (decision == 2) {
            digitalWrite(31, HIGH);
            positionB++;
            digitalWrite(Solenoid, HIGH);
            solenoidOpenTime();
            digitalWrite(Solenoid, LOW);
            digitalWrite(31, LOW);
            break;
          }
        case 2:
          if (decision == 1 && lightDecision == 1) {
              correctSOpen(SOpenTime);
            } else if (decision == 2 && lightDecision == 2) {
              correctSOpen(SOpenTime);
            } else if ((decision == 1 || decision == 2) && lightDecision == 0) {
              correctSOpen(SOpenTime);
            } else {
              decision = 0;
            }
          break;
        case 3:
          if (decision == 1 && lightDecision == 1) {
              float ratio = (float)reactionTime / (float)timeOutTime;
              SOpenTime1 = -37.5 * (ratio) + 55.75;
              correctSOpen(SOpenTime1);
          } else if (decision == 2 && lightDecision == 2) {
              float ratio = (float)reactionTime / (float)timeOutTime;
              SOpenTime1 = -37.5 * (ratio) + 55.75;
              correctSOpen(SOpenTime1);
          } else if ((decision == 1 || decision == 2) && lightDecision == 0) {
              float ratio = (float)reactionTime / (float)timeOutTime;
              SOpenTime1 = -37.5 * (ratio) + 55.75;
              correctSOpen(SOpenTime1);
          } else {
            decision = 0;
          }
          break;
        case 4:
          if (decision == 1 && lightDecision == 1) {
            correctSOpen(SOpenTime);
          } else if (decision == 2 && lightDecision == 2) {
            correctSOpen(SOpenTime);
          } else if ((decision == 1 || decision == 2) && lightDecision == 0) {
            correctSOpen(SOpenTime);
          } else {
            decision = 0;
          }
          break;
      }
    }


    if (motorTaskType == 1) {
      switch (CueEvokedTaskDay) {
        case 1:
          if (decision == 1) {
            correctSOpen(SOpenTime);
          } else {
            decision == 0;
          }
          break;
        case 2:
          if (decision == 1) {
            correctSOpen(SOpenTime);
          } else {
            decision == 0;
          }
          break;
        case 3:
          if (decision == 1) {
            positionA++;
            digitalWrite(31, HIGH);
            digitalWrite(Solenoid, HIGH);
            solenoidOpenTime();
            digitalWrite(Solenoid, LOW);
            digitalWrite(31, LOW);
            break;
          }
          if (decision == 2) {
            digitalWrite(31, HIGH);
            positionB++;
            digitalWrite(Solenoid, HIGH);
            solenoidOpenTime();
            digitalWrite(Solenoid, LOW);
            digitalWrite(31, LOW);
            break;
          }
      }
    }


    if (programType == 1 && decision == 0) {
      punishmentVisualTaskChooser();
    }
    if (motorTaskType == 1 && decision == 0) {
      wnPunishment(punishmentTime);
    }
    digitalWrite(28, LOW);

    digitalWrite(32, HIGH);
    if (programType == 1) {
      isiDelayModVisual();
    }
    int isiDelay = random(isiDelayLowerRange, isiDelayUpperRange + 1);
    delay(isiDelay);
    digitalWrite(32, LOW);

    if (programType == 1) {
      printVisualInformation(isiDelay);
    }
    if (motorTaskType == 1) {
      printCueEvokedInformation(isiDelay);
    }
    trialNumber++;
}


void printCueEvokedInformation(int isiDelay) {
  switch (CueEvokedTaskDay) {
    case 1: case 2:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatusMotor();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(SOpenTime);
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" ; ");
      break;
    case 3:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatusMotor();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(OpenTime);
      Serial.print(" , ");
      Serial.print(positionA);
      Serial.print(" / ");
      Serial.print(positionB);
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" ; ");
      break;
  }
}




void printVisualInformation(int isiDelay) {
  switch (visualStage) {
    case 1:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printLightDecision();
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime();
      Serial.print(" , ");
      Serial.print(positionA);
      Serial.print(" / ");
      Serial.print(positionB);
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");

      Serial.print( " // ");
      Serial.print(" , ");
      Serial.print(holdTime);
      Serial.print(" , ");
      Serial.print(timeOutTime);
      Serial.print(" , ");
      Serial.print(punishmentTime);
      Serial.print(" , ");
      Serial.print(isiDelayLowerRange);
      Serial.print(" , ");
      Serial.print(isiDelayUpperRange);
      Serial.println(" ; ");
      break;
    case 2:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printLightDecision();
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime();
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");

      Serial.print( " // ");
      Serial.print(" , ");
      Serial.print(plusPercentage);
      Serial.print(" , ");
      Serial.print(lightsVerticalPercentage);
      Serial.print(" , ");
      Serial.print(lightsHorizontalPercentage);
      Serial.print(" , ");
      Serial.print(holdTime);
      Serial.print(" , ");
      Serial.print(timeOutTime);
      Serial.print(" , ");
      Serial.print(punishmentTime);
      Serial.print(" , ");
      Serial.print(isiDelayLowerRange);
      Serial.print(" , ");
      Serial.print(isiDelayUpperRange);
      Serial.println(" ; ");
      break;
    case 3:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printLightDecision();
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime();
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");

      Serial.print( " // ");
      Serial.print(" , ");
      Serial.print(plusPercentage);
      Serial.print(" , ");
      Serial.print(lightsVerticalPercentage);
      Serial.print(" , ");
      Serial.print(lightsHorizontalPercentage);
      Serial.print(" , ");
      Serial.print(holdTime);
      Serial.print(" , ");
      Serial.print(timeOutTime);
      Serial.print(" , ");
      Serial.print(punishmentTime);
      Serial.print(" , ");
      Serial.print(isiDelayLowerRange);
      Serial.print(" , ");
      Serial.print(isiDelayUpperRange);
      Serial.println(" ; ");
      break;
    case 4:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printLightDecision();
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus();
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime();
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");

      Serial.print( " // ");
      Serial.print(" , ");
      Serial.print(plusPercentage);
      Serial.print(" , ");
      Serial.print(lightsVerticalPercentage);
      Serial.print(" , ");
      Serial.print(lightsHorizontalPercentage);
      Serial.print(" , ");
      Serial.print(holdTime);
      Serial.print(" , ");
      Serial.print(timeOutTime);
      Serial.print(" , ");
      Serial.print(punishmentTime);
      Serial.print(" , ");
      Serial.print(isiDelayLowerRange);
      Serial.print(" , ");
      Serial.print(isiDelayUpperRange);
      Serial.println(" ; ");
      break;
  }
}

void correctSOpen(int var) {
  digitalWrite(31, HIGH);
  digitalWrite(Solenoid, HIGH);
  delay(var);
  digitalWrite(Solenoid, LOW);
  digitalWrite(31, LOW);
}


void printLightDecision() {
  if (lightDecision == 0){
    Serial.print("Plus");
  } else if (lightDecision == 1) {
    Serial.print("Vertical");
  } else if (lightDecision == 2) {
    Serial.print("Horizontal");
  }
}

void printDecision() {
  if (decision == 1) {
    Serial.print("Push");
  } else if (decision == 2) {
    Serial.print("Pull");
  } else {
    Serial.print("timedOut");
  }
}

void printCorrectStatus() {
  if (decision == 1 || decision == 2 && lightDecision == 0) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else if (decision == 1 && lightDecision == 1) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else if (decision == 2 && lightDecision == 2) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else {
    Serial.print("Incorrect");
    correct = false;
  }
}

void printCorrectStatusMotor() {
  if (decision == 1 || decision == 2) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else {
    Serial.print("Incorrect");
    correct = false;
  }
}
void printOpenTime() {
  switch (visualStage) {
    case 1:
      if (decision == 1 || decision == 2) {
        Serial.print(OpenTime);
      } else {
        Serial.print("0");
      }
      break;
    case 2:
      if (decision != 0) {
        Serial.print(SOpenTime);
      } else {
        Serial.print("0");
      }
      break;
    case 3:
      if (decision != 0) {
        Serial.print(SOpenTime1);
      } else {
        Serial.print("0");
      }
      break;
    default:
      if (decision != 0) {
        Serial.print(SOpenTime);
      } else {
        Serial.print("0");
      }
      break;
  }
}