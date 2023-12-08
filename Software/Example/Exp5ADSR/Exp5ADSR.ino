/*
将ADSR包络应用于音频信号的示例与Mozzi声波库。这表明
如何在updateAudio（）中使用以AUDIO_RATE更新的ADSR，
并使用updateAudio（）中AUDIO_RATE处的next（）进行输出。


该文件夹中的另一个示例示出了在CONTROL_ RATE处的ADSR更新，
这更有效，但本例中显示的AUDIO_RATE更新
实现更快的包络转换。
演示使用控制的简单ADSR对象
noteOn（）和noteOff（）指令。
*/

#include <MozziGuts.h>
#include <Oscil.h>
#include <ADSR.h>
#include <tables/sin8192_int8.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>

Oscil<8192, AUDIO_RATE> aOscil(SIN8192_DATA);

// for triggering the envelope
// EventDelay noteDelay;

ADSR<AUDIO_RATE, AUDIO_RATE> envelope;

boolean note_is_on = true;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);  //将2引脚自动上拉电阻
  startMozzi();
}

int Vol = 128;
int GateOn = 0;
void updateControl() {
  // Serial.println(digitalRead(2));  //btn
  if (digitalRead(2) == 0&& GateOn != 1) {
    Serial.println("11111111111111111111111111");  //btn
    GateOn = 1;
    envelope.setADLevels(188, 188);
    envelope.setTimes(0, 1, 1, 1);                     //default
    // envelope.setTimes(analogRead(0), 127, 200, analogRead(0));  //analogRead(0)
    envelope.noteOn();
    byte midi_note = rand(107) + 20;
    aOscil.setFreq((int)mtof(midi_note));
    envelope.update();
  } else if (digitalRead(2) == 0 && GateOn == 1) {
    Serial.println("22222222222222");  //btn
  } else {
    Serial.println("33333");  //btn
    envelope.noteOff();
    GateOn = 0;
  }
  Vol = envelope.next();
}


int updateAudio() {
  return (int)(Vol * aOscil.next()) >> 8;
}


void loop() {
  audioHook();  // required here
}