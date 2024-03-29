
#include <MozziGuts.h>
#include <Oscil.h>                // oscillator template
#include <tables/sin2048_int8.h>  // sine table for oscillator


//该例程用于测试振荡器复位的功能
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率

  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  startMozzi(128);  // :)
}

int WaveTrig = 0;

void updateControl() {
  Serial.print("WaveTrig");
  Serial.println(WaveTrig);
  // Serial.print("d13-");
  // Serial.println(digitalRead(13));
  Serial.print("d12-");
  Serial.println(digitalRead(12));

  aSin.setFreq(analogRead(4)>>3);

  if (digitalRead(12) != WaveTrig && WaveTrig == 0) {  //d13按钮可以用来测试
    WaveTrig = !WaveTrig;
  }
  if (digitalRead(12) != WaveTrig && WaveTrig == 1) {
    WaveTrig = !WaveTrig;
  }
}

int updateAudio() {
  if (WaveTrig == 0) aSin.setPhase(0);
  return aSin.next();
}

void loop() {
  audioHook();
}