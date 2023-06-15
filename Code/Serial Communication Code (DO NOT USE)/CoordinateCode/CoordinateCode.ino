void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  pinMode(29, INPUT);
  pinMode(30, INPUT);
  pinMode(31, INPUT);
  pinMode(32, INPUT);
}

void loop() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 15; // interval at which to send data (milliseconds)
  unsigned long currentMillis = millis();
  unsigned long timestamp = micros(); 

  float x = analogRead(A0); // Replace with the pin connected to the joystick X axis
  float y = map(analogRead(A1), 0, 1023, 1023, 0); // Replace with the pin connected to the joystick Y axis

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Serial.print("x = ");
    // Serial.print(x);  // x value reading of the joystick
    // Serial.print(" y = ");
    // Serial.print(y);  // y value reading of the joystick
    // Serial.println(",");
    // Serial.print("Elapsed time: ");
    // Serial.print(timestamp);
    // Serial.println(" us ,");

    Serial.print(x);
    Serial.print(" , ");
    Serial.print(y);
    Serial.print(" , ");
    Serial.print(timestamp);
    Serial.print(" , ");

    // Send the x and y values as a single packet
    Serial2.write((byte*)&x, sizeof(x));
    Serial2.write((byte*)&y, sizeof(y));
    } else {
      // Serial.print("x = ");
      // Serial.print(x);  // x value reading of the joystick
      // Serial.print(" y = ");
      // Serial.print(y);  // y value reading of the joystick
      // Serial.println(",");
      // Serial.print("Elapsed time: ");
      // Serial.print(timestamp);
      // Serial.println(" us ,");

      Serial.print(x);
      Serial.print(" , ");
      Serial.print(y);
      Serial.print(" , ");
      Serial.print(timestamp);
      Serial.print(" , ");
    }
     if (digitalRead(29) == HIGH) {
      Serial.println("Stage 1: Rest ;");
    }
    if (digitalRead(30) == HIGH) {
      Serial.println("Stage 2: Push/Pull ;");
    }
    if (digitalRead(31) == HIGH) {
      Serial.println("Stage 3: Water Retrival ;");
    }
    if (digitalRead(32) == HIGH) {
      Serial.println("Stage 4: ISI ;");
    }
    delay(1);
  }