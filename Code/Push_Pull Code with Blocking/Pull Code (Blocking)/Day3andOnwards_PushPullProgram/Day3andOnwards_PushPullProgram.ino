#include <Entropy.h>
#include <Arduino.h>

/////// Joystick code based off of https://arduinogetstarted.com/tutorials/arduino-joystick

/// Behavioral Testing Configuration ///
int totalTrials = 1000000;  // DO NOT TOUCH THIS
int isiDelayLowerRange; // Enter ISI delay lower Value in ms
int isiDelayUpperRange; // Enter ISI delay upper Value in ms
int timeOutTime = 540000000000;     // Enter the time out time
int trialNumber = 0;  // DO NOT TOUCH THIS
const int ArraySize = 6; // Make sure this is an even number


// Pin Set-Ups //
#define VRX_PIN A0  // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1  // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4  // Water Solenoid will be connected to Digital Pin 4
#define Puff 10     // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5
#define REST_PIN 33 // Rest Pin will be connected to Digital Pin 33 on Behavior Teensy and Coordinate Teensy
#define PUSH_PIN 34 // Push Pin will be connected to Digital Pin 34 on Behavior Teensy and Coordinate Teensy
#define PULL_PIN 35 // Pull Pin will be connected to Digital Pin 35 on Behavior Teensy and Coordinate Teensy

// Soundboard Set-up //
#include <Adafruit_Soundboard.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
// #define SFX_TX 5
// #define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 19

// Hardware Serial Communication Set Up
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);


////// Timer Variables //////
#define timerMillis millis() - currentMillis
unsigned long currentMillis = 0;
unsigned long reactionTime;
unsigned long startTime;

/// Solenoid Open Time Variables ///////
int puffTime = 0;  // How long the air solenoid / punishment is
int mouse_failed; // Different conditions the mouse can go into, might want to rename in future
int positionA = 0.0;  // Push Counter;
int positionB = 0.0;  // Pull Counter;

int OpenTimeArray[ArraySize];  // initialize a blank array with 10 slots
int currentIndex = 0;  // initialize the current index of the array to 0
int ArrayCount1 = 0; // initialize the number of 1's in the array to 0
int ArrayCount2 = 0; // initialize the number of 2's in the array to 0
float OpenTime; // variable to store the solenoid open time

//TODO Make this into a global variable in the configuration where people can edit how big the array is
void solenoidOpenTime() {  /// function to determine the solenoid open time
  if (currentIndex < ArraySize) {    // if there aren't 10 integers in the array, add to it until there are 10
    if (mouse_failed == 0) {  // if a mouse did a push add a 1 to the array
      OpenTimeArray[currentIndex] = 1;
      currentIndex++;
    }
    if (mouse_failed == 2) {  // if a mouse does a pull add a 2 to the array
      OpenTimeArray[currentIndex] = 2;
      currentIndex++;
    }
  } else {                          // if the array is full
    for (int i = ArraySize - 2; i >= 0; i--) {  // shift all the values in the array down one
      OpenTimeArray[i + 1] = OpenTimeArray[i];
    }
    if (mouse_failed == 0) { // if a mouse did a push add a 1 to the array at position 0
      OpenTimeArray[0] = 1;
    }
    if (mouse_failed == 2) { // if a mouse did a pull add a 2 to the array at position 0
      OpenTimeArray[0] = 2;
    }
  }
  Serial.print("[");  // print the current array of pushes and pulls
  for (int i = 0; i < ArraySize; i++) {
    Serial.print(OpenTimeArray[i]);
    Serial.print(" ");
  }
  Serial.print("]");
  Serial.print(" , ");
  Serial.print(" ");
  for (int i = 0; i < ArraySize; i++) {
    if (OpenTimeArray[i] == 1) {  // Count the number of 1s in the array (pushes)
      ArrayCount1 = ArrayCount1 + 1;
    }
    if (OpenTimeArray[i] == 2) {  // Count the number of 2s in the array (pulls)
      ArrayCount2 = ArrayCount2 + 1;
    }
  }
  // Serial.print("Current F/R Ratio = ");  // Print the current F/R ratio (pushes/pulls)
  Serial.print(ArrayCount1);
  Serial.print(" / ");
  Serial.print(ArrayCount2);
  Serial.print(" , ");


  // Serial.print("Total F/R Ratio = "); // Print the total F/R ratio (pushes/pulls)
  Serial.print(positionA);
  Serial.print(" / ");
  Serial.print(positionB);
  Serial.print(" , ");

  if (mouse_failed == 0) {  // Push Equation
    if (ArrayCount1 == ArraySize or ArrayCount2 == ArraySize) {
      OpenTime = 0;
      Serial.print(OpenTime);
      Serial.print(" , ");
    } else {
    // OpenTime = -7.83 * (ArrayCount1 + 1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2 + 1), 2) - 18.44 * (ArrayCount1 + 1 - ArrayCount2)/(ArrayCount1 + ArrayCount2 + 1) + 48.28;
    OpenTime = (-7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) - 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28);
    // Serial.print("Solenoid Open Time = ");
    Serial.print(OpenTime);
    Serial.print(" , ");
    currentMillis = millis();
    while (timerMillis <= OpenTime) {
      continue;
    }
    }
  }

  if (mouse_failed == 2) {  // Pull Equation
    if (ArrayCount1 == ArraySize or ArrayCount2 == ArraySize) {
      OpenTime = 0;
      Serial.print(OpenTime);
      Serial.print(" , ");
    } else {
    // OpenTime = -7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2 + 1),2) + 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2 + 1) + 48.28;
    OpenTime = (-7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) + 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28);
    // Serial.print("Solenoid Open Time = ");
    Serial.print(OpenTime);
    Serial.print(" , ");
    currentMillis = millis();
    while (timerMillis <= OpenTime) {
      continue;
    }
  }
  }
  ArrayCount1 = 0; // Reset the number of 1s in the array to 0
  ArrayCount2 = 0; // Reset the number of 2s in the array to 0
}

void parseInput() {
  while (!Serial.available()) {

  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d, %d", &isiDelayLowerRange, &isiDelayUpperRange);

  }


void setup() {

  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(Solenoid, OUTPUT); // Sets the solenoid pin as an output
  pinMode(Puff, OUTPUT); // Sets the puff pin as an output
  pinMode(29, OUTPUT); // Sets pin 29 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the rest state
  pinMode(30, OUTPUT); // Sets pin 30 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the push/pull state
  pinMode(31, OUTPUT); // Sets pin 31 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the solenoid is open
  pinMode(32, OUTPUT); // Sets pin 32 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the ISI delay is occuring
  pinMode(25, OUTPUT);
  pinMode(REST_PIN, INPUT); // Sets the rest pin as an input, if the mouse is in the rest coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
  pinMode(PULL_PIN, INPUT); // Sets the pull pin as an input, if the mouse is in the pull coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
  pinMode(PUSH_PIN, INPUT); // Sets the push pin as an input, if the mouse is in the push coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
  Entropy.Initialize(); // Probably useless
  randomSeed(analogRead(9)); // Needed for random number generator for ISI Delay
  parseInput();
}

void loop() {
//   readSensors();
//     coordinates();
// }
  for (trialNumber = 1; trialNumber < totalTrials; trialNumber++) {  // Initializes the trial number at 1, goes until we hit the total number of trials (totalTrials) in the testing configuration section, and increases trial number by 1 every cycle
    //Serial.print("Trial Number = ");
    // Serial.print(trialNumber);  // prints the trial number
    // Serial.print(" ,");
    currentMillis = millis();     // starts timer
    digitalWrite(29, HIGH); // Tells the coordinate teensy the current state is the rest state
    while (timerMillis <= 50) { // mouse needs to hold at start position for 50 ms to go forward and for the sound to play
    //TODO Make the holdTime a global variable so it can be edited in the testing configuration section (currently 50 ms)
      if (digitalRead(REST_PIN) == HIGH) {
        continue;
      } else {
        currentMillis = millis();  //time programs been active. If the 50 ms is not reached, will reset to 0
        continue;
      }
    }
    digitalWrite(29, LOW); // Tells the coordinate teensy the rest state is ended
    digitalWrite(30, HIGH); // Tells the coordinate teensy the current state is the push/pull state
    sfx.playTrack("T02     OGG");         // code for playing the sound (6 db F# sound at 3 kHz);
    currentMillis = millis();             // start timer for timeout
    startTime = millis();                 // start reaction time timer
    while (timerMillis <= timeOutTime) {  //mouse has until timeOutTime to respond to the sound or else timed out
      if (digitalRead(PUSH_PIN) == HIGH) { // if the mouse pushes the lever
        reactionTime = millis() - startTime; // calculate reaction time
        Serial.print(trialNumber);  // prints the trial number
        Serial.print(" , ");
        Serial.print(reactionTime); // print reaction time
        Serial.print(" , ");
        mouse_failed = 0; // set mouse_failed to 0, which means the mouse pushed the lever
        positionA++; // increase the counter of the number of times the mouse pushed the lever
        digitalWrite(30, LOW); // Tells the coordinate teensy the push/pull state is ended
        digitalWrite(31, HIGH); // Tells the coordinate teensy the current state is that the solenoid is open
        sfx.playTrack("T03     OGG"); // Low tone sound is played as a cue for the mouse that water is available
        digitalWrite(Solenoid, HIGH); // Opens the solenoid
        solenoidOpenTime(); // Calls the function that calculates the solenoid open time
        digitalWrite(Solenoid, LOW); // Closes the solenoid
        digitalWrite(31, LOW); // Tells the coordinate teensy the solenoid open state is ended
        break;
      }
      if (digitalRead(PULL_PIN) == HIGH) { // if the mouse pulls the lever
        reactionTime = millis() - startTime; // calculate reaction time
        Serial.print(trialNumber);
        Serial.print(" , ");
        Serial.print(reactionTime); // print reaction time
        Serial.print(" , ");
        mouse_failed = 2; // set mouse_failed to 2, which means the mouse pulled the lever
        positionB++; // increase the counter of the number of times the mouse pulled the lever
        digitalWrite(30, LOW); // Tells the coordinate teensy the push/pull state is ended
        digitalWrite(31, HIGH); // Tells the coordinate teensy the current state is that the solenoid is open
        sfx.playTrack("T03     OGG"); // Low tone sound is played as a cue for the mouse that water is available
        digitalWrite(Solenoid, HIGH); // Opens the solenoid
        solenoidOpenTime(); // Calls the function that calculates the solenoid open time
        digitalWrite(Solenoid, LOW); // Closes the solenoid
        digitalWrite(31, LOW); // Tells the coordinate teensy the solenoid open state is ended
        break;
      } else {
        mouse_failed = 1; // set mouse_failed to 1, which means the mouse did not push or pull the lever
        continue;
      }
    }
    if (mouse_failed == 1) { // if the mouse did not push or pull the lever
      reactionTime = millis() - startTime;
      Serial.print(reactionTime);
      //Serial.print(", ");
      Serial.print("Failed ,");
      digitalWrite(Puff, HIGH); // Puff is turned on
      currentMillis = millis();
      while (timerMillis <= puffTime) { // puffTime is the amount of time the puff is on
        continue;
      }
      digitalWrite(Puff, LOW); // Puff is turned off
    } else if (mouse_failed == 0) {
      Serial.print("Push");
      Serial.print(" , ");
    } else {
      Serial.print("Pull");
      Serial.print(" , ");
    }
    digitalWrite(32, HIGH); // Tells the coordinate teensy the current state is that the ISI delay is occurring
    currentMillis = millis();
    int isiDelay = random(isiDelayLowerRange, isiDelayUpperRange + 1); // Generates a random number between the lower and upper range of the ISI delay
    Serial.print(isiDelay);
    Serial.println(" ; ");
    while (timerMillis <= isiDelay) {  //ISI delay
      continue;
    }
    // Serial.print("ISI Delay = "); // Prints the ISI delay
    digitalWrite(32, LOW); // Tells the coordinate teensy the ISI delay state is ended
  }
}