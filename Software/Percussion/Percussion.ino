#include <MozziGuts.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <mozzi_fixmath.h>
#include <ADSR.h>
#include <LowPassFilter.h>
#include <tables/whitenoise8192_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/triangle512_int8.h>
#include <tables/waveshape_compress_512_to_488_int16.h>  //wt compress
#include <tables/whitenoise8192_int8.h>

//lecheng的控制/显示模块封装
#include "Module_LEDDisplay.h"
#include "Module_Ctrl.h"
#include "Module_Const.h"

#define CONTROL_RATE 256  //Hz, powers of 2 are most reliable
#define FUNC_LENGTH 8     //功能列表长度

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
String function[FUNC_LENGTH] = { "kick", "tone", "clap", "freq", "hat", "decay", "rim", "pitch" };
int param[FUNC_LENGTH] = { 768, 1024, 768, 700, 768, 256, 768, 512 };
bool* ledGroup[FUNC_LENGTH] = { Led_K, Led_T, Led_C, Led_F, Led_H, Led_D, Led_R, Led_P };

//kick
Oscil<TRIANGLE512_NUM_CELLS, AUDIO_RATE> osc1(TRIANGLE512_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env1;
int adsr1;
int gain1;
int trg1 = 0;
int fall1 = 0;
//clap
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> osc2(WHITENOISE8192_DATA);  // audio noise
ADSR<AUDIO_RATE, AUDIO_RATE> env2;
LowPassFilter lpf2;
int adsr2;
int gain2;
int trg2 = 0;
//hat
Oscil<244, AUDIO_RATE> osc3(WHITENOISE8192_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env3;
int adsr3;
int gain3;
int trg3 = 0;
//rim
Oscil<181, AUDIO_RATE> osc4(TRIANGLE512_DATA);
Oscil<103, AUDIO_RATE> osc5(TRIANGLE512_DATA);
Oscil<103, AUDIO_RATE> osc6(TRIANGLE512_DATA);
ADSR<AUDIO_RATE, AUDIO_RATE> env4;
int adsr4;
int gain4;
int trg4 = 0;

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN2_PIN, BTN1_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
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
  hat();
  rim();
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
  // Serial.print(" d11= ");
  // Serial.print(digitalRead(11));
  // Serial.print(" d12= ");
  // Serial.println(digitalRead(12));
  // Serial.print(" d13= ");
  // Serial.println(digitalRead(13));
}

void kick() {
  int a1 = mozziAnalogRead(1);

  gain1 = param[0] >> 2;

  // int kickfreq = mozziAnalogRead(4) >> 4;
  int kickfreq = (param[1] >> 4) + (mozziAnalogRead(0) >> 3);

  env1.setADLevels(255, 255);
  env1.setTimes(0, 16, 0, 2);
  if (a1 > 500 && trg1 == 0) {
    trg1 = 1;
    fall1 = 30;
    env1.noteOn();
    env1.noteOff();
  }
  if (a1 < 501 && trg1 == 1) {
    trg1 = 0;
  }
  env1.update();
  adsr1 = env1.next();

  osc1.setFreq(kickfreq + fall1);
  fall1 -= 3;
}

void clap() {
  int a2 = mozziAnalogRead(2);

  gain2 = param[2] >> 2;
  osc2.setFreq((float)AUDIO_RATE / WHITENOISE8192_SAMPLERATE);
  int clapfreq = param[3] << 2;
  lpf2.setCutoffFreq(clapfreq + mozziAnalogRead(0) / 4);
  // lpf2.setCutoffFreq(mozziAnalogRead(4) << 2);
  lpf2.setResonance(256);

  env2.setADLevels(255, 255);
  env2.setTimes(0, 8, 0, 1);
  if (a2 > 500 && trg2 == 0) {
    trg2 = 1;
    env2.noteOn();
    env2.noteOff();
  }
  if (a2 < 501 && trg2 == 1) {
    trg2 = 0;
  }
  env2.update();

  adsr2 = env2.next();
  // for (int i = 0; i < adsr2 / 2; i++)
  //   Serial.print("]");
}

void hat() {
  int a3 = mozziAnalogRead(3);

  gain3 = param[4] >> 2;
  int hatFreq = 8000 + (mozziAnalogRead(0) << 3);
  // int hatFreq = 64 + mozziAnalogRead(4) >> 4;

  osc3.setFreq(hatFreq);
  osc6.setFreq(10000);

  env3.setADLevels(255, 255);
  env3.setTimes(0, 72 + (param[5] >> 5), 0, param[5] >> 7);
  // if (digitalRead(13) == 0 && trg3 == 0) {//test
  if (a3 > 500 && trg3 == 0) {
    trg3 = 1;
    env3.noteOn();
    env3.noteOff();
  }
  // if (digitalRead(13) == 1 && trg3 == 1) {//test
  if (a3 < 501 && trg3 == 1) {
    trg3 = 0;
  }
  env3.update();
  adsr3 = env3.next();
  // for (int i = 0; i < fall3; i++)
  //   Serial.print("]");
}

void rim() {
  gain4 = param[6] >> 2;

  int hirimfreq = param[7] << 2;
  int hirimfreqMOD = mozziAnalogRead(0) << 2;
  hirimfreq = hirimfreq + hirimfreqMOD;


  env4.setADLevels(255, 255);
  env4.setTimes(0, 16, 0, 4);
  if (digitalRead(11) == 1 && trg4 == 0) {
    trg4 = 1;
    env4.noteOn();
    env4.noteOff();
  }
  if (digitalRead(11) == 0 && trg4 == 1) {
    trg4 = 0;
  }
  env4.update();
  adsr4 = env4.next();

  osc4.setFreq(hirimfreq);
  // osc4.setFreq(12500);
  osc5.setFreq(mozziAnalogRead(0) + 5500);
}

AudioOutput_t updateAudio() {

  //kick
  int asig1 = adsr1 * (osc1.next()) >> 8;
  //clap
  int asig2 = adsr2 * lpf2.next(osc2.next()) >> 8;
  //hat
  int asig3 = adsr3 * (osc3.phMod(12000 * osc6.next() >> 8)) >> 8;
  //rim
  int asig4 = adsr4 * (osc4.phMod(600 * osc5.next() >> 8)) >> 8;
  asig1 = gain1 * asig1 >> 8;
  asig2 = gain2 * asig2 >> 8;
  asig3 = gain3 * asig3 >> 8;
  asig4 = gain4 * asig4 >> 8;

  return MonoOutput::fromNBit(8, (asig1 + asig2 + asig3 + asig4));
}

void loop() {
  audioHook();
}