#include "punishment_time_visual_mod.h"

void punishmentVisualTaskChooser() {
    switch (visualStage) {
    case 1: case 2: case 4:
        wnPunishment(punishmentTime);
        break;
    case 3:
        if (punishmentTime <= 1000 && correct == false) {
        punishmentTime+= 10;
        }
        wnPunishment(punishmentTime);
        break;
    }
}