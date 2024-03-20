#ifndef LIGHT_CONFIGS_H
#define LIGHT_CONFIGS_H

#include <Arduino.h>
#include <FastLED.h>
#include "parse_scripts.h"

extern int lightDecision;
extern int plusPercentage;
extern int lightsVerticalPercentage;
extern int lightsHorizontalPercentage;

#ifndef NUM_LEDS
// #define NUM_LEDS 48  //number of LEDs total
#define NUM_LEDS 64 //for the 8x8 matrix
#endif

extern CRGB leds[NUM_LEDS];  //define the array of LEDs

void lightArrayPlus();
void lightArrayVeritical();
void lightArrayHorizontal();
void executeLightArray();
void lightsOff();
void lightArrayModifier();
void turnOnTheLights();

#endif