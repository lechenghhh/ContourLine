
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <tables/square_no_alias512_int8.h>

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable

Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> osc3(SQUARE_NO_ALIAS512_DATA);

ADSR<AUDIO_RATE, AUDIO_RATE> env3;

int gain3;
int trg3 = 0;
int fall3 = 0;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  pinMode(12, INPUT_PULLUP);
}

void updateControl() {
  int tomFreq = 64 + mozziAnalogRead(4) >> 4;
  // Serial.print(" tomFreq=");
  // Serial.print(tomFreq);
  // Serial.print(" d12=");
  // Serial.println(digitalRead(12));
  // Serial.print("trg3=");
  // Serial.print(trg3);

  env3.setADLevels(255, 255);
  env3.setTimes(0, 16, 0, 2);
  if (digitalRead(12) == 0 && trg3 == 0) {
    trg3 = 1;
    fall3 = 30;
    env3.noteOn();
    env3.noteOff();
  }
  if (digitalRead(12) == 1 && trg3 == 1) {
    trg3 = 0;
  }
  env3.update();
  gain3 = env3.next();

  osc3.setFreq(tomFreq + fall3);
  fall3 -= 1;
  // gain3 = 255;//test

  // for (int i = 0; i < fall3; i++)
  //   Serial.print("]");
}

int updateAudio() {
  return gain3 * (osc3.next()) >> 8;
}

void loop() {
  audioHook();
}