#include <Arduino.h>
int solenoidPin = 10; //Solenoid is connected to digital pin 10

void setup() {
  // put your setup code here, to run once:
pinMode(solenoidPin, OUTPUT); //Sets the pin as an output
}

void loop() {
  // put your main code here, to run repeatedly:
delay(5000);
digitalWrite(solenoidPin, HIGH); //Switch Solenoid ON
delay(500000); //Wait x seconds
digitalWrite(solenoidPin, LOW); //Switch Solenoid OFF
delay(50000000); 
}
