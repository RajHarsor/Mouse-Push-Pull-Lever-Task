#include "light_configs.h"
#include <FastLED.h>

int lightDecision;

void lightArrayPlus() {
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
    for (int i = 2; i <= 44; i = i + 6) {
    leds[i] = CRGB::Black;
    FastLED.show();
    }

    for (int i = 3; i <= 45; i = i + 6) {
    leds[i] = CRGB::Black;
    FastLED.show();
    }

    for (int i = 18; i <= 23; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    }

    for (int i = 24; i <= 29; i++) {
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
    case 1:
      break;
    case 2:
      if (plusPercentage > 10 && correct == true) {
        plusPercentage = 50 - (CorrectCounter * 0.4);
        lightsVerticalPercentage = 25 + (CorrectCounter * 0.2);
        lightsHorizontalPercentage = 25 + (CorrectCounter * 0.2);
      }
      break;
    case 3:
      break;
    case 4:
      break;
      }
}