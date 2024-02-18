#include "hold_time_visual_punishment.h"


void holdTimeVisualPunishment() {
    switch (visualStage) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            sfx.playTrack("T04     WAV");
            delay(120 + holdTimePunishment);
            sfx.stop();
            break;
        case 4:
            sfx.playTrack("T04     WAV");
            delay(120 + holdTimePunishment);
            sfx.stop();
            break;
            }
        }