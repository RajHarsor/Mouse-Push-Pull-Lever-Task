#ifndef LIGHT_CONFIGS_H
#define LIGHT_CONFIGS_H

#include <Arduino.h>
#include <FastLED.h>

extern int lightDecision;

#ifndef NUM_LEDS
#define NUM_LEDS 48  //number of LEDs total
#endif

extern CRGB leds[NUM_LEDS];  //define the array of LEDs

void lightArrayPlus();
void lightArrayVeritical();
void lightArrayHorizontal();
void lightsOff();

#endif