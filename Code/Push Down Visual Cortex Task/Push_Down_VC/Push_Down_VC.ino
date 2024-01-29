#include <Arduino.h>

/// Behavioral Testing Configuration ///
int totalTrials = 1000000;  // DO NOT TOUCH THIS
int isiDelayLowerRange; // Enter ISI delay lower Value in ms
int isiDelayUpperRange; // Enter ISI delay upper Value in ms
int timeOutTime = 540000000000;     // Enter the time out time
int trialNumber = 0;  // DO NOT TOUCH THIS
// const int ArraySize = 6; // Make sure this is an even number

/// Pin Set-Ups ///
#define VRX_PIN A0  // Arduino pin connected to VRX pin (aka Analog Pin A0), this is the blue cable
#define VRY_PIN A1  // Arduino pin connected to VRY pin (aka Analog Pin A1), this is the red cable
#define Solenoid 4  // Water Solenoid will be connected to Digital Pin 4
#define Puff 10     // Air solenoid for blowing air in the face of the mouse will be connected to Pin 5
#define REST_PIN 33 // Rest Pin will be connected to Digital Pin 33 on Behavior Teensy and Coordinate Teensy
#define PUSH_PIN 34 // Push Pin will be connected to Digital Pin 34 on Behavior Teensy and Coordinate Teensy
#define PULL_PIN 35 // Pull Pin will be connected to Digital Pin 35 on Behavior Teensy and Coordinate Teensy
// TODO Add pin for the light array system (data pin)

/// Soundboard Set-up ///
#include <Adafruit_Soundboard.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
// #define SFX_TX 5
// #define SFX_RX 6

// Connect to the RST pin on the Sound Board
#define SFX_RST 19

// Hardware Serial Communication Set Up
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

/// Timer Variables ///
#define timerMillis millis() - currentMillis
unsigned long currentMillis = 0;
unsigned long reactionTime;
unsigned long startTime;

/// Light Array Setup ///

# include <FastLED.h>

//Parameters
#define NUM_LEDS 48 //number of LEDs total

//Set up
#define DATA_PIN 8
#define CLOCK_PIN 14
CRGB leds[NUM_LEDS]; //define the array of LEDs

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

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);
    pinMode(Solenoid, OUTPUT); // Sets the solenoid pin as an output
    pinMode(Puff, OUTPUT); // Sets the puff pin as an output
    pinMode(29, OUTPUT); // Sets pin 29 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the rest state
    pinMode(30, OUTPUT); // Sets pin 30 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is the push/pull state
    pinMode(31, OUTPUT); // Sets pin 31 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the solenoid is open
    pinMode(32, OUTPUT); // Sets pin 32 as an output, if this pin is high it prints on the Coordinate Teensy that the current state is that the ISI delay is occurring
    pinMode(25, OUTPUT);
    pinMode(REST_PIN, INPUT); // Sets the rest pin as an input, if the mouse is in the rest coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
    pinMode(PULL_PIN, INPUT); // Sets the pull pin as an input, if the mouse is in the pull coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
    pinMode(PUSH_PIN, INPUT); // Sets the push pin as an input, if the mouse is in the push coordinate range, the coordinate Teensy will set this pin high to tell the Behavior Teensy
    Entropy.Initialize(); // Probably useless
    randomSeed(analogRead(9)); // Needed for random number generator for ISI Delay
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // Sets up the light array
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
    digitalWrite(30, HIGH); // Tells the coordinate Teensy that the current state is the push/down state (decision making state)
    // Block 2 - Push/Pull State (light array decision)
    int lightArrayRandomizer = random(0, 2); // Generates a random number between 0 and 1
    if (lightArrayRandomizer == 0) { // Vertical Stripes
        verticalStripesOne(); // display one vertical stripe on the LED array
    }
    if (lightArrayRandomizer == 1) { // Horizontal Stripes
        horizontalStripesOne(); // display one horizontal stripe on the LED array
    }
    // Block 3 - Mouse Decision Making
    startTime = millis(); // Starts the reaction time timer
    // Block 3A - Push, Vertical Stripes
    // Block 3AA - Push State (Vertical Stripes) - Correct
    if (digitalRead(PUSH_PIN) == HIGH && lightArrayRandomizer == 0) {
        reactionTime = millis() - startTime; // Stops the reaction time timer
        Serial.print(trialNumber); // Prints the trial number
        Serial.print(" , ");
        Serial.print("Vertical Stripes")
        Serial.print(" , ");
        Serial.print(reactionTime); // Prints the reaction time
        Serial.print(" , ");
        Serial.print("Push , Correct"); // Prints the decision
        digitalWrite(30, LOW);
        digitalWrite(31, HIGH); // Tells the coordinate Teensy that the current state is that the solenoid is open
        digitalWrite(Solenoid, HIGH); // Opens the solenoid
        solenoidOpenTime(); // Runs the solenoid open time function
        digitalWrite(Solenoid, LOW);
        digitalWrite(31, LOW); // Tells the coordinate Teensy that the current state is not that the solenoid is open
    }
    // Block 3AB - Push State (Vertical Stripes) - Incorrect
    if (digitalRead(PULL_PIN) == HIGH && lightArrayRandomizer == 0) {
        reactionTime = millis() - startTime; // Stops the reaction time timer
        Serial.print(trialNumber); // Prints the trial number
        Serial.print(" , ");
        Serial.print("Vertical Stripes")
        Serial.print(" , ");
        Serial.print(reactionTime); // Prints the reaction time
        Serial.print(" , ");
        Serial.print("Pull , Incorrect"); // Prints the decision
        digitalWrite(30, LOW);
        digitalWrite(31, HIGH); // Tells the coordinate Teensy that the current state is that the solenoid is open
        digitalWrite(Solenoid, HIGH); // Opens the solenoid
        solenoidOpenTime(); // Runs the solenoid open time function
        digitalWrite(Solenoid, LOW);
        digitalWrite(31, LOW); // Tells the coordinate Teensy that the current state is not that the solenoid is open
    }
    // Block 3B - Pull, Horizontal Stripes
    // Block 3BA - Pull State (Horizontal Stripes) - Correct
    if (digitalRead(PULL_PIN) == HIGH && lightArrayRandomizer == 1) {
      
        
    }
}
}


/// Parse Input Function for user inputs ///
void parseInput() {
    while (!Serial.available()) {

    }

    String input = Serial.readStringUntil('\n');
    int numValues = sscanf(input.c_str(), "%d, %d", &isiDelayLowerRange, &isiDelayUpperRange);

    }

/// Solenoid Open Time Function ///

/// Light Array Functions ///

// Plus Sign //
void lightArrayPlus() {
    for (int i = 2; i <= 44; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
}

    for (int i = 3; i <= 45; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
    }

    for (int i = 18; i <= 23; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
    }

    for (int i = 24; i <= 29; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
}
}

// Vertical Stripes //
void verticalStripesMultiple() { // please note this function does 3 stripes
    for (int i = 0; i <= 42; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
}

    for (int i = 2; i <= 44; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
    }

    for (int i = 4; i <= 46; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
}
}

void verticalStripesOne() { // please note this function does 1 stripe
    for (int i = 2; i <= 44; i = i + 6) {
        leds[i] = CRGB::White;
        FastLED.show();
    }
}

// Horizontal Stripes //

void horizontalStripesMultiple() { // please note this function does 3 stripes
    for (int i = 0; i <= 5; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
    }

    for (int i = 12; i <= 17; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
    }

    for (int i = 24; i <= 29; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
    }
    for (int i = 36; i <= 41; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
    }
}

void verticalStripesOne() { // please note this function does 1 stripe
    for (int i = 24; i <= 29; i++) {
        leds[i] = CRGB::White;
        FastLED.show();
}
}