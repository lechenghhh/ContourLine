
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <tables/triangle512_int8.h>

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable

Oscil<TRIANGLE512_NUM_CELLS, AUDIO_RATE> osc1(TRIANGLE512_DATA);

ADSR<AUDIO_RATE, AUDIO_RATE> env1;

int gain1;
int tfg1 = 0;
int fall1 = 0;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  pinMode(12, INPUT_PULLUP);
}

void updateControl() {
  int kickfreq = mozziAnalogRead(4) >> 4;
  // Serial.print(" kickfreq=");
  // Serial.print(kickfreq);
  // Serial.print(" d12=");
  // Serial.println(digitalRead(12));
  // Serial.print("tfg1=");
  // Serial.print(tfg1);

  env1.setADLevels(255, 255);
  env1.setTimes(0, 16, 0, 2);
  if (digitalRead(12) == 0 && tfg1 == 0) {
    tfg1 = 1;
    fall1 = 30;
    env1.noteOn();
    env1.noteOff();
  }
  if (digitalRead(12) == 1 && tfg1 == 1) {
    tfg1 = 0;
  }
  env1.update();
  gain1 = env1.next();

  osc1.setFreq(kickfreq + fall1);
  fall1 -= 3;
}

int updateAudio() {
  return gain1 * (osc1.next()) >> 8;
}

void loop() {
  audioHook();
}