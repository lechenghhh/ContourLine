#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#define CONTROL_RATE 128
Oscil<2048, AUDIO_RATE> aSin(SIN2048_DATA);
Oscil<2048, CONTROL_RATE> kVib(SIN2048_DATA);

float centre_freq = 440.0;
float depth = 0.25;
byte volume = 88;  //音量范围 0-255

void setup() {
  Serial.begin(115200);  // for Teensy 3.1, beware printout can cause glitches
  kVib.setFreq(6.5f);
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  float vibrato = depth * kVib.next();
  aSin.setFreq(centre_freq + vibrato);
  int sensor_value = mozziAnalogRead(0);  // value is 0-1023
  // int sensor_value = 3;                   // value is 0-1023

  volume = map(sensor_value, 0, 1023, 0, 255);
  Serial.print("volume = ");
  Serial.println((int)volume);
}

int updateAudio() {
  return ((int)aSin.next() * volume) >> 8;  // shift back into range after multiplying by 8 bit value乘以 8 位值后移回范围
}

void loop() {
  audioHook();
}