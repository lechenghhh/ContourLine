/*
  这是一个标准的四段ADSR例程
  作者 lecheng
*/

#include <MozziGuts.h>
#include <Oscil.h>  // oscillator template

#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

#include <tables/sin1024_int8.h>
#include <tables/saw1024_int8.h>
#include <ADSR.h>
#include <FixMath.h>

#define TRIG_PIN 11
#define KONB_PIN 4   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define CV_PIN 0     //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> osc1(SIN1024_DATA);
ADSR<MOZZI_AUDIO_RATE, MOZZI_AUDIO_RATE> env1;

const static int PARAM_LENGTH = 5;
byte POSITION = 0;
String param_name[PARAM_LENGTH] = { "Attk", "Decay", "Sustain", "Release", "Pitch" };
int param[PARAM_LENGTH] = { 0, 512, 512, 512, 440 };
bool* ledGroup[PARAM_LENGTH] = { Led_A, Led_D, Led_S, Led_R, Led_P };
byte trig1 = 0;
int gain1 = 0;
void setup() {
  pinMode(TRIG_PIN, INPUT);
  Serial.begin(115200);
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);
  startMozzi(CONTROL_RATE);
}

void updateControl() {
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标  //菜单控制代码
  param[POSITION] = getParam(param[POSITION]);      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                   //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);   //如果处在非编辑状态 led将半灭显示

  osc1.setFreq(param[4]);

  Serial.print(param_name[POSITION]);
  Serial.print(param[POSITION]);
  Serial.print(" d11=");
  Serial.print(digitalRead(TRIG_PIN));
  Serial.print(" trig1=");
  Serial.print(trig1);
  Serial.print(" gain1=");
  Serial.print(gain1);
  for (int i = 0; i < gain1 / 2; i++) Serial.print("|");
  Serial.print("\n");

  //四段包络例子
  env1.setLevels(255, 176, 96, 0);
  env1.setTimes(param[0], param[1], param[2], param[3]);
  if (digitalRead(TRIG_PIN) == 1 && trig1 == 0) {
    trig1 = 1;
    env1.noteOn();
  }
  if (digitalRead(TRIG_PIN) == 0 && trig1 == 1) {
    trig1 = 0;
    env1.noteOff();
  }
  //二段AD包络例子
  // env1.setLevels(255, 0, 0, 0);
  // env1.setTimes(param[0], param[1], param[2], param[3]);
  // if (digitalRead(TRIG_PIN) == 1 && trig1 == 0) {
  //   trig1 = 1;
  //   env1.noteOn();
  // }
  // if (digitalRead(TRIG_PIN) == 0 && trig1 == 1) {
  //   trig1 = 0;
  //   env1.noteOff();
  // }
  //二段AR包络 但是依然不能保持住
  // env1.setLevels(255, 0, 255, 0);
  // env1.setTimes(param[0], param[1], param[2], param[3]);
  // if (digitalRead(TRIG_PIN) == 1 && trig1 == 0) {
  //   trig1 = 1;
  //   env1.noteOn();
  //   env1.noteOff();
  // }
  // if (digitalRead(TRIG_PIN) == 0 && trig1 == 1) {
  //   trig1 = 0;
  // }
  // 二段AR包络 使用一个尽可能长的延音数字 可以模拟保持住的状态 保持时间最长两秒
  // env1.setLevels(255, 196, 196, 0);
  // env1.setTimes(param[0], param[1], 5000, param[3]);
  // if (digitalRead(TRIG_PIN) == 1 && trig1 == 0) {
  //   trig1 = 1;
  //   env1.noteOn();
  // }
  // if (digitalRead(TRIG_PIN) == 0 && trig1 == 1) {
  //   trig1 = 0;
  //   env1.noteOff();
  // }
}

int updateAudio() {
  env1.update();
  gain1 = env1.next();
  if (gain1 > 1) {
    // 如果包络正在活动，则正常计算音频输出
    int output = MonoOutput::from16Bit((int)(gain1 * osc1.next()));

    return output;
  } else {     // 如果包络已关闭，则返回静音
    return 0;  // 或者使用MonoOutput的静音值，如果它有的话
  }
}

void loop() {
  audioHook();
}
