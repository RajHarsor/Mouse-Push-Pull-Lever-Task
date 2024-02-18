#ifndef HOLD_TIME_VISUAL_PUNISHMENT_H
#define HOLD_TIME_VISUAL_PUNISHMENT_H

#include <Arduino.h>
#include "parse_scripts.h"

/* #region Soundboard setup*/
#include <Adafruit_Soundboard.h>

// Choose any two pins that can be used with SoftwareSerial to RX & TX
// #define SFX_TX 5
// #define SFX_RX 6


// Connect to the RST pin on the Sound Board
#define SFX_RST 19

// Hardware Serial Communication Set Up
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

/* #endregion */

void holdTimeVisualPunishment();

#endif