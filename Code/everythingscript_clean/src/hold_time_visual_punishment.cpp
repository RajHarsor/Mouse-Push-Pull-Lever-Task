#include "hold_time_visual_punishment.h"

Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial1, NULL, SFX_RST);

void holdTimeVisualPunishment() {
    switch (visualStage) {
        case 1:
            break;
        case 2:
            break;
        case 3: case 4:
            sfx.playTrack("T04     WAV");
            delay(120 + holdTimePunishment);
            sfx.stop();
            break;
            }
        }