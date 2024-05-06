#ifndef PARSE_INPUTS_H
#define PARSE_INPUTS_H

#include <Arduino.h>
#include "parse_program.h"

extern int PUSH;
extern int PULL; // please note this variable can be used for both pull and down
extern int DOWN;
extern int REST_LOWERBOUND_X;
extern int REST_UPPERBOUND_X;
extern int REST_LOWERBOUND_Y;
extern int REST_UPPERBOUND_Y;

void parseInputs();

#endif
