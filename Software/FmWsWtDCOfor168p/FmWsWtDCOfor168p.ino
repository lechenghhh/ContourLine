#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>

#include <tables/sin256_int8.h>             // table for Oscils to play
#include <tables/halfsinwindow512_uint8.h>  // sine table for oscillator
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/waveshape_tanh_int8.h>  //wt
#include <tables/waveshape_compress_512_to_488_int16.h>

#include "Module_Ctrl.h"
// #include "Module_LEDDisplay.h"
#include "Module_Const.h"

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable
#define FUNC_LENGTH 6     // menu length

Oscil<256, AUDIO_RATE> aTanh(WAVESHAPE_TANH_DATA);
Oscil<256, AUDIO_RATE> aSin1(SIN256_DATA);
Oscil<169, AUDIO_RATE> aLofi2(SAW256_DATA);
Oscil<212, AUDIO_RATE> aPha(PHASOR256_DATA);
Oscil<197, AUDIO_RATE> aLofi3(SIN256_DATA);
Oscil<256, AUDIO_RATE> aHSinWin(HALFSINWINDOW512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw(SAW256_DATA);
Oscil<128, AUDIO_RATE> aLofi1(SIN256_DATA);  //SQUARE_NO_ALIAS512_NUM_CELLS

WaveShaper<char> wsTanh(WAVESHAPE_TANH_DATA);                // 8th harmonic
WaveShaper<int> wsComp(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals

Oscil<256, AUDIO_RATE> aModulator(SIN256_DATA);
Oscil<256, CONTROL_RATE> kModIndex(SIN256_DATA);

int voct = 500;
byte POSITION = 0;
char function[FUNC_LENGTH][3] = { "PTC", "rg", "WS", "FMF", "FMA", "WT" };
short param[FUNC_LENGTH] = { 1, 400, 2, 3, 0 };

Q16n16 FMA;
byte Wave;
byte WaveMod = 0;
byte WaveTrigger = 0;
byte WaveType = 0;
Q16n16 toneFreq, FmFreq, pitch;
byte Shape = 0;
byte ShapeMod = 0;
Q16n16 BaseFreq = 2143658;  //C0
Q16n16 FreqRange = 5200;    //2OCT

void setup() {
  Serial.begin(115200);           //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(4, 32, 13, 12, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
  startMozzi(CONTROL_RATE);
}

//三个旋钮 Carrier A0  ModFreq A1  ModLV A3    C
void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值

  for (byte i = 2; i < 9; i++)  //display  //简易参数展示
    digitalWrite(i, HIGH);
  digitalWrite(POSITION + 2, LOW);

  Serial.print(POSITION);           //func param
  Serial.println("func");           //func param
  Serial.println(param[POSITION]);  //func param
  Serial.println(" ");

  //VOCT A7  CV-Freq A4  CV-LV A5
  voct = mozziAnalogRead(0);  //由于cltest的voct接口阻抗问题 这里需要乘以一个系数 调谐才比较准确
  pitch = param[0];           //{ "PTC", "range", "WS", "ModFq", "ModLV", "WT" };

  switch (param[1] >> 8) {
    case 0:
      BaseFreq = 2143;
      FreqRange = 1000;
      break;
    case 3:
      BaseFreq = 8574632;
      FreqRange = 5200;
      break;
    case 2:
      BaseFreq = 4287316;
      FreqRange = 5200;
      break;
    default:
      BaseFreq = 2143658;
      FreqRange = 5200;
      break;
  }
  Shape = param[2];                                                                   //波形渐变
  toneFreq = (BaseFreq + pitch * 5200) * pow(2, (pgm_read_float(&(voctpow[voct]))));  // V/oct apply
  FmFreq = ((toneFreq >> 8) * (param[3] / 2 + mozziAnalogRead(2) / 2));               // mozziAnalogRead(1)
  FMA = ((FmFreq >> 16) * (1 + param[4] + mozziAnalogRead(3)));
  Wave = param[5] >> 7;  //波表  将1023分成8个波表类型

  //波形切换触发器
  if (digitalRead(11) != WaveTrigger && WaveTrigger == 0) {  //d13按钮可以用来测试
    WaveTrigger = 1;
    WaveMod++;
    if (WaveMod > 7) WaveMod = 0;
  }
  if (digitalRead(11) != WaveTrigger && WaveTrigger == 1) {
    WaveTrigger = 0;
  }
  WaveType = (Wave + WaveMod) % 8;

  // Serial.print("waveMod= ");
  // Serial.print(WaveMod);
  // Serial.print(" wave= ");
  // Serial.println(Wave);

  aLofi1.setFreq_Q16n16(toneFreq);    //给主波形设置频率、音高
  aSin1.setFreq_Q16n16(toneFreq);     //给主波形设置频率、音高
  aLofi2.setFreq_Q16n16(toneFreq);    //给主波形设置频率、音高
  aLofi3.setFreq_Q16n16(toneFreq);    //给主波形设置频率、音高
  aPha.setFreq_Q16n16(toneFreq);      //给主波形设置频率、音高
  aTanh.setFreq_Q16n16(toneFreq);     //给主波形设置频率、音高
  aSaw.setFreq_Q16n16(toneFreq);      //给主波形设置频率、音高
  aHSinWin.setFreq_Q16n16(toneFreq);  //给主波形设置频率、音高

  aModulator.setFreq_Q16n16(FmFreq);

  // Serial.print(toneFreq);
  // Serial.print("--");
  // Serial.print(FmFreq);
  // Serial.print("--");
  // Serial.println(FMA);
  if (Shape > 30) ShapeMod = mozziAnalogRead(1);
  else ShapeMod = Shape;

  if (getKnobEnable() == 0) digitalWrite(POSITION + 2, HIGH);  //如果处在非编辑状态 led将半灭显示
}

AudioOutput_t updateAudio() {
  Q15n16 modulation = FMA * aModulator.next() >> 8;
  char asig = 0;

  switch (WaveType) {
    default:
      asig = MonoOutput::from8Bit(aTanh.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 1:
      asig = MonoOutput::from8Bit(aSin1.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 2:
      asig = MonoOutput::from8Bit(aLofi2.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 3:
      asig = MonoOutput::from8Bit(aPha.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 4:
      asig = MonoOutput::from8Bit(aLofi3.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 5:
      asig = MonoOutput::from8Bit(aHSinWin.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 6:
      asig = MonoOutput::from8Bit(aSaw.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
    case 7:
      asig = MonoOutput::from8Bit(aLofi1.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode
      break;
  }
  //波形渐变算法
  int wtasig = 0;
  wtasig = wsTanh.next(asig) * (Shape + ShapeMod) >> 10;
  asig = wsComp.next(256u + asig + wtasig);
  //最终信号输出
  return asig;
}

void loop() {
  audioHook();
}