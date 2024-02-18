#include "parse_scripts.h"
#include "hold_time_mod_visual.h"

void holdTimeVisualTask() {
    switch (visualStage) {
        case 1:
            if (holdTime < 250 && correct == true) {
                holdTime += 2;
                }
                break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
    }
        }