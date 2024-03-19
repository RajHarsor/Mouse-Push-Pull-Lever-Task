#include "parse_scripts.h"
#include "light_configs.h"

int programType;
int visualStage;
int holdTime;
int plusPercentage;
int motorTaskType;
int CueEvokedTaskDay;
int lightsVerticalPercentage;
int lightsHorizontalPercentage;
int timeOutTime;
int SOpenTime;
int punishmentTime;
int isiDelayLowerRange;
int isiDelayUpperRange;
int holdTimePunishment;
int soundCueDuration;
bool correct = false;

void parseProgramTypeInput() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &programType);
}

void parseMotorTaskType() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &motorTaskType);
  switch (motorTaskType) {
    case 1:
      Serial.println("What day is it?");
      Serial.println("Type 1 if Push Only (Day 1)");
      Serial.println("Type 2 if Pull Only (Day 2)");
      Serial.println("Type 3 if Push and Pull (Day 3 - Dynamic Water)");
      parseCueEvokedTaskDay();
      break;
    case 2:
      Serial.println("Input the isiDelayLowerRange, isiDelayUpperRange, and punishmentLength");
      // TODO Need to add another parse function for this
      break;
    case 3:
      // TODO Do this whole section
      break; 
  }
}

void parseCueEvokedTaskDay() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &CueEvokedTaskDay);

  switch (CueEvokedTaskDay) {
    case 1:
      holdTime = 50;
      soundCueDuration = 50;
      timeOutTime = 2000;
      SOpenTime = 44;
      isiDelayLowerRange = 250;
      isiDelayUpperRange = 1000;
      punishmentTime = 50;
      break;
    case 2:
      holdTime = 50;
      soundCueDuration = 50;
      timeOutTime = 2000;
      SOpenTime = 44;
      isiDelayLowerRange = 1000;
      isiDelayUpperRange = 2000;
      punishmentTime = 100;
      break;
    case 3:
      holdTime = 50;
      soundCueDuration = 50;
      timeOutTime = 2000;
      isiDelayLowerRange = 1000;
      isiDelayUpperRange = 3000;
      punishmentTime = 500;
      break;
  }
  }

void parseVisualStage() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &visualStage);

  switch (visualStage) {
    case 1:
      holdTime = 50; // this is a dynamic variable in the code
      timeOutTime = 2000;
      punishmentTime = 50;
      isiDelayLowerRange = 250;
      isiDelayUpperRange = 300;
      break;
    case 2:
      holdTime = 250;
      plusPercentage = 50;
      lightsVerticalPercentage = 25;
      lightsHorizontalPercentage = 25;
      timeOutTime = 2000;
      SOpenTime = 52; // ~6 ul of water
      punishmentTime = 100;
      isiDelayLowerRange = 1600;
      isiDelayUpperRange = 2400;
      break;
    case 3:
      holdTime = 500;
      plusPercentage = 10;
      lightsVerticalPercentage = 45;
      lightsHorizontalPercentage = 45;
      timeOutTime = 2000;
      SOpenTime = 52;
      punishmentTime = 250;
      isiDelayLowerRange = 1000;
      isiDelayUpperRange = 3000;
      holdTimePunishment = 100;
      break;
    case 4:
      holdTime = 500;
      plusPercentage = 10;
      lightsVerticalPercentage = 45;
      lightsHorizontalPercentage = 45;
      timeOutTime = 1000;
      SOpenTime = 52;
      punishmentTime = 1000;
      isiDelayLowerRange = 1000;
      isiDelayUpperRange = 3000;
      holdTimePunishment = 100;
      break;
  }
}