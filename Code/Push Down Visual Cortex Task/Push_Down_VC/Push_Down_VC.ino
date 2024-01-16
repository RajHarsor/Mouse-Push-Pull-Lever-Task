#include <Arduino.h>

/// Behavioral Testing Configuration ///
int totalTrials = 1000000;  // DO NOT TOUCH THIS
int isiDelayLowerRange; // Enter ISI delay lower Value in ms
int isiDelayUpperRange; // Enter ISI delay upper Value in ms
int timeOutTime = 540000000000;     // Enter the time out time
int trialNumber = 0;  // DO NOT TOUCH THIS
// const int ArraySize = 6; // Make sure this is an even number

/ Pin Set-Ups //
#define VRX_PIN A0  // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1  // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4  // Water Solenoid will be connected to Digital Pin 4
#define Puff 10     // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5
#define REST_PIN 33 // Rest Pin will be connected to Digital Pin 33 on Behavior Teensy and Coordinate Teensy
#define PUSH_PIN 34 // Push Pin will be connected to Digital Pin 34 on Behavior Teensy and Coordinate Teensy
#define PULL_PIN 35 // Pull Pin will be connected to Digital Pin 35 on Behavior Teensy and Coordinate Teensy
// TODO Add pin for the light array system (data pin)

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
    for (trialNumber = 1; trialNumber < totalTrials; trialNumber++) {
    // Block 1 - Rest State (wait for 50 ms in the rest position)
    currentMillis = millis(); // starts the timber
    digitalWrite(29, HIGH); //Tells the coordinate Teensy that the current state is the rest state
    while (timerMillis <= 50) {
        if (digitalRead(REST_PIN) == HIGH) {
            continue;
        } else {
            currentMillis = millis();
            continue;
        }
    }
    digitalWrite(29, LOW); // Tells the coordinate Teensy that the current state is not the rest state anymore
    digitalWrite(30, HIGH); // Tells the coordinate Teensy that the current state is the push/pull state (decision making state)
    // Block 2 - Push/Pull State (light array + decision making)
    int lightArrayRandomizer = random(0, 2); // Generates a random number between 0 and 1
    if (lightArrayRandomizer == 0) { // Vertical Stripes

    }
}
}



void parseInput() {
    while (!Serial.available()) {

    }

    String input = Serial.readStringUntil('\n');
    int numValues = sscanf(input.c_str(), "%d, %d", &isiDelayLowerRange, &isiDelayUpperRange);

    }