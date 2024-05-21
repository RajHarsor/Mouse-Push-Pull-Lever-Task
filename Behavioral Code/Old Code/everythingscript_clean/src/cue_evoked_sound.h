#ifndef CUE_EVOKED_SOUND_H
#define CUE_EVOKED_SOUND_H

#include <Arduino.h>
#include "hold_time_visual_punishment.h"

extern Adafruit_Soundboard sfx;

void play12kHzSound(int soundCueDuration);

#endif