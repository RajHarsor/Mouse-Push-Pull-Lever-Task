// Includes
#include <Arduino.h>
#include "parse_program.h"
#include "parse_inputs.h"

// Define the pins for the joystick and communication to the other Teensy
#define REST_PIN 33
#define PUSH_PIN 34
#define PULL_OR_DOWN_PIN 35
#define LED_PIN 0
#define X_COORD A0
#define Y_COORD A1

// Behavior Teensy Pins for Communicating
#define REST_STAGE_PIN 29
#define MOVEMENT_STAGE_PIN 30
#define REWARD_STAGE_PIN 31
#define ITI_STAGE_PIN 32

// Joystick Variables
unsigned int x; // Variable to store the value read from the X axis of the Joystick
unsigned int y; // Variable to store the value read from the Y axis of the Joystick
unsigned long long timestamp = millis(); // Variable to store the time at which the values were read

//

// Lick Sensor Set-Up
#include <Wire.h>
#include "Adafruit_MPR121.h"
#define LICK_SENSOR_PIN 1

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t currtouched = 0;
int i = 0;

void setup() {
    delay(1000); // Delay to make sure the Teensy is connected to the computer
    Serial.begin(115200);
    pinMode(REST_PIN, OUTPUT); // Sets the Rest pin as an output, tells the Behavior Teensy that when the joystick is in the rest position
    pinMode(PUSH_PIN, OUTPUT); // Sets the Push pin as an output, tells the Behavior Teensy that when the joystick is in the push position
    pinMode(PULL_OR_DOWN_PIN, OUTPUT); // Sets the pull/down pin as an output, tells the Behavior Teensy that when the joystick is in the either of these positions (depending on the program)
    while (!Serial) { // giving a delay to make sure the serial monitor is connected
        delay (10);
    }
    Serial.println("What type of Program is this?");
    Serial.println("Type 1 for a Push/Pull Program");
    Serial.println("Type 2 for a Push/Down Program");
    parseProgram(); // Function to parse the type of program
    parseInputs(); // Function to parse the threshold values depending on the program type
    Serial.print("Program Type: ");
    Serial.println(programType);
    Serial.print("Push Threshold: ");
    Serial.println(PUSH);
    Serial.print("Pull Threshold: ");
    Serial.println(PULL);
    Serial.print("Down Threshold: ");
    Serial.println(DOWN);
    Serial.print("Rest Lower Bound X: ");
    Serial.println(REST_LOWERBOUND_X);
    Serial.print("Rest Upper Bound X: ");
    Serial.println(REST_UPPERBOUND_X);
    Serial.print("Rest Lower Bound Y: ");
    Serial.println(REST_LOWERBOUND_Y);
    Serial.print("Rest Upper Bound Y: ");
    Serial.println(REST_UPPERBOUND_Y);
    delay(3000);
}


void loop() {
    timestamp = millis(); // Stores the time at which the values were read
    x = analogRead(A0); // Replace with the pin connected to the joystick X axis
    y = map(analogRead(A1), 0, 1023, 1023, 0); // Replace with the pin connected to the joystick Y axis

    Serial.print(x); // Prints the X value
    Serial.print(" , ");
    Serial.print(y); // Prints the Y value
    Serial.print(" , ");
    Serial.print(timestamp); // Prints the time at which the values were read in milliseconds
    Serial.print(" , ");


    switch (programType) {
        case 1: // Push/Pull Program
            if (x >= REST_LOWERBOUND_X && x <= REST_UPPERBOUND_X) // If the X value is within the rest bounds
            {
                writePins(REST_PIN); // Set the rest pin to high
            }
            else if (x >= PUSH) // If the X value is greater than the push threshold
            {
                writePins(PUSH_PIN); // Set the push pin to high
            }
            else if (x <= PULL) // If the X value is less than the pull threshold
            {
                writePins(PULL_OR_DOWN_PIN); // Set the pull/down pin to high
            }
            break;
        case 2: // Push/Down Program
            if (x >= REST_LOWERBOUND_X && x <= REST_UPPERBOUND_X && y >= REST_LOWERBOUND_Y && y <= REST_UPPERBOUND_Y) // If the X and Y values are within the rest bounds
            {
                writePins(REST_PIN); // Set the rest pin to high
            }
            else if (x >= PUSH) // If the X value is greater than the push threshold
            {
                writePins(PUSH_PIN); // Set the push pin to high
            }
            else if (y <= DOWN) // If the Y value is less than the down threshold
            {
                writePins(PULL_OR_DOWN_PIN); // Set the pull/down pin to high
            }
            break;
    }

    // Print the Current Stage
    if (digitalRead(REST_STAGE_PIN) == HIGH)
    {
        Serial.println("Stage 1: Rest ,");
    }
    else if(digitalRead(MOVEMENT_STAGE_PIN) == HIGH) {
        Serial.println("Stage 2: Movement ,");
    } else if (digitalRead(REWARD_STAGE_PIN) == HIGH) {
        Serial.println("Stage 3: Reward ,");
    } else if(digitalRead(ITI_STAGE_PIN) == HIGH) {
        Serial.println("Stage 4: ITI ,");
    } else {
        Serial.println("None/Error ,");
    }

    // Print Lick Sensor Status
    currtouched = cap.touched();
  if ((currtouched & _BV(LICK_SENSOR_PIN))){ // Please note that "1" in _BV(1) is the pin number of the lick sensor...can be changed accordingly
    Serial.println(" Lick ;");
  } else {
    Serial.println(" No Lick ;");
  }
  delay(1);
}

void writePins(int pin) // Function to set all pins to low at the beginning of each loop
{
    for(int i=REST_PIN; i<=PULL_OR_DOWN_PIN; i++) {
        if(i == pin) {
        digitalWrite(i, HIGH);
    } else {
        digitalWrite(i, LOW);
    }
    }
}