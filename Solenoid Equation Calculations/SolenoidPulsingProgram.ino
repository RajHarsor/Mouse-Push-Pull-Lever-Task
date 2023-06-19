#include <Arduino.h>
int solenoidPin = 5; //This is the output pin on the Arduino we are using

void setup() {
// put your setup code here, to run once:
pinMode(solenoidPin, OUTPUT); //Sets the pin as an output
}

void loop() {
delay(5000);
for(int i=0; i <= 100; i++) // for however many pulses that you want, adjust the integer value after "i<="
{
  digitalWrite(solenoidPin, HIGH);
  delay(500); // how long the solenoid is open per pulse
  digitalWrite(solenoidPin, LOW);
  delay(75);
}
delay(50000);
}

/*
100 pulses @ 50 ms per pulse = 450 ul -> 4.5 ul per pulse
200 pulses @ 50 ms per pulse = 900 ul -> 4.5 ul per pulse

50 pulses @ 22 ms per pulse = 100 ul -> 2 ul per pulse (1)
400 pulses @ 22 ms per pulse = 700 ul -> 1.75 ul per pulse (2)
200 pulse @ 22 ms per pulse = 430 ul -> 2.15 ul per pulse (3)
100 pulses @ 22 ms per pulse = 200 ul -> 2 ul per pulse (4)
50 pulses @ 22 ms per pulse = 100 ul -> 2 ul per pulse (5)
300 pulses @ 22 ms per pulse = 400 ul -> 1.3 ul per pulse (6) 
300 pulses @ 22 ms per pulse = 400 ul -> 1.3 ul per pulse (7)
250 pulses @ 22 ms per pulse = 

250 pulse @ 44 ms per pulse = 1000 ul -> 4 ul per pulse
500 pulse @ 44 ms per pulse = 1600 ul -> 3.2 ul pulse
500 pulse @ 44 ms per pulse = 1800 ul -> 3.6 ul per pulse
100 pulse @ 44 ms per pulse = 400 ul -> 4 ul per pulse

100 pulse @ 67 ms per pulse = 900 ul -> 9 ul per pulse
100 pulse @ 50 ms per pulse = 550 ul -> 5.5 ul per pulse
100 pulse @ 55 ms per pulse = 700 ul -> 7 ul per pulse
100 pulse @ 52 ms per pulse = 600 ul -> 6 ul per pulse
100 pulse @ 52 ms per pulse = 600 ul -> 6 ul per pulse
50 pulse @ 52 ms per pulse = 300 ul -> 6 ul per pulse
150 pulse @ 52 ms per pulse = 850 ul -> 5.6 ul per pulse

100 pulse @ 60 ms per pulse = 800 ul -> 8 ul per pulse
50 pulse @ 60 ms per pulse = 400 ul -> 8 ul per pulse
150 pulse @ 60 ms per pulse = 1100 ul -> 7.3 per pulse
125 pulses @ 60 ms per pulse = 900 ul -> 7.2 per pulse

*/

/* Scale testing
22 ms -> 3 mg, 2 mg, 3 mg, 2 mg, 1 mg, 1 mg
44 ms -> 5 mg, 5 mg, 4 mg, 4 mg, 4 mg, 4 mg,  4 mg, 5 mg, 3 mg, 5 mg
52 ms -> 7 mg, 6 mg, 6 mg, 6 mg, 6 mg, 7 mg, 7 mg, 5 mg, 5 mg, 6 mg
60 ms -> 8 mg, 8 mg, 9 mg, 10 mg, 9 mg, 11 mg, 10 mg, 9 mg, 8 mg, 9 mg


*/