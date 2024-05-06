#ifndef HOLD_TIME_MOD_VISUAL_H
#define HOLD_TIME_MOD_VISUAL_H

#include <Arduino.h>
#include "parse_scripts.h"


extern int holdTime;
extern int programType;
extern bool correct;

void holdTimeVisualTask();

#endif