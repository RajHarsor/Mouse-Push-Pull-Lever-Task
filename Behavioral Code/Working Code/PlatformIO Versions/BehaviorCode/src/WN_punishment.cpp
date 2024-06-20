#include "WN_punishment.h"

void wnPunishment(int punishmentTimeVar) {
  digitalWrite(28, HIGH);
  sfx.playTrack("T04     WAV");
  delay(120 + punishmentTimeVar);
  sfx.stop();
  digitalWrite(28, LOW);
}