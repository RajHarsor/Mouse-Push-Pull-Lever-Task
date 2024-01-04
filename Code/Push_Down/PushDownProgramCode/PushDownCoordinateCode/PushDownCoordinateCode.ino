//Pins
#define REST_PIN 33
#define PUSH_PIN 34
#define PULL_PIN 35
#define LED_PIN 0
#define X_COORD A0
#define Y_COORD A1

//Threshold Values
int PUSH;
int DOWN;
int REST_LOWERBOUNDX;
int REST_UPPERBOUNDX;
int REST_LOWERBOUNDY;
int REST_UPPERBOUNDY;

unsigned int x; // Variable to store the value read from the X axis of the Joystick
unsigned int y; // Variable to store the value read from the Y axis of the Joystick
unsigned long long timestamp = millis(); // Variable to store the time at which the values were read

// Lick Sensor Stuffs
#include <Wire.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t currtouched = 0;
int i = 0;

void writePins(int pin) // Function to set all pins to low at the beginning of each loop
{
    for(int i=REST_PIN; i<=PULL_PIN; i++)
    {
    if(i == pin)
    {
        digitalWrite(i, HIGH);
    }
    else
    {
        digitalWrite(i, LOW);
    }
    }
}

void parseInput() {
    while (!Serial.available()) {

    }

    String input = Serial.readStringUntil('\n');
    int numValues = sscanf(input.c_str(), "%d, %d, %d, %d", &PUSH, &PULL, &REST_LOWERBOUNDX, &REST_UPPERBOUNDX, &REST_LOWERBOUNDY, &REST_UPPERBOUNDY);

    }

void setup() {

  Serial.begin(115200);
  pinMode(REST_PIN, OUTPUT); // Sets the Rest pin as an output, tells the Behavior Teensy that when the arm is in the rest position
  pinMode(PUSH_PIN, OUTPUT); // Sets the Push pin as an output, tells the Behavior Teensy that when the arm is in the push position
  pinMode(PULL_PIN, OUTPUT); // Sets the Pull pin as an output, tells the Behavior Teensy that when the arm is in the pull position
  pinMode(LED_PIN, OUTPUT); // Sets the LED pin as an output is lit when the mouse is in the rest position
  while (!Serial) {
    delay (10);
  //if (!cap.begin(0x5A)) {
    //Serial.println("MPR121 not found, check wiring?");
    //exit(1);
  //}
  }
  parseInput();
}

void loop() {
    timestamp = millis(); // Stores the time at which the values were read
    x = analogRead(A0); // Replace with the pin connected to the joystick X axis
    y = map(analogRead(A1), 0, 1023, 1023, 0); // Replace with the pin connected to the joystick Y axis

    Serial.print(x); // Prints the X value
    Serial.print(" , ");
    Serial.print(y); // Prints the Y value
    Serial.print(" , ");
    Serial.print(timestamp); // Prints the time at which the values were read in microseconds
    Serial.print(" , ");

    if(x >= REST_LOWERBOUND_X && x <= REST_UPPERBOUND_X && y >= REST_LOWERBOUND_Y && y <= REST_UPPERBOUND_Y)
  {
    writePins(REST_PIN);
    digitalWrite(LED_PIN, HIGH);
  }
  else if(x >= PUSH)
  {
    writePins(PUSH_PIN);;
    digitalWrite(LED_PIN, LOW);

  }
  else if(y <= DOWN)
  {
    writePins(DOWN_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  if(digitalRead(29) == HIGH)
  {
    Serial.println("Stage 1: Rest ,");
  }
  else if(digitalRead(30) == HIGH)
  {
    Serial.println("Stage 2: Push/Down ,");
  }
  else if (digitalRead(31) == HIGH) {
    Serial.println("Stage 3: Water Retrival ,");
  }
  else if (digitalRead(32) == HIGH) {
    Serial.println("Stage 4: ISI ,");
  }
  else
  {
    Serial.println("None ,");
  }
  currtouched = cap.touched();
  if ((currtouched & _BV(1))){
    Serial.println(" Lick ;");
  } else {
    Serial.println(" No Lick ;");
  }
  delay(1);
}
