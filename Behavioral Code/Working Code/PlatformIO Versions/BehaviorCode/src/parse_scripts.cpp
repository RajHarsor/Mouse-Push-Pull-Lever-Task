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

void parseVisualStage() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &visualStage);

  switch (visualStage) {
    case 1:
      holdTime = 50; // this is a dynamic variable in the code
      timeOutTime = 1000;
      punishmentTime = 0;
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
  }
}