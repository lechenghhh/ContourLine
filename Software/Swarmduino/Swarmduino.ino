/*
  Swarmduino
  Thanks to Sound Simulator on YouTube for the idea
  https://www.youtube.com/watch?v=0H5wQOUrlIY&t=227s
*/

#include <MozziGuts.h>
#include <Oscil.h>  // oscillator template
#include <tables/square_analogue512_int8.h>
#include <tables/sin512_int8.h>
#include <tables/saw_analogue512_int8.h>
#include <tables/waveshape1_softclip_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/phasor256_int8.h>
#include <tables/cosphase256_int8.h>
#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable

// harmonics
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc1(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc2(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc3(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc4(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc5(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc6(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc7(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> osc8(SQUARE_ANALOGUE512_DATA);

// volume controls
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol1(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol2(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol3(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol4(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol5(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol6(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol7(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, CONTROL_RATE> kVol8(SIN512_DATA);

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> vibrato(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> tremolo(SIN512_DATA);

int PWMPin = 9;

char v1, v2, v3, v4, v5, v6, v7, v8;
byte POSITION = 0;
#define PARAM_LENGTH 5
char param_name[PARAM_LENGTH][6] = { "Freq", "Detune", "Tremo", "TSape", "WaveT" };
int param[PARAM_LENGTH] = { 0, 1, 32, 3, 0 };
bool* ledGroup[PARAM_LENGTH] = { Led_F, Led_D, Led_T, Led_S, Led_W };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率

  initCtrl(4, 50, 13, 12, 1);
  initLED(2, 3, 4, 5, 6, 7, 8);  //初始化Led引脚
  pinMode(11, INPUT);
  // set harmonic frequencies
  osc1.setFreq(100);
  osc2.setFreq(100);
  osc3.setFreq(100);
  osc4.setFreq(100);
  osc5.setFreq(100);
  osc6.setFreq(100);
  osc7.setFreq(100);
  osc8.setFreq(100);

  v1 = v2 = v3 = v4 = v5 = v6 = v7 = v8 = 127;
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  // Serial.print(POSITION);           //func param
  // Serial.println("func");           //func param
  // Serial.println(param[POSITION]);  //func param
  // Serial.println(" ");

  //tone & detune_span
  float Tone = map(param[0] + mozziAnalogRead(0), 0, 669, 55.0, 660.0);
  float detune_span = param[1] + mozziAnalogRead(1);
  osc1.setFreq(Tone);
  osc2.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.1)));
  osc3.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.2)));
  osc4.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.3)));
  osc5.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.4)));
  osc6.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.5)));
  osc7.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.25)));
  osc8.setFreq(Tone + (float)(Tone * mapf(detune_span, 0, 699, 0, 1.26)));

  // set volume change frequencies
  float tremolo = (param[2] + mozziAnalogRead(2)) /32;
  kVol1.setFreq(0.42f * tremolo);  // more of a pulse // change for pulsating while mute
  kVol2.setFreq(0.0245f * tremolo);
  kVol3.setFreq(0.019f * tremolo);
  kVol4.setFreq(0.07f * tremolo);
  kVol5.setFreq(0.047f * tremolo);
  kVol6.setFreq(0.031f * tremolo);
  kVol7.setFreq(0.0717f * tremolo);
  kVol8.setFreq(0.041f * tremolo);
  v1 = kVol1.next();
  v2 = kVol2.next();
  v3 = kVol3.next();
  v4 = kVol4.next();
  v5 = kVol5.next();
  v6 = kVol6.next();
  v7 = kVol7.next();
  v8 = kVol8.next();

  // set Tremolo WaveType
  switch (param[3] >> 7) {
    default:
      kVol1.setTable(SIN512_DATA);
      kVol2.setTable(SIN512_DATA);
      kVol3.setTable(SIN512_DATA);
      kVol4.setTable(SIN512_DATA);
      kVol5.setTable(SIN512_DATA);
      kVol6.setTable(SIN512_DATA);
      kVol7.setTable(SIN512_DATA);
      kVol8.setTable(SIN512_DATA);

      break;
    case 1:
      kVol1.setTable(SQUARE_ANALOGUE512_DATA);
      kVol2.setTable(SQUARE_ANALOGUE512_DATA);
      kVol3.setTable(SQUARE_ANALOGUE512_DATA);
      kVol4.setTable(SQUARE_ANALOGUE512_DATA);
      kVol5.setTable(SQUARE_ANALOGUE512_DATA);
      kVol6.setTable(SQUARE_ANALOGUE512_DATA);
      kVol7.setTable(SQUARE_ANALOGUE512_DATA);
      kVol8.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 2:
      kVol1.setTable(HALFSIN256_DATA);
      kVol2.setTable(HALFSIN256_DATA);
      kVol3.setTable(HALFSIN256_DATA);
      kVol4.setTable(HALFSIN256_DATA);
      kVol5.setTable(HALFSIN256_DATA);
      kVol6.setTable(HALFSIN256_DATA);
      kVol7.setTable(HALFSIN256_DATA);
      kVol8.setTable(HALFSIN256_DATA);
      break;
    case 3:
      kVol1.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol2.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol3.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol4.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol5.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol6.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol7.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      kVol8.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      break;
    case 4:
      kVol1.setTable(TRIANGLE512_DATA);
      kVol2.setTable(TRIANGLE512_DATA);
      kVol3.setTable(TRIANGLE512_DATA);
      kVol4.setTable(TRIANGLE512_DATA);
      kVol5.setTable(TRIANGLE512_DATA);
      kVol6.setTable(TRIANGLE512_DATA);
      kVol7.setTable(TRIANGLE512_DATA);
      kVol8.setTable(TRIANGLE512_DATA);
      break;
    case 5:
      kVol1.setTable(SAW_ANALOGUE512_DATA);
      kVol2.setTable(SAW_ANALOGUE512_DATA);
      kVol3.setTable(SAW_ANALOGUE512_DATA);
      kVol4.setTable(SAW_ANALOGUE512_DATA);
      kVol5.setTable(SAW_ANALOGUE512_DATA);
      kVol6.setTable(SAW_ANALOGUE512_DATA);
      kVol7.setTable(SAW_ANALOGUE512_DATA);
      kVol8.setTable(SAW_ANALOGUE512_DATA);
      break;
    case 6:
      kVol1.setTable(PHASOR256_DATA);
      kVol2.setTable(PHASOR256_DATA);
      kVol3.setTable(PHASOR256_DATA);
      kVol4.setTable(PHASOR256_DATA);
      kVol5.setTable(PHASOR256_DATA);
      kVol6.setTable(PHASOR256_DATA);
      kVol7.setTable(PHASOR256_DATA);
      kVol8.setTable(PHASOR256_DATA);
      break;
    case 7:
      kVol1.setTable(COSPHASE256_DATA);
      kVol2.setTable(COSPHASE256_DATA);
      kVol3.setTable(COSPHASE256_DATA);
      kVol4.setTable(COSPHASE256_DATA);
      kVol5.setTable(COSPHASE256_DATA);
      kVol6.setTable(COSPHASE256_DATA);
      kVol7.setTable(COSPHASE256_DATA);
      kVol8.setTable(COSPHASE256_DATA);
      break;
  }
  // setWaveType
  switch (param[4] >> 7) {
    default:
      osc1.setTable(SAW_ANALOGUE512_DATA);
      osc2.setTable(SAW_ANALOGUE512_DATA);
      osc3.setTable(SAW_ANALOGUE512_DATA);
      osc4.setTable(SAW_ANALOGUE512_DATA);
      osc5.setTable(SAW_ANALOGUE512_DATA);
      osc6.setTable(SAW_ANALOGUE512_DATA);
      osc7.setTable(SAW_ANALOGUE512_DATA);
      osc8.setTable(SAW_ANALOGUE512_DATA);
      break;
    case 1:
      osc1.setTable(SQUARE_ANALOGUE512_DATA);
      osc2.setTable(SQUARE_ANALOGUE512_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(SQUARE_ANALOGUE512_DATA);
      osc5.setTable(SQUARE_ANALOGUE512_DATA);
      osc6.setTable(SQUARE_ANALOGUE512_DATA);
      osc7.setTable(SQUARE_ANALOGUE512_DATA);
      osc8.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 2:
      osc1.setTable(HALFSIN256_DATA);
      osc2.setTable(HALFSIN256_DATA);
      osc3.setTable(HALFSIN256_DATA);
      osc4.setTable(HALFSIN256_DATA);
      osc5.setTable(HALFSIN256_DATA);
      osc6.setTable(HALFSIN256_DATA);
      osc7.setTable(HALFSIN256_DATA);
      osc8.setTable(HALFSIN256_DATA);
      break;
    case 3:
      osc1.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc2.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc3.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc4.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc5.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc6.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc7.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      osc8.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      break;
    case 4:
      osc1.setTable(TRIANGLE512_DATA);
      osc2.setTable(TRIANGLE512_DATA);
      osc3.setTable(TRIANGLE512_DATA);
      osc4.setTable(TRIANGLE512_DATA);
      osc5.setTable(TRIANGLE512_DATA);
      osc6.setTable(TRIANGLE512_DATA);
      osc7.setTable(TRIANGLE512_DATA);
      osc8.setTable(TRIANGLE512_DATA);
      break;
    case 5:
      osc1.setTable(SIN512_DATA);
      osc2.setTable(SIN512_DATA);
      osc3.setTable(SIN512_DATA);
      osc4.setTable(SIN512_DATA);
      osc5.setTable(SIN512_DATA);
      osc6.setTable(SIN512_DATA);
      osc7.setTable(SIN512_DATA);
      osc8.setTable(SIN512_DATA);
      break;
    case 6:
      osc1.setTable(PHASOR256_DATA);
      osc2.setTable(PHASOR256_DATA);
      osc3.setTable(PHASOR256_DATA);
      osc4.setTable(PHASOR256_DATA);
      osc5.setTable(PHASOR256_DATA);
      osc6.setTable(PHASOR256_DATA);
      osc7.setTable(PHASOR256_DATA);
      osc8.setTable(PHASOR256_DATA);
      break;
    case 7:
      osc1.setTable(COSPHASE256_DATA);
      osc2.setTable(COSPHASE256_DATA);
      osc3.setTable(COSPHASE256_DATA);
      osc4.setTable(COSPHASE256_DATA);
      osc5.setTable(COSPHASE256_DATA);
      osc6.setTable(COSPHASE256_DATA);
      osc7.setTable(COSPHASE256_DATA);
      osc8.setTable(COSPHASE256_DATA);
      break;
  }
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int updateAudio() {
  int asig1 = (osc1.next() * v1 + osc2.next() * v2) >> 1;
  int asig2 = (osc3.next() * v3 + osc4.next() * v4) >> 1;
  int asig3 = (osc5.next() * v5 + osc6.next() * v6) >> 1;
  int asig4 = (osc7.next() * v7 + osc8.next() * v8) >> 1;
  int asig5 = (asig1 + asig2) >> 1;
  int asig6 = (asig3 + asig4) >> 1;
  int asigDRN = (asig5 + asig6) >> 7;
  return (asigDRN);
}

void loop() {
  audioHook();
}
