
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable

Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> osc2(WHITENOISE8192_DATA);  // audio noise
ADSR<AUDIO_RATE, AUDIO_RATE> env2;
LowPassFilter lpf2;
int gain2;
int trg2 = 0;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  // use float to set freq because it will be small and fractional
  pinMode(12, INPUT_PULLUP);
}


void updateControl() {
  // Serial.print("trg2=");
  // Serial.print(trg2);
  // Serial.print(" d12=");
  // Serial.println(digitalRead(12));
  osc2.setFreq((float)AUDIO_RATE / WHITENOISE8192_SAMPLERATE);

  lpf2.setCutoffFreq(mozziAnalogRead(4) << 2);
  lpf2.setResonance(256);

  env2.setADLevels(255, 255);
  env2.setTimes(0, 16, 0, 2);
  if (digitalRead(12) == 0 && trg2 == 0) {
    trg2 = 1;
    env2.noteOn();
  }
  if (digitalRead(12) == 1 && trg2 == 1) {
    trg2 = 0;
    env2.noteOff();
  }
  env2.update();

  gain2 = env2.next();
  // for (int i = 0; i < gain2 / 2; i++)
  //   Serial.print("]");
}


int updateAudio() {
  return gain2 * lpf2.next(osc2.next()) >> 8;
}


void loop() {
  audioHook();
}