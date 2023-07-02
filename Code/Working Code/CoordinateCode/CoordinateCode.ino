//Pins
#define REST_PIN 33
#define PUSH_PIN 34
#define PULL_PIN 35
#define LED_PIN 0
#define X_COORD A0
#define Y_COORD A1

//Threshold values
#define PUSH 570 // Set the push threshold value to a value greater than the REST_UPPERBOUND value
#define DOWN 515 // Set the pull threshold value to a value less than the REST_LOWERBOUND value
#define REST_LOWERBOUND 525
#define REST_UPPERBOUND 535


unsigned int x; // Variable to store the value read from the X axis of the Joystick
unsigned int y; // Variable to store the value read from the Y axis of the Joystick
unsigned long timestamp = micros(); // Variable to store the time at which the values were read


//
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

void setup() {

  Serial.begin(115200);
  pinMode(REST_PIN, OUTPUT); // Sets the Rest pin as an output, tells the Behavior Teensy that when the arm is in the rest position
  pinMode(PUSH_PIN, OUTPUT); // Sets the Push pin as an output, tells the Behavior Teensy that when the arm is in the push position
  pinMode(PULL_PIN, OUTPUT); // Sets the Pull pin as an output, tells the Behavior Teensy that when the arm is in the pull position
  pinMode(LED_PIN, OUTPUT); // Sets the LED pin as an output is lit when the mouse is in the rest position
}

void loop() {
  timestamp = micros(); // Stores the time at which the values were read
  x = analogRead(A0); // Replace with the pin connected to the joystick X axis
  y = map(analogRead(A1), 0, 1023, 1023, 0); // Replace with the pin connected to the joystick Y axis

  Serial.print(x); // Prints the X value
  Serial.print(" , ");
  Serial.print(y); // Prints the Y value
  Serial.print(" , ");
  Serial.print(timestamp); // Prints the time at which the values were read in microseconds
  Serial.print(" , ");

  if(x >= REST_LOWERBOUND && x <= REST_UPPERBOUND)
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
    writePins(PULL_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  if(digitalRead(29) == HIGH)
  {
    Serial.println("Stage 1: Rest ;");
  }
  else if(digitalRead(30) == HIGH)
  {
    Serial.println("Stage 2: Push/Down ;");
  }
  else if (digitalRead(31) == HIGH) {
    Serial.println("Stage 3: Water Retrival ;");
  }
  else if (digitalRead(32) == HIGH) {
    Serial.println("Stage 4: ISI ;");
  }
  else 
  {
    Serial.println("None ;");
  }
  delay(1);
}