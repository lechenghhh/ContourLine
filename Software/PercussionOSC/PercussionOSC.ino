#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>
#include <tables/whitenoise8192_int8.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <tables/triangle512_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/triangle512_int8.h>

#include <tables/waveshape_compress_512_to_488_int16.h>  //wt compress
#include <tables/whitenoise8192_int8.h>

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

Q16n16 POSITION = 0;
String function[FUNC_LENGTH] = { "kick", "pitch", "clap", "freq", "tom", "pitch", "hat", "decay" };
int param[FUNC_LENGTH] = { 0, 1024, 0, 768, 0, 512, 0, 128 };
bool* ledGroup[FUNC_LENGTH] = { Led_K, Led_P, Led_C, Led_F, Led_T, Led_P, Led_H, Led_D };

//kick
Oscil<TRIANGLE512_NUM_CELLS, AUDIO_RATE> osc1(TRIANGLE512_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env1;
int gain1;
int tfg1 = 0;
int fall1 = 0;
//clap
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> osc2(WHITENOISE8192_DATA);  // audio noise
ADSR<AUDIO_RATE, AUDIO_RATE> env2;
LowPassFilter lpf2;
int gain2;
int trg2 = 0;
//tom
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> osc3(SQUARE_NO_ALIAS512_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env3;
int gain3;
int trg3 = 0;
int fall3 = 0;
//hat
Oscil<181, AUDIO_RATE> osc4(TRIANGLE512_DATA);
Oscil<103, AUDIO_RATE> osc5(TRIANGLE512_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env4;
int gain4;
int trg4 = 0;

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

  kick();
  clap();
  tom();
  hat();
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

void kick() {
  // int kickfreq = mozziAnalogRead(4) >> 4;
  int kickfreq = param[1] >> 4;

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

void clap() {
  osc2.setFreq((float)AUDIO_RATE / WHITENOISE8192_SAMPLERATE);
  int clapfreq = param[3] << 2;
  lpf2.setCutoffFreq(clapfreq + 200);
  // lpf2.setCutoffFreq(mozziAnalogRead(4) << 2);
  lpf2.setResonance(256);

  env2.setADLevels(255, 255);
  env2.setTimes(0, 16, 0, 2);
  if (digitalRead(12) == 0 && trg2 == 0) {
    trg2 = 1;
    env2.noteOn();
  }
  if (digitalRead(12) == 1 && trg2 == 1) {
    trg2 = 0;
    env2.noteOff();
  }
  env2.update();

  gain2 = env2.next();
  // for (int i = 0; i < gain2 / 2; i++)
  //   Serial.print("]");
}

void tom() {
  int tomFreq = 64 + param[5] >> 4;
  // int tomFreq = 64 + mozziAnalogRead(4) >> 4;

  env3.setADLevels(255, 255);
  env3.setTimes(0, 16, 0, 2);
  if (digitalRead(12) == 0 && trg3 == 0) {
    trg3 = 1;
    fall3 = 30;
    env3.noteOn();
    env3.noteOff();
  }
  if (digitalRead(12) == 1 && trg3 == 1) {
    trg3 = 0;
  }
  env3.update();
  gain3 = env3.next();

  osc3.setFreq(tomFreq + fall3);
  fall3 -= 1;
  // gain3 = 255;//test

  // for (int i = 0; i < fall3; i++)
  //   Serial.print("]");
}

void hat() {
  int hihatfreq = param[7] << 4;
  // int hihatfreq = mozziAnalogRead(4) << 4;

  env4.setADLevels(255, 255);
  env4.setTimes(0, 8, 0, 2);
  if (digitalRead(12) == 0 && trg4 == 0) {
    trg4 = 1;
    env4.noteOn();
    env4.noteOff();
  }
  if (digitalRead(12) == 1 && trg4 == 1) {
    trg4 = 0;
  }
  env4.update();
  gain4 = env4.next();

  osc4.setFreq(hihatfreq);
  osc5.setFreq(4000);
}

AudioOutput_t updateAudio() {
  // int gain_cv_val = 255;
  // int asig1 = osc1.next() * N0Amp >> 10;
  // int asig2 = osc2.next() * N1Amp >> 10;
  // int asig3 = osc3.next() * N2Amp >> 10;
  // int asig4 = osc4.next() * N3Amp >> 10;

  //kick
  int asig1 = gain1 * (osc1.next()) >> 8;
  //clap
  int asig2 = gain2 * lpf2.next(osc2.next()) >> 8;
  // //tom
  int asig3 = gain3 * (osc3.next()) >> 8;
  // //hat
  int asig4 = gain4 * (osc4.phMod(600 * osc5.next() >> 8)) >> 8;

  return MonoOutput::fromNBit(8, (asig1 + asig2 + asig3 + asig4));
}

void loop() {
  audioHook();
}