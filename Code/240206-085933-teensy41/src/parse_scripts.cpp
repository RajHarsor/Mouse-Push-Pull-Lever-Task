#include "parse_scripts.h"
#include "light_configs.h"

int programType;
int visualStage;
int holdTime;
int plusPercentage;
int lightsVerticalPercentage;
int lightsHorizontalPercentage;
int timeOutTime;
int SOpenTime;
int punishmentTime;
int isiDelayLowerRange;
int isiDelayUpperRange;

void parseProgramTypeInput() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &programType);
}

void parseVisualStage() {
  while (!Serial.available()) {
  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &visualStage);

  switch (visualStage) {
    case 1:
      holdTime = 50;
      timeOutTime = 10000;
      punishmentTime = 50;
      isiDelayLowerRange = 3000;
      isiDelayUpperRange = 3000;
      break;
    case 2:
      holdTime = 2000;
      plusPercentage = 50;
      lightsVerticalPercentage = 25;
      lightsHorizontalPercentage = 25;
      timeOutTime = 2000;
      SOpenTime = 52;
      punishmentTime = 100;
      isiDelayLowerRange = 250;
      isiDelayUpperRange = 250;
      break;
    case 3:
      holdTime = 3000;
      plusPercentage = 10;
      lightsVerticalPercentage = 45;
      lightsHorizontalPercentage = 45;
      timeOutTime = 2000;
      SOpenTime = 52;
      punishmentTime = 1000;
      isiDelayLowerRange = 250;
      isiDelayUpperRange = 250;
      break;
    case 4:
      holdTime = 4000;
      plusPercentage = 10;
      lightsVerticalPercentage = 45;
      lightsHorizontalPercentage = 45;
      timeOutTime = 1000;
      SOpenTime = 52;
      punishmentTime = 1000;
      isiDelayLowerRange = 250;
      isiDelayUpperRange = 250;
      break;
  }
}

void executeLightArray() {
  int totalPercentage = plusPercentage + lightsVerticalPercentage + lightsHorizontalPercentage;
  int randomValue = random(totalPercentage);

  if (randomValue <= plusPercentage) {
    lightArrayPlus();
  } else if (randomValue < (plusPercentage + lightsVerticalPercentage)) {
    lightArrayVeritical();
  } else {
    lightArrayHorizontal();
  }
}