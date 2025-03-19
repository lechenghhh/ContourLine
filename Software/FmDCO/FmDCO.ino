#include <Mozzi.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <FixMath.h>
#include <ADSR.h>  // table for Oscils to play

#include <tables/sin1024_int8.h>            // table for Oscils to play
#include <tables/halfsinwindow512_uint8.h>  // sine table for oscillator
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_chebyshev_5th_256_int8.h>

#include <tables/phasor256_int8.h>
#include <tables/uphasor256_uint8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/cos256_int8.h>
#include <tables/cosphase256_int8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/waveshape1_softclip_int8.h>
#include <tables/waveshape_sigmoid_int8.h>
#include <tables/saw_analogue512_int8.h>
#include <tables/whitenoise8192_int8.h>

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
// #define MOZZI_AUDIO_MODE MOZZI_OUTPUT_PWM  //输出模式
#define MOZZI_AUDIO_MODE MOZZI_OUTPUT_2PIN_PWM  //输出模式
#define MOZZI_AUDIO_PIN_1 9                     // GPIO pin number, can be any pin
#define MOZZI_AUDIO_PIN_2 10                    // GPIO pin number, can be any pin
#define MOZZI_AUDIO_RATE 32768                  // 音频采样率
#define MOZZI_CONTROL_RATE 256                  // Hz, powers of 2 are most reliable
#define MOZZI_AUDIO_BITS 12                     // 输出位数

#define PARAM_LENGTH 9                          //功能列表长度
#define OSC_BASE1_FREQ 2143658                  //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_BASE2_FREQ 4287316                  //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_BASE3_FREQ 8574632                  //振荡器基础频率 约32.7hz  org:apply 2270658 1=c#
#define OSC_VOCT_COEFFICIENT 5200               //振荡器可调范围 5200-两个八度
#define LFO_FREQENCY 2143                       //振荡器基础频率 0.1hz-4hz
#define LFO_CV_COEFFICIENT 1000                 //振荡器可调范围
/*   引脚定义   */
#define KONB_PIN 4   //
#define VOCT_PIN 0   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define GATE_PIN 11  //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> osc1(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> osc2(SIN1024_DATA);
ADSR<MOZZI_AUDIO_RATE, MOZZI_AUDIO_RATE> env1;

Q16n16 POSITION = 0;
String param_name[PARAM_LENGTH] = { "Pitch", "Octave", "OPFreq", "OPLV", "Attk", "Decay", "Sustain", "Release", "WaveType" };
int param[PARAM_LENGTH] = { 0, 360, 64, 0, 0, 64, 128, 32 };
bool* ledGroup[PARAM_LENGTH] = { Led_P, Led_O, Led_F, Led_L, Led_A, Led_D, Led_S, Led_R, Led_W };

int RangeType = 1;          //C0
Q16n16 BaseFreq = 2143658;  //C0
Q16n16 FreqRange = 5200;    //2OCT
Q16n16 OP1Freq, OP2Freq, OP2Amt, Pitch;
int envgain = 0;
int trig3 = 0;

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化Led引脚

  startMozzi();
}

void updateControl() {
  /*控制参数获取与显示逻辑*/
  POSITION = getPostition(POSITION, PARAM_LENGTH);                    //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);                        //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值 // if arduino nano
  displayLED(ledGroup[POSITION]);                                     //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);                     //如果处在非编辑状态 led将半灭显示
  Serial.println(POSITION + param_name[POSITION] + param[POSITION]);  //func param Log

  //四段包络
  env1.setLevels(255, 64, 32, 0);
  int attk = param[4];
  int decay = param[5] + myread(1);
  int sustain = param[6] + myread(1);
  int release = param[7] + myread(1);
  env1.setTimes(attk, decay, sustain, release);

  if (digitalRead(11) == 1 && trig3 == 0) {
    trig3 = 1;
    env1.noteOn();
  }
  if (digitalRead(11) == 0 && trig3 == 1) {
    trig3 = 0;
    env1.noteOff();
  }


  /*音高 伏每八度*/
  Pitch = param[0];
  RangeType = (param[1] >> 7);  //震荡范围

  if (RangeType < 0) {
    RangeType = 0;
    BaseFreq = LFO_FREQENCY;
    FreqRange = LFO_CV_COEFFICIENT;
  } else {
    BaseFreq = OSC_BASE1_FREQ;
    FreqRange = OSC_VOCT_COEFFICIENT;
  }
  OP1Freq = ((BaseFreq + Pitch * FreqRange) << RangeType) * pow(2, (pgm_read_float(&(voctpow[myread(VOCT_PIN)]))));  // V/oct 由于cltest0.6的voct接口阻抗问题 这里可能需要乘以一个系数 调谐才比较准确
  // Serial.print(" OP1Freq= ");
  // Serial.print(OP1Freq);
  // Serial.print(" OP1Freq>>16= ");
  // Serial.print(OP1Freq >> 16);

  /*FM量*/
  OP2Freq = (OP1Freq >> 1) * ((param[2] + myread(CV2_PIN)) >> 5);  //新版倍频算法 0.5-16倍频
  // OP2Freq = (OP1Freq >> 5) * ((param[2] + myread(CV2_PIN)) >> 5);           //新版倍频
  OP2Amt = ((OP2Freq >> 16) * (1 + param[3] + myread(CV3_PIN) + envgain));  //op2amount

  osc1.setFreq_Q16n16(OP1Freq);  //给主波形设置频率
  osc2.setFreq_Q16n16(OP2Freq);  //给算子设置频率

  //波形类型
  switch (param[8] >> 6) {
    default:
      osc1.setTable(SIN1024_DATA);
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
      osc1.setTable(WHITENOISE8192_DATA);
      break;
  }

  Serial.print("\n");
}

AudioOutput_t updateAudio() {
  env1.update();
  envgain = env1.next() << 1;
  Q15n16 modulation = OP2Amt * osc2.next() >> 8;
  return osc1.phMod(modulation);
}

void loop() {
  audioHook();
}