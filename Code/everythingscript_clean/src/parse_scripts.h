#ifndef PARSE_SCRIPTS_H
#define PARSE_SCRIPTS_H

#include <Arduino.h>

extern int programType;
extern int visualStage;
extern int motorTaskType;
extern int soundCueDuration;
extern int CueEvokedTaskDay;
extern int holdTime;
extern int trialNumber;
extern int plusPercentage;
extern int lightsVerticalPercentage;
extern int lightsHorizontalPercentage;
extern int timeOutTime;
extern int SOpenTime;
extern int punishmentTime;
extern int isiDelayLowerRange;
extern int isiDelayUpperRange;
extern int CorrectCounter;
extern int holdTimePunishment;
extern bool correct;


void parseProgramTypeInput();

void parseVisualStage();

void parseMotorTaskType();

void parseCueEvokedTaskDay();

#endif