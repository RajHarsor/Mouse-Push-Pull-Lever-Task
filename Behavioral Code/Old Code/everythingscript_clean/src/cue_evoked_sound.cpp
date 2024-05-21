#include "cue_evoked_sound.h"

void play12kHzSound(int soundCueDuration) {
    sfx.playTrack("T05     WAV");
    delay(120 + soundCueDuration);
    sfx.stop();
}