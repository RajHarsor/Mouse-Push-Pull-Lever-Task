#include "parse_program.h"

int programType;

void parseProgram() {
    while (!Serial.available()) {

  }

  String input = Serial.readStringUntil('\n');
  int numValues = sscanf(input.c_str(), "%d", &programType);
}