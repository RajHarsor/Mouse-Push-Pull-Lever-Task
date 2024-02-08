#ifndef SOLENOID_OPEN_TIME_H
#define SOLENOID_OPEN_TIME_H

extern int decision;
extern int positionA;
extern int positionB;
extern unsigned long currentMillis;
extern unsigned long timerMillis;
extern int OpenTime;
extern int ArrayCount1;
extern int ArrayCount2;

#include <Arduino.h>

void solenoidOpenTime();

#endif