
#include <MozziGuts.h>
#include <LowPassFilter.h>

#define KNOB_PIN 4

LowPassFilter lpf;


void setup() {
  Serial.begin(115200);
  lpf.setResonance(220);
  startMozzi();
}


void updateControl() {
  int knob = mozziAnalogRead(KNOB_PIN);
  byte cutoff_freq = knob >> 2;  // range 0-255
  lpf.setCutoffFreq(cutoff_freq);
}


int updateAudio() {
  // subtracting 512 moves the unsigned audio data into 0-centred,
  // signed range required by all Mozzi units
  // int asig = getAudioInput() - 512;
  // int asig = mozziAnalogRead(0) - 512;
  Serial.println(asig);

  asig = lpf.next(asig >> 1);

  return asig;
}


void loop() {
  audioHook();
}