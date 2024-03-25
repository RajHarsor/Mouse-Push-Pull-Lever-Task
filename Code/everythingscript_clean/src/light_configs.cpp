#include "light_configs.h"
#include <FastLED.h>

int lightDecision;
bool lightTreedixLEDs = true;


void lightArrayPlus() { // For the Treedix (6 x 8 matrix)
    for (int i = 8; i <= 32; i = i + 6) {
        leds[i] = CRGB::Blue;
        FastLED.show();
    }

    for (int i = 18; i <= 22; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
    }
    lightDecision = 0;
}

void lightArrayPlus1() { // For the NeoPixel 8x8 matrix
  for (int i = 19; i <= 51; i = i + 8) {
    leds[i] = CRGB::Blue;
    FastLED.show();
    }

  for (int i = 33; i <= 37; i++) {
    leds[i] = CRGB::Blue;
    FastLED.show();
  }
  }

void lightArrayVeritical() {  // please note this function does 1 stripe
    for (int i = 8; i <= 32; i = i + 6) {
        leds[i] = CRGB::Blue;
        FastLED.show();
    }
    lightDecision = 1;
}

void lightArrayHorizontal() {  // please note this function does 1 stripe
    for (int i = 18; i <= 22; i++) {
        leds[i] = CRGB::Blue;
        FastLED.show();
    }
    lightDecision = 2;
}

void lightsOff() {
    for (int i = 0; i <= 64; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    }
}

void executeLightArray() {
  int totalPercentage = plusPercentage + lightsVerticalPercentage + lightsHorizontalPercentage;
  int randomValue = random(totalPercentage);

  if (randomValue <= plusPercentage) {
    lightArrayPlus();
  }
  else if (randomValue <= plusPercentage + lightsVerticalPercentage) {
    lightArrayVeritical();
  }
  else {
    lightArrayHorizontal();
  }
}

void lightArrayModifier() {
  switch (visualStage) {
    case 1: case 3: case 4:
      break;
    case 2:
      if (plusPercentage > 10 && correct == true) {
        plusPercentage = 50 - (CorrectCounter * 0.4);
        lightsVerticalPercentage = 25 + (CorrectCounter * 0.2);
        lightsHorizontalPercentage = 25 + (CorrectCounter * 0.2);
      }
      break;
      }
}

void turnOnTheLights() {
  switch (visualStage) {
    case 1:
      if (lightTreedixLEDs == true) {
        lightArrayPlus();
      }
      else {
        lightArrayPlus1();
      break;
    case 2: case 3: case 4:
      executeLightArray();
      break;
      }
}
}