#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>

#include <tables/sin256_int8.h>             // table for Oscils to play
#include <tables/halfsinwindow512_uint8.h>  // sine table for oscillator
#include <tables/triangle_dist_squared_2048_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_chebyshev_5th_256_int8.h>

#include <tables/phasor256_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/triangle_valve_2_2048_int8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/waveshape1_softclip_int8.h>
#include <tables/waveshape_sigmoid_int8.h>
#include <tables/saw_analogue512_int8.h>

#include <tables/waveshape_tanh_int8.h>  //wt
#include <tables/waveshape_compress_512_to_488_int16.h>

#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

#define CONTROL_RATE 256  // Hz, powers of 2 are most reliable
#define FUNC_LENGTH 5     // menu length

Oscil<256, AUDIO_RATE> aSin1(SIN256_DATA);
Oscil<PHASOR256_NUM_CELLS, AUDIO_RATE> aPha(PHASOR256_DATA);
Oscil<TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aTriHermes(TRIANGLE_HERMES_2048_DATA);
Oscil<TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aTriValve(TRIANGLE_VALVE_2_2048_DATA);
Oscil<TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> aTriDsitCub(TRIANGLE_DIST_CUBED_2048_DATA);
Oscil<TRIANGLE_DIST_SQUARED_2048_NUM_CELLS, AUDIO_RATE> aTriDsitSqu(TRIANGLE_DIST_SQUARED_2048_DATA);
Oscil<SQUARE_ANALOGUE512_SAMPLERATE, AUDIO_RATE> aSquAnalogue(SQUARE_ANALOGUE512_DATA);
Oscil<512, AUDIO_RATE> aSqu4(SQUARE_NO_ALIAS512_DATA);  //SQUARE_NO_ALIAS512_NUM_CELLS

Oscil<SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSawAnalogue(SAW_ANALOGUE512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw(SAW256_DATA);
Oscil<256, AUDIO_RATE> aHSinWin(HALFSINWINDOW512_DATA);
Oscil<WAVESHAPE_TANH_NUM_CELLS, AUDIO_RATE> aTanh(WAVESHAPE_TANH_DATA);
Oscil<WAVESHAPE_SIGMOID_NUM_CELLS, AUDIO_RATE> aSigmoid(WAVESHAPE_SIGMOID_DATA);
Oscil<256, AUDIO_RATE> aCheb5(CHEBYSHEV_5TH_256_DATA);
Oscil<CHEBYSHEV_6TH_256_NUM_CELLS, AUDIO_RATE> aCheb6(CHEBYSHEV_6TH_256_DATA);
Oscil<WAVESHAPE1_SOFTCLIP_NUM_CELLS, AUDIO_RATE> aSoftClip(WAVESHAPE1_SOFTCLIP_DATA);

WaveShaper<char> wsTanh(WAVESHAPE_TANH_DATA);                // 8th harmonic
WaveShaper<int> wsComp(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals

Oscil<256, AUDIO_RATE> aModulator(SIN256_DATA);
Oscil<256, CONTROL_RATE> kModIndex(SIN256_DATA);

int voct = 500;
Q16n16 POSITION = 0;
String function[FUNC_LENGTH] = { "PTC", "WS", "ModFq", "ModLV", "WT" };
int param[FUNC_LENGTH] = { 1, 2, 3, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_P, Led_S, Led_F, Led_A, Led_T };

Q16n16 FMA;
int Wave;
int WaveMod = 0;
Q16n16 WaveTrigger = 0;
Q16n16 toneFreq, FmFreq, pitch;
Q16n16 Shape = 0;
Q16n16 ShapeMod = 0;

void setup() {
  Serial.begin(115200);           //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
  startMozzi(CONTROL_RATE);
}

//三个旋钮 Carrier A0  ModFreq A1  ModLV A3    C
void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  // displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  for (int i = 2; i < 9; i++)  //display  //简易参数展示
    digitalWrite(i, HIGH);
  digitalWrite(POSITION + 2, LOW);

  Serial.print(POSITION);           //func param
  Serial.println("func");           //func param
  Serial.println(param[POSITION]);  //func param
  Serial.println(" ");
  // Serial.print("d11 = ");
  // Serial.println(digitalRead(11));

  //VOCT A7  CV-Freq A4  CV-LV A5
  voct = mozziAnalogRead(0);                                                         //由于cltest的voct接口阻抗问题 这里需要乘以一个系数 调谐才比较准确
  pitch = param[0];                                                                  //{ "PTC", "WS", "ModFq", "ModLV", "WT" };
  Shape = param[1];                                                                  //波形渐变
  toneFreq = (2270658 + pitch * 5000) * pow(2, (pgm_read_float(&(voctpow[voct]))));  // V/oct apply
  FmFreq = ((toneFreq >> 8) * (param[2] / 2 + mozziAnalogRead(1) / 2));              // mozziAnalogRead(1)
  FMA = ((FmFreq >> 16) * (1 + param[3] + mozziAnalogRead(2)));
  Wave = param[4] >> 6;  //波表  将1023分成16个波表类型

  //波形切换触发器
  if (digitalRead(11) != WaveTrigger && WaveTrigger == 0) {  //d13按钮可以用来测试
    WaveTrigger = 1;
    WaveMod++;
    if (WaveMod > 15) WaveMod = 0;
  }
  if (digitalRead(11) != WaveTrigger && WaveTrigger == 1) {
    WaveTrigger = 0;
  }
  if (Wave < 16) {
    if (Wave + WaveMod > 6) {  //使波表的选择循环起来
      Wave = Wave + WaveMod - 6;
    } else {
      Wave = WaveMod + Wave;
    }
    if (Wave < 0) Wave = 0;
  }

  Serial.print("waveMod= ");
  Serial.print(WaveMod);
  Serial.print(" wave= ");
  Serial.println(Wave);

  aSin1.setFreq_Q16n16(toneFreq);         //给主波形设置频率、音高
  aPha.setFreq_Q16n16(toneFreq);          //给主波形设置频率、音高
  aTriHermes.setFreq_Q16n16(toneFreq);    //给主波形设置频率、音高
  aTriValve.setFreq_Q16n16(toneFreq);     //给主波形设置频率、音高
  aTriDsitCub.setFreq_Q16n16(toneFreq);   //给主波形设置频率、音高
  aTriDsitSqu.setFreq_Q16n16(toneFreq);   //给主波形设置频率、音高
  aSquAnalogue.setFreq_Q16n16(toneFreq);  //给主波形设置频率、音高
  aSqu4.setFreq_Q16n16(toneFreq);         //给主波形设置频率、音高

  aSawAnalogue.setFreq_Q16n16(toneFreq);  //给主波形设置频率、音高
  aSaw.setFreq_Q16n16(toneFreq);          //给主波形设置频率、音高
  aHSinWin.setFreq_Q16n16(toneFreq);      //给主波形设置频率、音高
  aSigmoid.setFreq_Q16n16(toneFreq);      //给主波形设置频率、音高
  aTanh.setFreq_Q16n16(toneFreq);         //给主波形设置频率、音高
  aCheb6.setFreq_Q16n16(toneFreq);        //给主波形设置频率、音高
  aCheb5.setFreq_Q16n16(toneFreq);        //给主波形设置频率、音高
  aSoftClip.setFreq_Q16n16(toneFreq);     //给主波形设置频率、音高

  aModulator.setFreq_Q16n16(FmFreq);

  // Serial.print(toneFreq);
  // Serial.print("--");
  // Serial.print(FmFreq);
  // Serial.print("--");
  // Serial.println(FMA);
  if (Shape > 30) ShapeMod = mozziAnalogRead(3);
  else ShapeMod = Shape;
}

AudioOutput_t updateAudio() {
  Q15n16 modulation = FMA * aModulator.next() >> 8;
  char asig = 0;

  switch (Wave) {
    default:
      asig = MonoOutput::from8Bit(aSin1.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 1:
      asig = MonoOutput::from8Bit(aPha.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 2:
      asig = MonoOutput::from8Bit(aTriHermes.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 3:
      asig = MonoOutput::from8Bit(aTriValve.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 4:
      asig = MonoOutput::from8Bit(aTriDsitCub.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 5:
      asig = MonoOutput::from8Bit(aSquAnalogue.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 6:
      asig = MonoOutput::from8Bit(aSqu4.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 7:
      asig = MonoOutput::from8Bit(aSawAnalogue.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 8:
      asig = MonoOutput::from8Bit(aSaw.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 9:
      asig = MonoOutput::from8Bit(aHSinWin.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 10:
      asig = MonoOutput::from8Bit(aTanh.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 11:
      asig = MonoOutput::from8Bit(aSigmoid.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 12:
      asig = MonoOutput::from8Bit(aCheb5.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 13:
      asig = MonoOutput::from8Bit(aCheb6.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 14:
      asig = MonoOutput::from8Bit(aSoftClip.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 15:
      asig = MonoOutput::from8Bit(aSawAnalogue.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
  }
  //波形渐变算法
  int wtasig = 0;
  wtasig = wsTanh.next(asig) * (Shape >> 1 + ShapeMod >> 1) >> 10;
  asig = wsComp.next(256u + asig + wtasig);
  //最终信号输出
  return asig;
}

void loop() {
  audioHook();
}