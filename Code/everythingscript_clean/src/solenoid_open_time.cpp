#include "solenoid_open_time.h"

const int ArraySize = 6; // set the size of the array to 6
int OpenTimeArray[ArraySize];  // initialize a blank array with 10 slots
int currentIndex = 0;  // initialize the current index of the array to 0
int ArrayCount1 = 0; // initialize the number of 1's in the array to 0
int ArrayCount2 = 0; // initialize the number of 2's in the array to 0
int OpenTime; // variable to store the solenoid open time


void solenoidOpenTime() {  /// function to determine the solenoid open time
  if (currentIndex < ArraySize) {    // if there aren't 10 integers in the array, add to it until there are 10
    if (decision == 1) {  // if a mouse did a push add a 1 to the array
      OpenTimeArray[currentIndex] = 1;
      currentIndex++;
    }
    if (decision == 2) {  // if a mouse does a pull add a 2 to the array
      OpenTimeArray[currentIndex] = 2;
      currentIndex++;
    }
  } else {                          // if the array is full
    for (int i = ArraySize - 2; i >= 0; i--) {  // shift all the values in the array down one
      OpenTimeArray[i + 1] = OpenTimeArray[i];
    }
    if (decision == 1) { // if a mouse did a push add a 1 to the array at position 0
      OpenTimeArray[0] = 1;
    }
    if (decision == 2) { // if a mouse did a pull add a 2 to the array at position 0
      OpenTimeArray[0] = 2;
    }
  }
/*   Serial.print("[");  // print the current array of pushes and pulls
  for (int i = 0; i < ArraySize; i++) {
    Serial.print(OpenTimeArray[i]);
    Serial.print(" ");
  }
  Serial.print("]");
  Serial.print(" , ");
  Serial.print(" "); */
  for (int i = 0; i < ArraySize; i++) {
    if (OpenTimeArray[i] == 1) {  // Count the number of 1s in the array (pushes)
      ArrayCount1 = ArrayCount1 + 1;
    }
    if (OpenTimeArray[i] == 2) {  // Count the number of 2s in the array (pulls)
      ArrayCount2 = ArrayCount2 + 1;
    }
  } 
  // Serial.print("Current F/R Ratio = ");  // Print the current F/R ratio (pushes/pulls)
 /*  Serial.print(ArrayCount1);
  Serial.print(" / ");
  Serial.print(ArrayCount2);
  Serial.print(" , ");

  Serial.print(positionA);
  Serial.print(" / ");
  Serial.print(positionB);
  Serial.print(" , "); */

  if (decision == 1) {  // Push Equation
    if (ArrayCount1 == ArraySize or ArrayCount2 == ArraySize) {
      OpenTime = 104;
      /* Serial.print(OpenTime);
      Serial.print(" , "); */
      delay(OpenTime);
    } else {
    //OpenTime = ((-7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) - 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28)) * 2;
    OpenTime = 104;
    // Serial.print("Solenoid Open Time = ");
   /*  Serial.print(OpenTime);
    Serial.print(" , "); */
    // currentMillis = millis();
    // while (timerMillis <= OpenTime) {
    //   continue;
    delay(OpenTime);
    }
    }

  if (decision == 2) {  // Pull Equation
    if (ArrayCount1 == ArraySize or ArrayCount2 == ArraySize) {
      OpenTime = 104;
      // Serial.print(OpenTime);
     //  Serial.print(" , ");
      delay(OpenTime);
    } else {
      //OpenTime = ((-7.83 * (ArrayCount1 - ArrayCount2) / pow((ArrayCount1 + ArrayCount2), 2) + 18.44 * (ArrayCount1 - ArrayCount2) / (ArrayCount1 + ArrayCount2) + 48.28)) * 2;
      OpenTime = 104;
    // Serial.print("Solenoid Open Time = ");
    // Serial.print(OpenTime);
    // Serial.print(" , ");
    // currentMillis = millis();
    // while (timerMillis <= OpenTime) {
    //   continue;
    // }
    delay(OpenTime);
    }
  }
  ArrayCount1 = 0; // Reset the number of 1s in the array to 0
  ArrayCount2 = 0; // Reset the number of 2s in the array to 0
}