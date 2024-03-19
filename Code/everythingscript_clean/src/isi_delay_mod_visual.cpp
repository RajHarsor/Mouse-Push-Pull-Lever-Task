#include "isi_delay_mod_visual.h"

void isiDelayModVisual() {
    switch (visualStage) {
            case 1:
                if (isiDelayUpperRange < 2500 && correct == true) {
                    isiDelayUpperRange += 22;
            }
                break;
            case 2: case 3: case 4:
                break;
        }
    }