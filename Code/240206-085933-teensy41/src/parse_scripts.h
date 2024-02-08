#ifndef PARSE_SCRIPTS_H
#define PARSE_SCRIPTS_H

#include <Arduino.h>

extern int programType;
extern int visualStage;
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


void parseProgramTypeInput();

void parseVisualStage();

void executeLightArray();

#endif