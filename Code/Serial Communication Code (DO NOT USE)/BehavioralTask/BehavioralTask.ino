#include <Entropy.h>
#include <Arduino.h>

/////// Joystick code based off of https://arduinogetstarted.com/tutorials/arduino-joystick

/// Behavioral Testing Configuration ///
int totalTrials = 1000000;  // DO NOT TOUCH THIS
int isiDelayLowerRange = 1000; // Enter ISI delay lower Value in ms
int isiDelayUpperRange = 3000; // Enter ISI delay upper Value in ms
int timeOutTime = 540000000000;     // Enter the time out time
int LowerBoundRestX = 525; // Enter the lower bound for the rest position
int UpperBoundRestX = 535; // Enter the upper bound for the rest position

// Push and Pull Parameters //
int Push = 540;  // X position to be considered a push (You can change this)
int Pull = 515;  // Y position to be considered a pull (You can change this value)

// Pin Set-Ups //
#define VRX_PIN A0  // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1  // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4  // Water Solenoid will be connected to Digital Pin 4
#define Puff 10     // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5

// Soundboard Set-up //
#include <Adafruit_Soundboard.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
// #define SFX_TX 5
// #define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 19

// Hardware Serial Communication Set Up
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);


// Joystick Variables //
  float x, y;

void readSensors() {      // function for reading the X and Y values from the Joystick
   if (Serial2.available()) {
    // Serial.print(available); 
    Serial2.readBytes((byte*)&x, sizeof(x));
    Serial2.readBytes((byte*)&y, sizeof(y));
    // delay(15);
} else {
  readSensors();
}
}

void coordinates() {  // Coordinates function which displays the x value, y value, and the time in seconds of the movement
  Serial.println(" ");
  Serial.print("x = ");
  Serial.print(x);  // x value reading of the joystick
  Serial.print(" y = ");
  Serial.print(y);  // y value reading of the joystick
  Serial.println(",");
}

////// Timer Variables //////
#define timerMillis millis() - currentMillis
unsigned long currentMillis = 0;
unsigned long reactionTime;
unsigned long startTime;

/// Solenoid Open Time Variables ///////
int puffTime = 0;  // How long the air solenoid / punishment is
int mouse_failed;
int positionA = 0.0;  // Push Counter;
int positionB = 0.0;  // Pull Counter;

int OpenTimeArray[10];  // initialize a blank array with 10 slots
int currentIndex = 0;
int ArrayCount1 = 0;
int ArrayCount2 = 0;
float OpenTime;

void solenoidOpenTime() {  /// function to determine the solenoid open time
  if (currentIndex < 10) {    // if there aren't 10 integers in the array, add to it
    if (mouse_failed == 0) {  // if a mouse did a push add a 1 to the array
      OpenTimeArray[currentIndex] = 1;
      currentIndex++;
    }
    if (mouse_failed == 2) {  // if a mouse does a pull add a 2 to the array
      OpenTimeArray[currentIndex] = 2;
      currentIndex++;
    }
  } else {                          // if the array is full
    for (int i = 8; i >= 0; i--) {  // This section might be in the incorrect order, why are we moving the values down after we added the value to the array?
      OpenTimeArray[i + 1] = OpenTimeArray[i];
    }
    if (mouse_failed == 0) {
      OpenTimeArray[0] = 1;
    }
    if (mouse_failed == 2) {
      OpenTimeArray[0] = 2;
    }
  }
  Serial.print("Current Array: [");  // print the current array of pushes and pulls
  for (int i = 0; i < 10; i++) {
    Serial.print(OpenTimeArray[i]);
    Serial.print(" ");
  }
  Serial.print("]");
  Serial.print(",");
  Serial.println(" ");
  for (int i = 0; i < 10; i++) {
    if (OpenTimeArray[i] == 1) {  // Count the number og 1s in the array (pushes)
      ArrayCount1 = ArrayCount1 + 1;
    }
    if (OpenTimeArray[i] == 2) {  // Count the number of 2s in the array (pulls)
      ArrayCount2 = ArrayCount2 + 1;
    }
  }
  Serial.print("Current F/R Ratio = ");  // Should ultimately replace the code that is above that prints the number of 1's and 2's
  Serial.print(ArrayCount1);
  Serial.print(" / ");
  Serial.print(ArrayCount2);
  Serial.println(",");


  Serial.print("Total F/R Ratio = ");
  Serial.print(positionA);
  Serial.print(" / ");
  Serial.print(positionB);
  Serial.println(",");

  if (mouse_failed == 0) {  // Push Equation
    // OpenTime = -7.83 * (ArrayCount1 + 1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2 + 1), 2) - 18.44 * (ArrayCount1 + 1 - ArrayCount2)/(ArrayCount1 + ArrayCount2 + 1) + 48.28;
    OpenTime = -7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) - 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28;
    Serial.print("Solenoid Open Time = ");
    Serial.print(OpenTime);
    Serial.println(",");
    currentMillis = millis();
    while (timerMillis <= OpenTime) {
      continue;
    }
  }

  if (mouse_failed == 2) {  // Pull Equation
    // OpenTime = -7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2 + 1),2) + 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2 + 1) + 48.28;
    OpenTime = -7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) + 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28;
    Serial.print("Solenoid Open Time = ");
    Serial.print(OpenTime);
    Serial.println(",");
    currentMillis = millis();
    while (timerMillis <= OpenTime) {
      continue;
    }
  }
  ArrayCount1 = 0;
  ArrayCount2 = 0;
}

void setup() {
  unsigned char Serial2Buffer[10000];

  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(115200);
  Serial2.addMemoryForRead(Serial2Buffer, sizeof(Serial2Buffer));
  pinMode(Solenoid, OUTPUT);
  pinMode(Puff, OUTPUT);
  pinMode(29, OUTPUT);
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(25, OUTPUT);
  Entropy.Initialize();
  randomSeed(analogRead(9));
}

void loop() {
//   readSensors();
//     coordinates();
// }
  for (int trialNumber = 1; trialNumber < totalTrials; trialNumber++) {  // Initializes the trial number at 1, goes until we hit the total number of trials (totalTrials) in the testing configuration section, and increases trial number by 1 every cycle
    Serial.print("Trial Number = ");
    Serial.print(trialNumber);  // prints the trial number
    Serial.println(" ,");
    currentMillis = millis();     // starts timer
    digitalWrite(29, HIGH);
    while (timerMillis <= 50) {
      readSensors();
      // coordinates();
      if (x >= LowerBoundRestX && x <= UpperBoundRestX) {  //mouse needs to hold at start position for 50 ms to go forward and for the sound to play
        digitalWrite(25, HIGH);
        continue;
      } else {
        digitalWrite(25, LOW);
        currentMillis = millis();  //time programs been active. If the 50 ms is not reached, will reset to 0
        continue;
      }
    }
    digitalWrite(25, LOW);
    digitalWrite(29, LOW);
    digitalWrite(30, HIGH);
    sfx.playTrack("T02     OGG");         // code for playing the sound (6 db F# sound at 3 kHz);
    currentMillis = millis();             // start timer for timeout
    startTime = millis();                 // start reaction time timer
    while (timerMillis <= timeOutTime) {  //mouse has 3 seconds to respond to the sound or else timed out
      readSensors();
      // coordinates();
      if (x >= Push) {
        reactionTime = millis() - startTime;
        Serial.print(reactionTime);
        Serial.print(" ms,");
        coordinates();
        mouse_failed = 0;
        positionA++;
        digitalWrite(30, LOW);
        digitalWrite(31, HIGH);
        sfx.playTrack("T03     OGG"); 
        digitalWrite(Solenoid, HIGH);
        solenoidOpenTime();
        digitalWrite(Solenoid, LOW);
        digitalWrite(31, LOW);
        break;
      }
      if (x <= Pull) {
        reactionTime = millis() - startTime;
        Serial.print(reactionTime);
        Serial.print(" ms,");
        coordinates();
        mouse_failed = 2;
        positionB++;
        digitalWrite(30, LOW);
        digitalWrite(31, HIGH);
        sfx.playTrack("T03     OGG"); 
        digitalWrite(Solenoid, HIGH);
        solenoidOpenTime();
        digitalWrite(Solenoid, LOW);
        digitalWrite(31, LOW);
        break;
      } else {
        mouse_failed = 1;
        continue;
      }
    }
    if (mouse_failed == 1) {
      reactionTime = millis() - startTime;
      Serial.print(reactionTime);
      Serial.print(" ms,");
      coordinates();
      Serial.println("Failed ,");
      digitalWrite(Puff, HIGH);
      currentMillis = millis();
      while (timerMillis <= puffTime) {
        continue;
      }
      digitalWrite(Puff, LOW);
    } else if (mouse_failed == 0) {
      Serial.println("Push ,");
    } else {
      Serial.println("Pull ,");
    }
    digitalWrite(32, HIGH);
    currentMillis = millis();
    int isiDelay = random(isiDelayLowerRange, isiDelayUpperRange + 1);
    while (timerMillis <= isiDelay) {  //ISI delay
      readSensors();
      continue;
    }
    Serial.print("ISI Delay = ");
    Serial.print(isiDelay);
    Serial.println(" ,");
    digitalWrite(32, LOW);
    Serial2.clear();
  }
}