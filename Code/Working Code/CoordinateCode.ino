//Pins
#define REST_PIN 33
#define PUSH_PIN 34
#define PULL_PIN 35
#define LED_PIN 0
#define X_COORD A0
#define Y_COORD A1

//Threshold values
#define PUSH 570
#define PULL 515
#define REST_LOWERBOUND 525
#define REST_UPPERBOUND 535


unsigned int x;
unsigned int y;
unsigned long timestamp = micros();


//
void writePins(int pin)
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
  pinMode(REST_PIN, OUTPUT);
  pinMode(PUSH_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  timestamp = micros();
  x = analogRead(A0); // Replace with the pin connected to the joystick X axis
  y = map(analogRead(A1), 0, 1023, 1023, 0); // Replace with the pin connected to the joystick Y axis

  Serial.print(x);
  Serial.print(" , ");
  Serial.print(y);
  Serial.print(" , ");
  Serial.print(timestamp);
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
  else if(x <= PULL)
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
    Serial.println("Stage 2: Push/Pull ;");
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