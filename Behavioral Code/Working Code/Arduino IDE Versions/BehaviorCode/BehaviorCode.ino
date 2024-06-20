/* #region DEPENDENCIES*/
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

/* #endregion */

/* #region PIN SETUPS */
#define VRX_PIN A0   // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1   // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4   // Water Solenoid will be connected to Digital Pin 4
#define Puff 10      // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5
#define REST_PIN 33  // Rest Pin will be connected to Digital Pin 33 on Behavior Teensy and Coordinate Teensy
#define PUSH_PIN 34  // Push Pin will be connected to Digital Pin 34 on Behavior Teensy and Coordinate Teensy
#define PULL_PIN 35  // Pull Pin will be connected to Digital Pin 35 on Behavior Teensy and Coordinate Teensy
/* #endregion */

/* #region LED Array Set Up*/
//Parameters
#define NUM_LEDS 48  //number of LEDs total
#define BRIGHTNESS 5

// Set up
#define DATA_PIN 8
#define CLOCK_PIN 14
//#define NUM_LEDS 48  //number of LEDs total
#define NUM_LEDS 64 //for the 8x8 matrix

CRGB leds[NUM_LEDS];  //define the array of LEDs

/* #endregion */

/* #region BEHAVIOR VARIBALES CONST */
int totalTrials = 1000000;
int trialNumber = 1;
int decision;
int positionA = 0;
int positionB = 0;
int CorrectCounter = 0;
int SOpenTime1;
int lightDecision1;

/* #endregion */

/* #region Timer Variables */
unsigned long currentMillis = 0;
int reactionTime;
#define timerMillis millis() - currentMillis
unsigned long startTime;

/* #endregion */

void setup() {
  delay(2000); // 2 second delay for the serial monitor to open
  /* #region Pins + Serial */
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(Solenoid, OUTPUT);
  pinMode(Puff, OUTPUT);      // Sets the puff pin as an output
  pinMode(29, OUTPUT);        // Sets pin 29 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the rest state
  pinMode(30, OUTPUT);        // Sets pin 30 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the push/pull or push/down state
  pinMode(31, OUTPUT);        // Sets pin 31 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the solenoid is open
  pinMode(32, OUTPUT);        // Sets pin 32 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the ISI delay is occurring
  pinMode(28, OUTPUT);        // Set pin 28 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is punishment
  pinMode(25, OUTPUT);
  pinMode(REST_PIN, INPUT);                             // Sets the rest pin as an input, if the mouse is in the rest coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
  pinMode(PULL_PIN, INPUT);                             // Sets the pull pin as an input, if the mouse is in the pull coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
  pinMode(PUSH_PIN, INPUT);                             // Sets the push pin as an input, if the mouse is in the push coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy                          // Probably useless
  randomSeed(analogRead(9));                            // Needed for random number generator for ISI Delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // Sets up the light array
  FastLED.setBrightness(BRIGHTNESS);                    // Sets the brightness of the light array
  /* #endregion */
  Serial.println("What Stage would you like to do?");
  Serial.println("Type 1 for Task Design A or Task Design B Phase 1");
  Serial.println("Type 2 for Task Design B Phase 2");
  parseVisualStage();
  Serial.println("You Chose Visual Stage: ");
  Serial.println(visualStage);
  Serial.println("Task Starting...");
  delay(2000);
}

void loop() {
    // Rest position block
    currentMillis = millis(); // Start the hold time timer
    digitalWrite(29, HIGH); // Tells the coordinate Teensy that the current state is the rest state
    while (timerMillis <= holdTime) { // hold time timer
      if (digitalRead(REST_PIN) == HIGH) {
        continue;
      } else {
        currentMillis = millis();
        continue;
      }
    }
    digitalWrite(29, LOW);   // Tells the coordinate Teensy that the current state is not the rest state anymore
    digitalWrite(30, HIGH);  // Tells the coordinate Teensy that the current state is the push/pull or push/down state
    turnOnTheLights(); // turn on the appropriate light array
    lightDecision1 = lightDecision; // save the light decision for the decision block
    if (digitalRead(REST_PIN) != HIGH) { // Catch if the mouse leaves the rest position before the light array is turned on, prevents 0 RTs
      loop();
    }
    currentMillis = millis(); // start the reaction time window timer
    startTime = millis(); // start the reaction time timer
    while (timerMillis <= timeOutTime) {
      if (digitalRead(PUSH_PIN) == HIGH) {
          decision = 1; // push
          break;
        }
      if (digitalRead(PULL_PIN) == HIGH) {
          decision = 2; // pull
          break;
        } else {
          decision = 0; // punishment
        }
    }
    reactionTime = millis() - startTime; // calculate the reaction time
    lightsOff(); // turn off the lights - this can be in the motor task as well (doesn't hurt)
    digitalWrite(30, LOW);  // Tells the coordinate Teensy that the current state is not the push/pull or push/down state anymore
    // Decision Results block //
    /* #region Visual Task Rewards */
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
        // Serial.print("Light Decision: ");
        // Serial.println(lightDecision1);
        // Serial.print("Decision: ");
        // Serial.println(decision);
        if (decision == 1 && lightDecision1 == 1) {
            correctSOpen(SOpenTime);
        }
        if (decision == 2 && lightDecision1 == 2) {
            correctSOpen(SOpenTime);
        }
         if ((decision == 1 || decision == 2) && lightDecision1 == 0) {
            correctSOpen(SOpenTime);
          } 
        break;
        if (decision == 0) {
          break;
        }
        break;
    }
  /* #endregion */
    // Punishment block //
    if (decision == 0) {
      punishmentVisualTaskChooser();
    }
    digitalWrite(28, LOW);
    // ISI Delay block //
    digitalWrite(32, HIGH);  // Tells the coordinate Teensy that the current state is that the ISI delay is occurring
    int isiDelay = random(isiDelayLowerRange, isiDelayUpperRange + 1);
    delay(isiDelay);
    digitalWrite(32, LOW);  // Tells the coordinate Teensy that the current state is not the ISI delay state anymore
    // Print the data and increment variables as needed //
      printVisualInformation(isiDelay);
    trialNumber++;
    // Modifiers for the next trial //
    holdTimeVisualTask(); // hold time modifier
    reactionTimeWindowModVisual(); // RT modifier
    lightArrayModifier(); // light percentages array modifier
    isiDelayModVisual(); // ISI Delay modifier
}

/* #region CueEvokedTaskInformation */
void printCueEvokedInformation(int isiDelay) {
  switch (CueEvokedTaskDay) {
    case 1: case 2:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      //printCorrectStatusMotor();
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
      //printCorrectStatusMotor();
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
/* #endregion */


/* #region Visual Task Print Statements*/
void printVisualInformation(int isiDelay) {
  switch (visualStage) {
    case 1:
      Serial.print(trialNumber);
      Serial.print(" , ");
      Serial.print(reactionTime);
      Serial.print(" , ");
      printLightDecision(lightDecision1);
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus(decision, lightDecision1);
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime(decision, lightDecision1);
      Serial.print(" , ");
      Serial.print(positionA);
      Serial.print(" / ");
      Serial.print(positionB);
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");
      // debug //
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
      printLightDecision(lightDecision1);
      Serial.print(" , ");
      printDecision();
      Serial.print(" , ");
      printCorrectStatus(decision, lightDecision1);
      Serial.print(" , ");
      Serial.print (CorrectCounter);
      Serial.print(" / ");
      Serial.print(trialNumber);
      Serial.print(" , ");
      printOpenTime(decision, lightDecision1);
      Serial.print(" , ");
      Serial.print(isiDelay);
      Serial.print(" , ");
      // debug //
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

void printLightDecision(int lightDecision1) {
  if (lightDecision1 == 0){
    Serial.print("Plus");
  } else if (lightDecision1 == 1) {
    Serial.print("Vertical");
  } else if (lightDecision1 == 2) {
    Serial.print("Horizontal");
  }
}

void printDecision() {
  if (decision == 1) {
    Serial.print("Push");
  } else if (decision == 2) {
    Serial.print("Pull");
  } else if (decision == 0) {
    Serial.print("timedOut");
  }
}

void printCorrectStatus(int decision, int lightDecision1) {
  if ((decision == 1 || decision == 2) && lightDecision1 == 0) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else if (decision == 1 && lightDecision1 == 1) {
    Serial.print("Correct");
    correct = true;
    CorrectCounter++;
  } else if (decision == 2 && lightDecision1 == 2) {
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
void printOpenTime(int decision, int lightDecision1) {
  switch (visualStage) {
    case 1:
      if (decision == 1 || decision == 2) {
        Serial.print(OpenTime);
      } else {
        Serial.print("0");
      }
      break;
    case 2: 
    if ((decision == 1 || decision == 2) && lightDecision1 == 0) {
      Serial.print(SOpenTime);
    } else if (decision == 1 && lightDecision == 1) {
      Serial.print(SOpenTime);
    } else if (decision == 2 && lightDecision == 2) {
      Serial.print(SOpenTime);
  } else {
    Serial.print("0");
  }
  }
}

/* #endregion */