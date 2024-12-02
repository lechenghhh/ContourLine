
#include <MozziGuts.h>
#include <Oscil.h>  // oscillator template

#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable
#include <MozziConfigValues.h>
#include <EventDelay.h>
#include <Mozzi.h>
#include <tables/whitenoise8192_int8.h>
#include <tables/brownnoise8192_int8.h>
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

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> osc(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> FMop(SIN1024_DATA);
ADSR<MOZZI_AUDIO_RATE, MOZZI_AUDIO_RATE> envelope;
EventDelay noteDelay;

Q16n16 WaveType = 0;

boolean note_is_on = true;
byte POSITION = 0;
#define PARAM_LENGTH 5
char param_name[PARAM_LENGTH][3] = { "P", "D", "T", "F", "A" };
int param[PARAM_LENGTH] = { 0, 127, 0, 0, 0 };
bool* ledGroup[PARAM_LENGTH] = { Led_P, Led_D, Led_T, Led_F, Led_A };
int lastPinState = LOW;
bool param_nameTriggered = false;
void setup() {
  pinMode(TRIG_PIN, INPUT);
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);
  noteDelay.set(2000);
  startMozzi(CONTROL_RATE);
}

unsigned int duration, attack, decay, sustain, release_ms;
int Freq;
int FmOPFreq;
int OPAmt;
void updateControl() {

  //菜单控制代码
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                   //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);   //如果处在非编辑状态 led将半灭显示

  // Serial.print(POSITION);           //func param
  // Serial.println("func");           //func param
  // Serial.println(param[POSITION]);  //func param

  //波形选择
  WaveType = param[2] / 333 + mozziAnalogRead(CV2_PIN) / 300;

  switch (WaveType) {
    case 0:
      osc.setTable(SIN1024_DATA);
      break;
    case 1:
      osc.setTable(SAW1024_DATA);
      break;
    case 2:
      osc.setTable(BROWNNOISE8192_DATA);
      break;
    case 3:
      osc.setTable(WHITENOISE8192_DATA);
      break;
  }

  //包络构建与触发
  envelope.setLevels(255, 0, 127, 0);

  envelope.setTimes(0, 255, 0, param[1] + mozziAnalogRead(CV1_PIN));
  int TRIG = digitalRead(TRIG_PIN);
  if (TRIG == HIGH && lastPinState == LOW) {
    // 检测到上升沿，触发noteOn
    envelope.noteOn();
    noteDelay.start(1);  // 重置EventDelay
  }
  lastPinState = TRIG;  // 更新引脚状态

  if (noteDelay.ready()) {
    // 如果经过了设定的时间，触发noteOff
    envelope.noteOff();
  }

  //FM调制与频率
  FmOPFreq = param[3] + mozziAnalogRead(CV3_PIN);
  Freq = param[0] + mozziAnalogRead(CV_PIN);
  osc.setFreq(envelope.next() + Freq);
  FMop.setFreq(FmOPFreq);
  OPAmt = param[4];
}


int updateAudio() {

  Q15n16 modulation = OPAmt * FMop.next();
  envelope.update();
  if (envelope.next() > 1) {
    // 如果包络正在活动，则正常计算音频输出
    int output = MonoOutput::from16Bit((int)(envelope.next() * osc.phMod(modulation)));

    return output;
  } else {
    // 如果包络已关闭，则返回静音
    return 0;  // 或者使用MonoOutput的静音值，如果它有的话
  }
}




void loop() {
  audioHook();
}
