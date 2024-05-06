#include "parse_inputs.h"

int PUSH;
int PULL;
int DOWN;
int REST_LOWERBOUND_X;
int REST_UPPERBOUND_X;
int REST_LOWERBOUND_Y;
int REST_UPPERBOUND_Y;

void parseInputs() {
    switch (programType) {
        case 1: {
            Serial.println("Enter the threshold values for the Push/Pull Program (Push, Pull, Rest LowerBound X, Rest UpperBound X)");
            while (!Serial.available()) {
            }
            String input = Serial.readStringUntil('\n');
            int numValues = sscanf(input.c_str(), "%d, %d, %d, %d", &PUSH, &PULL, &REST_LOWERBOUND_X, &REST_UPPERBOUND_X);
            break;
        }
        case 2: {
            Serial.println("Enter the threshold values for the Push/Down Program (Push, Down, Rest LowerBound X, Rest UpperBound X, Rest LowerBound Y, Rest UpperBound Y)");
            while (!Serial.available()) {
            }
            String input = Serial.readStringUntil('\n');
            int numValues = sscanf(input.c_str(), "%d, %d, %d, %d, %d, %d", &PUSH, &DOWN, &REST_LOWERBOUND_X, &REST_UPPERBOUND_X, &REST_LOWERBOUND_Y, &REST_UPPERBOUND_Y);
            break;
        }
    }
}