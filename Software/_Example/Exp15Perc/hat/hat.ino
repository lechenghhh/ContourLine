
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <tables/triangle512_int8.h>

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable

Oscil<181, AUDIO_RATE> osc4(TRIANGLE512_DATA);
Oscil<103, AUDIO_RATE> osc5(TRIANGLE512_DATA);

ADSR<AUDIO_RATE, AUDIO_RATE> env4;
int gain4;
int trg4 = 0;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  pinMode(12, INPUT_PULLUP);
}

void updateControl() {
  int hihatfreq = mozziAnalogRead(4) << 4;
  // Serial.print("trg4=");
  // Serial.print(trg4);

  env4.setADLevels(255, 255);
  env4.setTimes(0, 8, 0, 2);
  if (digitalRead(12) == 0 && trg4 == 0) {
    trg4 = 1;
    env4.noteOn();
    env4.noteOff();
  }
  if (digitalRead(12) == 1 && trg4 == 1) {
    trg4 = 0;
  }
  env4.update();
  gain4 = env4.next();

  osc4.setFreq(hihatfreq);
  osc5.setFreq(4000);
}

int updateAudio() {
  int fm = 600 * osc5.next() >> 8;
  return gain4 * (osc4.phMod(fm)) >> 8;
}

void loop() {
  audioHook();
}