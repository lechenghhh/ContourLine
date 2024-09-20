#include <Mozzi.h>
#include <Oscil.h>
#include <WaveShaper.h>
#include <FixMath.h>
#include <LowPassFilter.h>

//8192 16384
#include <tables/whitenoise8192_int8.h>        //WHITENOISE8192_DATA
#include <tables/noise_static_1_16384_int8.h>  //NOISE_STATIC_1_16384_DATA
#include <tables/brownnoise8192_int8.h>        //BROWNNOISE8192_DATA
#include <tables/pinknoise8192_int8.h>         //PINKNOISE8192_DATA


/*lecheng的控制/显示模块封装*/
#include "Module_LEDDisplay.h"
#include "Module_Ctrl.h"
#include "Module_Const.h"

#define CONTROL_RATE 256  //Hz, powers of 2 are most reliable
#define PARAM_LENGTH 5     //功能列表长度

/*   引脚定义   */
#define KONB_PIN 4   //
#define VOCT_PIN 0   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define GATE_PIN 11  //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //

Oscil<8192, AUDIO_RATE> wdosc1(WHITENOISE8192_DATA);

byte POSITION = 0;
// String param_name[PARAM_LENGTH] = { "Pitch", "Cutof", "Reso", "Vol", "BitC", "Type" };
int param[PARAM_LENGTH] = { 256, 128, 768, 0, 0 };
bool* ledGroup[PARAM_LENGTH] = { Led_F, Led_Q, Led_A, Led_B, Led_T };
LowPassFilter lpf1;
int amp = 0, bitcrush = 0;

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化Led引脚

  startMozzi(CONTROL_RATE);
}

void updateControl() {
  /*控制参数获取与显示逻辑*/
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示
  Serial.print(POSITION);                          //func param Log
  Serial.print("func=");                           //func param Log
  Serial.print(param[POSITION]);                   //func param Log

  wdosc1.setFreq(440);
  int cutof = param[0] + mozziAnalogRead(0);
  if (cutof > 1024) cutof = 1024;
  int reso = param[1] + mozziAnalogRead(1);
  if (reso > 1024) reso = 1024;
  lpf1.setCutoffFreqAndResonance(cutof / 6, reso / 4);


  amp = param[2] + mozziAnalogRead(2);
  if (amp > 1024) amp = 1024;
  amp = amp / 128;

  bitcrush = param[3] + mozziAnalogRead(3);
  if (bitcrush > 1024) bitcrush = 1024;
  bitcrush = bitcrush / 200;

  switch (param[5] / 256) {
    default:
      wdosc1.setTable(WHITENOISE8192_DATA);
      break;
    // case 1:
    //   wdosc1.setTable(NOISE_STATIC_1_16384_DATA);
    //   break;
    case 2:
      wdosc1.setTable(BROWNNOISE8192_DATA);
      break;
    case 3:
      wdosc1.setTable(PINKNOISE8192_DATA);

      break;
  }
  // Serial.print("-WaveChange= ");
  // Serial.println(WaveChange);
  // Serial.print("-ShapeGradient-");
  // Serial.println(ShapeGradient);
  Serial.println("\n");
}

AudioOutput updateAudio() {
  // return MonoOutput::fromNBit(16, (wdosc1.next() << 8));  //原始正弦波输出 无任何渐变
  return MonoOutput::fromNBit(16 - bitcrush, (lpf1.next(wdosc1.next()) << amp));  //原始正弦波输出 无任何渐变
}

void loop() {
  audioHook();
}