#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>

#include <tables/sin512_int8.h>             // table for Oscils to play
#include <tables/halfsinwindow512_uint8.h>  // sine table for oscillator
// #include <tables/triangle_dist_squared_2048_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_chebyshev_5th_256_int8.h>

#include <tables/phasor256_int8.h>
#include <tables/uphasor256_uint8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/cos256_int8.h>
#include <tables/cosphase256_int8.h>
// #include <tables/triangle_dist_cubed_2048_int8.h>
// #include <tables/triangle_hermes_2048_int8.h>
// #include <tables/triangle_valve_2_2048_int8.h>
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

/*lecheng的控制/显示模块封装*/
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

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> osc1(SIN512_DATA);
Oscil<256, AUDIO_RATE> osc2(SIN512_DATA);

WaveShaper<char> wsTanh(WAVESHAPE_TANH_DATA);                // WaveShaper
WaveShaper<char> wsSigmod(WAVESHAPE_SIGMOID_DATA);           // WaveShaper
WaveShaper<char> wsSoftclip1(WAVESHAPE1_SOFTCLIP_DATA);      // WaveShaper
WaveShaper<char> wsSoftclip2(WAVESHAPE2_SOFTERCLIP_DATA);    // WaveShaper
WaveShaper<char> wsCH3rd(CHEBYSHEV_3RD_256_DATA);            // WaveShaper
WaveShaper<char> wsCH4th(CHEBYSHEV_4TH_256_DATA);            // WaveShaper
WaveShaper<char> wsCH5th(CHEBYSHEV_5TH_256_DATA);            // WaveShaper
WaveShaper<char> wsCH6th(CHEBYSHEV_6TH_256_DATA);            // WaveShaper
WaveShaper<int> wsComp(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals

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

void updateControl() {
  /*控制参数获取与显示逻辑*/
  POSITION = getPostition(POSITION, FUNC_LENGTH);                   //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);                      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                                   //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);                   //如果处在非编辑状态 led将半灭显示
  Serial.println(POSITION + function[POSITION] + param[POSITION]);  //func param Log

  /*震荡与音高范围计算*/
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

  /*波形渐变量*/
  ShapeGradient = param[2] >> 7;
  ShapeSelf = param[3] >> 3;                 //0-128
  ShapeMod = mozziAnalogRead(CV1_PIN) >> 3;  //波形渐变CV值获取A1

  /*FM量*/
  OP1Freq = (BaseFreq + Pitch * FreqRange) * pow(2, (pgm_read_float(&(voctpow[mozziAnalogRead(VOCT_PIN)]))));  // V/oct 由于cltest0.6的voct接口阻抗问题 这里可能需要乘以一个系数 调谐才比较准确
  OP2Freq = ((OP1Freq >> 8) * (param[4] / 2 + mozziAnalogRead(CV2_PIN) / 2));                                  // mozziAnalogRead(1)
  OP2Amt = ((OP2Freq >> 16) * (1 + param[5] + mozziAnalogRead(CV3_PIN)));
  osc1.setFreq_Q16n16(OP1Freq);  //给主波形设置频率
  osc2.setFreq_Q16n16(OP2Freq);  //给算子设置频率

  Q16n16 WaveSelect = param[6] >> 6;  //波表  将1023分成16个波表类型
  Q16n16 WaveChange = param[7] >> 6;  //偏移量为16
  //波形切换触发器
  if (digitalRead(GATE_PIN) != WaveTrig && WaveTrig == 0) {  //d13按钮可以用来测试
    WaveTrig = 1;

    if (RangeType == 0) {  //lfo模式 用于rst
    }
    if (RangeType != 0) {  //vco模式 用于切换波形
      WavePosition += WaveChange /** (rand() % WaveChangeRandom + 1)*/;
      if (WavePosition > 1073741823) WavePosition = 0;
    }
  }
  if (digitalRead(GATE_PIN) != WaveTrig && WaveTrig == 1) {
    WaveTrig = 0;
  }
  WaveType = (WaveSelect + WavePosition) % 16;

  //设置波形
  switch (WaveType) {
    default:
      osc1.setTable(SIN512_DATA);
      break;
    case 1:
      osc1.setTable(WAVESHAPE_TANH_DATA);
      break;
    case 2:  //2345
      osc1.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 3:
      osc1.setTable(WAVESHAPE1_SOFTCLIP_DATA);
      break;
    case 4:
      osc1.setTable(WAVESHAPE_SIGMOID_DATA);
      break;
    case 5:
      osc1.setTable(SQUARE_NO_ALIAS512_DATA);
      break;
    case 6:
      osc1.setTable(SAW256_DATA);
      break;
    case 7:
      osc1.setTable(SAW_ANALOGUE512_DATA);
      break;
    case 8:
      osc1.setTable(HALFSINWINDOW512_DATA);
      break;
    case 9:
      osc1.setTable(CHEBYSHEV_5TH_256_DATA);
      break;
    case 10:
      osc1.setTable(PHASOR256_DATA);
      break;
    case 11:
      osc1.setTable(UPHASOR256_DATA);
      break;
    case 12:
      osc1.setTable(HALFSIN256_DATA);
      break;
    case 13:
      osc1.setTable(COSPHASE256_DATA);
      break;
    case 14:
      osc1.setTable(COS256_DATA);
      break;
    case 15:
      osc1.setTable(CHEBYSHEV_6TH_256_DATA);
      break;
  }

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
  // Serial.print(WaveType);
  // Serial.print(" WaveType= ");
  // Serial.print("-WaveChange= ");
  // Serial.println(WaveChange);
  // Serial.print("-ShapeGradient-");
  // Serial.println(ShapeGradient);
}

AudioOutput_t updateAudio() {
  Q15n16 modulation = OP2Amt * osc2.next() >> 8;
  char asig = MonoOutput::from8Bit(osc1.phMod(modulation));  // Internally still only 8 bits, will be shifted up to 14 bits in HIFI mode

  // if (RangeType == 0&&WaveTrig == 1) asig = MonoOutput::from8Bit(osc1.phMod(0));

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