#include <Arduino.h>
#include <vector>
#include <string>
#include <map>
#include <FastLED.h>
#include "parse_scripts.h"
#include "light_configs.h"
#include "solenoid_open_time.h"

/* #region PIN SETUPS */
#define VRX_PIN A0   // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1   // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4   // Water Solenoid will be connected to Digital Pin 4
#define Puff 10      // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5
#define REST_PIN 33  // Rest Pin will be connected to Digital Pin 33 on Behavior Teensy and Coordinate Teensy
#define PUSH_PIN 34  // Push Pin will be connected to Digital Pin 34 on Behavior Teensy and Coordinate Teensy
#define PULL_PIN 35  // Pull Pin will be connected to Digital Pin 35 on Behavior Teensy and Coordinate Teensy
/* #endregion */

//Parameters
#define NUM_LEDS 48  //number of LEDs total
#define BRIGHTNESS 5

// Set up
#define DATA_PIN 8
#define CLOCK_PIN 14
#define NUM_LEDS 48  //number of LEDs total

CRGB leds[NUM_LEDS];  //define the array of LEDs

/* Array Set up (WS2012B)

0  1  2  3  4  5
6  7  8  9  10 11
12 13 14 15 16 17
18 19 20 21 22 23
24 25 26 27 28 29
30 31 32 33 34 35
36 37 38 39 40 41
42 43 44 45 46 47

*/

/* #endregion */

/// Soundboard Set-up ///
#include <Adafruit_Soundboard.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
// #define SFX_TX 5
// #define SFX_RX 6


// Connect to the RST pin on the Sound Board
#define SFX_RST 19

// Hardware Serial Communication Set Up
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);


/* #region BEHAVIOR VARIBALES CONST */
int totalTrials = 1000000;
int trialNumber = 1;
int decision;
int positionA = 0;
int positionB = 0;
int CorrectCounter = 0;

/* #end region */

/* #region Timer Variables */
unsigned long currentMillis = 0;
unsigned long reactionTime = 0;
#define timerMillis millis() - currentMillis
unsigned long startTime;
/* #end region */

void setup() {
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
  Serial.println("What type of program would you like?");
  Serial.println("Type 1 for Visual Task");
  Serial.println("Type 2 for Motor Task");
  parseProgramTypeInput();
  Serial.print(programType);
  switch (programType) {
    case 1: // visual task
      Serial.println("Pick the Stage (1-4)");
      parseVisualStage();
      break;
    case 2:
      Serial.println("What type of Motor Task do you want to do?");
      // TODO: finish case 2 //
      break;
  }
}

void loop() {
    // Rest position block
    currentMillis = millis();
    digitalWrite(29, HIGH);
    while (timerMillis <= holdTime) {
      if (digitalRead(REST_PIN) == HIGH) {
        continue;
      } else {
        currentMillis = millis();
        continue;
      }
    }
    digitalWrite(29, LOW);   // Tells the coordinate Teensy that the current state is not the rest state anymore
    digitalWrite(30, HIGH);  // Tells the coordinate Teensy that the current state is the push/pull or push/down state
    // Light array decision block //
    if (programType == 1) {
      switch (visualStage) {
        case 1:
          lightArrayPlus();
          break;
        case 2:
          executeLightArray();
          break;
        case 3:
          executeLightArray();
          break;
        case 4:
          executeLightArray();
          break;
          }
      }
      // Push/Pull decision block //
    currentMillis = millis();
    startTime = millis();
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
    reactionTime = millis() - startTime;
    lightsOff();
    digitalWrite(30, LOW);  // Tells the coordinate Teensy that the current state is not the push/pull or push/down state anymore
    // Decision Results block //
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
              correctSOpen();
            } else if (decision == 2 && lightDecision == 2) {
              correctSOpen();
            } else {
              decision = 0;
            }
          break;
        case 3:
          if (decision == 1 && lightDecision == 1) {
            correctSOpen();
          } else if (decision == 2 && lightDecision == 2) {
            correctSOpen();
          } else {
            decision = 0;
          }
          break;
        case 4:
          if (decision == 1 && lightDecision == 1) {
            correctSOpen();
          } else if (decision == 2 && lightDecision == 2) {
            correctSOpen();
          } else {
            decision = 0;
          }
          break;
      }
    }
    if (decision == 0) {
      digitalWrite(28, HIGH);
      sfx.playTrack("T04     WAV");
      delay(120 + punishmentTime);
      sfx.stop();
    }
    digitalWrite(28, LOW);
    // ISI Delay block //
    digitalWrite(32, HIGH);  // Tells the coordinate Teensy that the current state is that the ISI delay is occurring
    int isiDelay = random(isiDelayLowerRange, isiDelayUpperRange + 1);
    delay(isiDelay);
    digitalWrite(32, LOW);  // Tells the coordinate Teensy that the current state is not the ISI delay state anymore
    // Print the data and increment variables as needed //
    Serial.print(trialNumber);
    Serial.print(" , ");
    Serial.print(reactionTime);
    Serial.print(" , ");
    if (lightDecision == 0 ){
      Serial.print("Plus");
    } else if (lightDecision == 1) {
      Serial.print("Verical");
    } else if (lightDecision == 2) {
      Serial.print("Horizontal");
    }
    Serial.print(" , ");
    if (decision == 1) {
      Serial.print("Push");
    } else if (decision == 2) {
      Serial.print("Pull");
    } else {
      Serial.print("Timeout");
    }
    Serial.print(" , ");
    if (decision == 1 || decision == 2 && lightDecision == 0) {
      Serial.print("Correct");
      CorrectCounter++;
    } else if (decision == 1 && lightDecision == 1) {
      Serial.print("Correct");
      CorrectCounter++;
    } else if (decision == 2 && lightDecision == 2) {
      Serial.print("Correct");
      CorrectCounter++;
    } else {
      Serial.print("Incorrect");
    }
    
    Serial.print(" , ");
    Serial.print (CorrectCounter);
    Serial.print(" / ");
    Serial.print(trialNumber);
    Serial.print(" , ");
    if (visualStage == 1){
      if (decision == 1 || decision == 2) {
        Serial.print(OpenTime);
      } else {
        Serial.print("0");
      }
      Serial.print(" , ");
      Serial.print(positionA);
      Serial.print(" / ");
      Serial.print(positionB);
      Serial.print(" , ");
    } else {
      Serial.print(SOpenTime);
    }
    Serial.print(isiDelay);
    Serial.println(" ; ");
    trialNumber++;
}

void correctSOpen() {
  digitalWrite(31, HIGH);
  digitalWrite(Solenoid, HIGH);
  delay(SOpenTime);
  digitalWrite(Solenoid, LOW);
  digitalWrite(31, LOW);
}

