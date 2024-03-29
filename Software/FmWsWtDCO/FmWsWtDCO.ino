#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>

#include <tables/sin512_int8.h>             // table for Oscils to play
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

#include <tables/waveshape_tanh_int8.h>               //wt
#include <tables/waveshape_sigmoid_int8.h>            //wt
#include <tables/waveshape1_softclip_int8.h>          //wt
#include <tables/waveshape2_softerclip_int8.h>        //wt
#include <tables/waveshape_chebyshev_3rd_256_int8.h>  //wt
#include <tables/waveshape_chebyshev_4th_256_int8.h>  //wt
#include <tables/waveshape_chebyshev_5th_256_int8.h>  //wt
#include <tables/waveshape_chebyshev_6th_256_int8.h>  //wt

#include <tables/waveshape_compress_512_to_488_int16.h>  //wt compress

//lecheng的控制/显示模块封装
#include "Module_LEDDisplay.h"
#include "Module_Ctrl.h"
#include "Module_Const.h"

#define CONTROL_RATE 256           //Hz, powers of 2 are most reliable
#define FUNC_LENGTH 8              //功能列表长度
#define OSC_BASE1_FREQ 2143658     //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_BASE2_FREQ 4287316     //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_BASE3_FREQ 8574632     //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_VOCT_COEFFICIENT 5200  //振荡器可调范围 5200-两个八度
#define LFO_FREQENCY 2143          //振荡器基础频率 0.1hz-4hz
#define LFO_CV_COEFFICIENT 1000    //振荡器可调范围
/*   引脚定义   */
#define KONB_PIN 4   //
#define VOCT_PIN 0   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define GATE_PIN 11  //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> aSin1(SIN512_DATA);
Oscil<PHASOR256_NUM_CELLS, AUDIO_RATE> aPha(PHASOR256_DATA);
Oscil<TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> aTriHermes(TRIANGLE_HERMES_2048_DATA);
Oscil<TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aTriValve(TRIANGLE_VALVE_2_2048_DATA);
Oscil<TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> aTriDsitCub(TRIANGLE_DIST_CUBED_2048_DATA);
Oscil<TRIANGLE_DIST_SQUARED_2048_NUM_CELLS, AUDIO_RATE> aTriDsitSqu(TRIANGLE_DIST_SQUARED_2048_DATA);
Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSquAnalogue(SQUARE_ANALOGUE512_DATA);
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> aSqu4(SQUARE_NO_ALIAS512_DATA);  //SQUARE_NO_ALIAS512_NUM_CELLS

Oscil<SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aSawAnalogue(SAW_ANALOGUE512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw(SAW256_DATA);
Oscil<HALFSINWINDOW512_NUM_CELLS, AUDIO_RATE> aHSinWin(HALFSINWINDOW512_DATA);
Oscil<WAVESHAPE_TANH_NUM_CELLS, AUDIO_RATE> aTanh(WAVESHAPE_TANH_DATA);
Oscil<WAVESHAPE_SIGMOID_NUM_CELLS, AUDIO_RATE> aSigmoid(WAVESHAPE_SIGMOID_DATA);
Oscil<CHEBYSHEV_5TH_256_NUM_CELLS, AUDIO_RATE> aCheb5(CHEBYSHEV_5TH_256_DATA);
Oscil<CHEBYSHEV_6TH_256_NUM_CELLS, AUDIO_RATE> aCheb6(CHEBYSHEV_6TH_256_DATA);
Oscil<WAVESHAPE1_SOFTCLIP_NUM_CELLS, AUDIO_RATE> aSoftClip(WAVESHAPE1_SOFTCLIP_DATA);

WaveShaper<char> wsTanh(WAVESHAPE_TANH_DATA);                // WaveShaper
WaveShaper<char> wsSigmod(WAVESHAPE_SIGMOID_DATA);           // WaveShaper
WaveShaper<char> wsSoftclip1(WAVESHAPE1_SOFTCLIP_DATA);      // WaveShaper
WaveShaper<char> wsSoftclip2(WAVESHAPE2_SOFTERCLIP_DATA);    // WaveShaper
WaveShaper<char> wsCH3rd(CHEBYSHEV_3RD_256_DATA);            // WaveShaper
WaveShaper<char> wsCH4th(CHEBYSHEV_4TH_256_DATA);            // WaveShaper
WaveShaper<char> wsCH5th(CHEBYSHEV_5TH_256_DATA);            // WaveShaper
WaveShaper<char> wsCH6th(CHEBYSHEV_6TH_256_DATA);            // WaveShaper
WaveShaper<int> wsComp(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals

Oscil<256, AUDIO_RATE> aModulator(SIN512_DATA);
Oscil<256, CONTROL_RATE> kModIndex(SIN512_DATA);

Q16n16 POSITION = 0;
String function[FUNC_LENGTH] = { "Pitch", "Range", "ShapeG", "WaveS", "OPFreq", "OPAmt", "WaveT", "WaveC" };
int param[FUNC_LENGTH] = { 0, 360, 0, 0, 0, 0, 0, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_P, Led_R, Led_G, Led_S, Led_F, Led_A, Led_T, Led_C };

Q16n16 RangeType = 1;       //C0
Q16n16 BaseFreq = 2143658;  //C0
Q16n16 FreqRange = 5200;    //2OCT
Q16n16 ShapeSelf = 0;       //当前波形手动选择标志
Q16n16 ShapeGradient = 0;   //波形渐变算法
Q16n16 ShapeMod = 0;        //波形渐变量调制量
Q16n16 OP1Freq, OP2Freq, OP2Amt, Pitch;
Q16n16 WaveTrig = 0;
Q16n16 WavePosition = 0;
Q16n16 WaveType = 0;
Q16n16 WaveChange = 0;

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化Led引脚

  startMozzi(CONTROL_RATE);
}

//三个旋钮 Carrier A0  ModFreq A1  ModLV A3    C
void updateControl() {

  POSITION = getPostition(POSITION, FUNC_LENGTH);                   //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);                      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                                   //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);                   //如果处在非编辑状态 led将半灭显示
  Serial.println(POSITION + function[POSITION] + param[POSITION]);  //func param Log

  Pitch = param[0];           //音高旋钮参数
  RangeType = param[1] >> 8;  //震荡范围
  switch (RangeType) {
    case 0:
      BaseFreq = LFO_FREQENCY;
      FreqRange = LFO_CV_COEFFICIENT;
      break;
    case 3:
      BaseFreq = OSC_BASE3_FREQ;
      FreqRange = OSC_VOCT_COEFFICIENT;
      break;
    case 2:
      BaseFreq = OSC_BASE2_FREQ;
      FreqRange = OSC_VOCT_COEFFICIENT;
      break;
    default:
      BaseFreq = OSC_BASE1_FREQ;
      FreqRange = OSC_VOCT_COEFFICIENT;
      break;
  }
  ShapeGradient = param[2] >> 7;
  ShapeSelf = param[3] >> 3;                                                                                   //0-128
  ShapeMod = mozziAnalogRead(CV1_PIN) >> 3;                                                                    //波形渐变CV值获取A1
  OP1Freq = (BaseFreq + Pitch * FreqRange) * pow(2, (pgm_read_float(&(voctpow[mozziAnalogRead(VOCT_PIN)]))));  // V/oct 由于cltest0.6的voct接口阻抗问题 这里可能需要乘以一个系数 调谐才比较准确
  OP2Freq = ((OP1Freq >> 8) * (param[4] / 2 + mozziAnalogRead(CV2_PIN) / 2));                                  // mozziAnalogRead(1)
  OP2Amt = ((OP2Freq >> 16) * (1 + param[5] + mozziAnalogRead(CV3_PIN)));
  Q16n16 WaveSelect = param[6] >> 6;  //波表  将1023分成16个波表类型
  Q16n16 WaveChange = param[7] >> 6;  //偏移量为16
  // Q16n16 WaveChangeRandom = (param[8] >> 6);

  //波形切换触发器
  if (digitalRead(GATE_PIN) != WaveTrig && WaveTrig == 0) {  //d13按钮可以用来测试
    
    // if (RangeType == 0) {
    // }
    // if (RangeType != 0) {
    // }
    WaveTrig = 1;
    // WavePosition += WaveChange /** (rand() % WaveChangeRandom + 1)*/;
    // if (WavePosition > 1073741823) WavePosition = 0;
  }
  if (digitalRead(GATE_PIN) != WaveTrig && WaveTrig == 1) {
    // WaveTrig = 0;
  }
  WaveType = (WaveSelect + WavePosition) % 16;

  aSin1.setFreq_Q16n16(OP1Freq);         //给主波形设置频率、音高
  aPha.setFreq_Q16n16(OP1Freq);          //给主波形设置频率、音高
  aTriHermes.setFreq_Q16n16(OP1Freq);    //给主波形设置频率、音高
  aTriValve.setFreq_Q16n16(OP1Freq);     //给主波形设置频率、音高
  aTriDsitCub.setFreq_Q16n16(OP1Freq);   //给主波形设置频率、音高
  aTriDsitSqu.setFreq_Q16n16(OP1Freq);   //给主波形设置频率、音高
  aSquAnalogue.setFreq_Q16n16(OP1Freq);  //给主波形设置频率、音高
  aSqu4.setFreq_Q16n16(OP1Freq);         //给主波形设置频率、音高
  aSawAnalogue.setFreq_Q16n16(OP1Freq);  //给主波形设置频率、音高
  aSaw.setFreq_Q16n16(OP1Freq);          //给主波形设置频率、音高
  aHSinWin.setFreq_Q16n16(OP1Freq);      //给主波形设置频率、音高
  aSigmoid.setFreq_Q16n16(OP1Freq);      //给主波形设置频率、音高
  aTanh.setFreq_Q16n16(OP1Freq);         //给主波形设置频率、音高
  aCheb6.setFreq_Q16n16(OP1Freq);        //给主波形设置频率、音高
  aCheb5.setFreq_Q16n16(OP1Freq);        //给主波形设置频率、音高
  aSoftClip.setFreq_Q16n16(OP1Freq);     //给主波形设置频率、音高

  aModulator.setFreq_Q16n16(OP2Freq);

  /*TEST LOG*/
  // Serial.print(" a0= ");
  // Serial.print(mozziAnalogRead(0));
  // Serial.print(" a1= ");
  // Serial.print(mozziAnalogRead(1));
  // Serial.print(" a2= ");
  // Serial.print(mozziAnalogRead(2));
  // Serial.print(" a3= ");
  // Serial.print(mozziAnalogRead(3));
  // Serial.print(" a4= ");
  // Serial.println(mozziAnalogRead(4));
  // Serial.print(" d12= ");
  // Serial.println(digitalRead(12));
  // Serial.print(" d13= ");
  // Serial.println(digitalRead(13));
  // Serial.print("-OP1Freq-");
  // Serial.print(OP1Freq);
  // Serial.print("-OP2Freq-");
  // Serial.print(OP2Freq);
  // Serial.print("-OP2Amt-");
  // Serial.println(OP2Amt);
  // Serial.print(" d11= ");
  // Serial.print(digitalRead(11));
  // Serial.print("-WaveTrig= ");
  // Serial.print(WaveTrig);
  // Serial.print(" WaveType= ");
  // Serial.print(WaveType);
  // Serial.print("-WaveChange= ");
  // Serial.println(WaveChange);
  // Serial.print("-ShapeGradient-");
  // Serial.println(ShapeGradient);
}

AudioOutput_t updateAudio() {
  Q15n16 modulation = OP2Amt * aModulator.next() >> 8;
  char asig = 0;
  switch (WaveType) {
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
  byte asigShape = (byte)128 + ((asig * ((byte)128 + ShapeSelf + ShapeMod)) >> 8);
  char wtasig = 0;
  switch (ShapeGradient) {
    default:
      wtasig = wsTanh.next(asigShape);
      break;
    case 1:
      wtasig = wsSigmod.next(asigShape);
      break;
    case 2:
      wtasig = wsSoftclip1.next(asigShape);
      break;
    case 3:
      wtasig = wsSoftclip2.next(asigShape);
      break;
    case 4:
      wtasig = wsCH3rd.next(asigShape);
      break;
    case 5:
      wtasig = wsCH4th.next(asigShape);
      break;
    case 6:
      wtasig = wsCH5th.next(asigShape);
      break;
    case 7:
      // wtasig = wsCH5th.next(asig) * (Shape*8 + ShapeMod*8) >> 10 + asig;//旧的叠加算法
      wtasig = wsCH6th.next(asigShape);
      break;
  }
  //压缩波形变化后的幅值
  asig = wsComp.next(256u + wtasig);  //+ awaveshaped2
  //最终信号输出
  return asig;
}

void loop() {
  audioHook();
}