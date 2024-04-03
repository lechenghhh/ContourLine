#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>

#include <tables/sin512_int8.h>  // table for Oscils to play

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

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> osc1(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> osc2(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> osc3(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> osc4(SIN512_DATA);

WaveShaper<int> wsComp(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals


Q16n16 POSITION = 0;
String function[FUNC_LENGTH] = { "RootP", "N1Amp", "Note2", "N2Amp", "Note3", "N3Amp" };
int param[FUNC_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5, Led_6 };

Q16n16 RangeType = 1;       //C0
Q16n16 BaseFreq = 2143658;  //C0
Q16n16 FreqRange = 5200;    //2OCT
Q16n16 OP1Freq, Pitch;
Q16n16 WaveType = 0;

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
  OP1Freq = (BaseFreq + Pitch * FreqRange) * pow(2, (pgm_read_float(&(voctpow[mozziAnalogRead(VOCT_PIN)]))));  // V/oct 由于cltest0.6的voct接口阻抗问题 这里可能需要乘以一个系数 调谐才比较准确

  osc1.setFreq_Q16n16(OP1Freq);      //给主波形设置频率、音高
  osc2.setFreq_Q16n16(OP1Freq * 2);  //给主波形设置频率、音高
  osc3.setFreq_Q16n16(OP1Freq * 3);  //给主波形设置频率、音高
  osc4.setFreq_Q16n16(OP1Freq * 4);  //给主波形设置频率、音高

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
  // Serial.print(" d11= ");
  // Serial.print(digitalRead(11));
  // Serial.print("-WaveTrig= ");
}

AudioOutput_t updateAudio() {
  int inv_aply5 = 1;  //0 = no output root sound , 1 = output root sound
  int gain_cv_val = 511;
  // return MonoOutput::fromNBit(16, ((osc1.next() / 8 + osc2.next() / 8 + osc3.next() / 8 + osc4.next() / 8 * inv_aply5) * gain_cv_val));
  return MonoOutput::fromNBit(8, osc1.next());
}

void loop() {
  audioHook();
}