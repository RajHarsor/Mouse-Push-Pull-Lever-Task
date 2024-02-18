#include "reaction_time_mod_visual.h"

void reactionTimeWindowModVisual() {
    switch (visualStage){
        case 1:
            break;
        case 2:
            break;
        case 3:
            if (timeOutTime > 1000 && correct == true) {
            timeOutTime = timeOutTime - 10;
            }
            break;
        case 4:
            break;
        }
}