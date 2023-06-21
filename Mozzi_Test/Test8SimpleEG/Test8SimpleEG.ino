#include "Module_SimpleEG.h"

void setup() {
  initEG(11, 10);
}

void loop() {
  generateEG(analogRead(1), analogRead(2), analogRead(4));
}
