//主程序 逻辑
#include "Module_LEDDisplay.h"  //自定义库封装led 控制 常量
#include "Module_Ctrl.h"
#include "Module_Const.h"

#include <MozziGuts.h>
#include <Oscil.h>               // oscillator template
#include <tables/sin256_int8.h>  // sine table for oscillator
#include <tables/triangle_analogue512_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/waveshape_chebyshev_4th_256_int8.h>

#define FUNCTION_LENGTH 10  //总菜单数
#define CONTROL_RATE 128    //控制速率
#define KNOB_PIN 4          //旋钮引脚
#define BTN1_PIN 12         //按钮引脚
#define BTN2_PIN 13         //按钮引脚
#define OUTA_PIN 11         //OUTA引脚
#define IN0_PIN 0           //IV OCT
#define IN1_PIN 1           //FM
#define IN2_PIN 2           //Gate
#define IN3_PIN 3           //Mod

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> aSin1(SIN256_DATA);
Oscil<HALFSIN256_NUM_CELLS, AUDIO_RATE> aHSin1(HALFSIN256_DATA);
Oscil<PHASOR256_NUM_CELLS, AUDIO_RATE> aPha1(PHASOR256_DATA);
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> aSqu1(SQUARE_NO_ALIAS512_DATA);

Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aTra2(TRIANGLE_ANALOGUE512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw2(SAW256_DATA);
Oscil<CHEBYSHEV_4TH_256_NUM_CELLS, AUDIO_RATE> aCheb2(CHEBYSHEV_4TH_256_DATA);
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> aSqu2(SQUARE_NO_ALIAS512_DATA);

// Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNos(WHITENOISE8192_DATA);

int POSITION = 0;  //菜单下标
String function[FUNCTION_LENGTH] = { "Wave1", "Shape1", "Freq1", "Lv1", "Mod1", "Wave2", "Shape2", "Freq2", "Lv2", "Mod2" };
int param[FUNCTION_LENGTH] = { 0, 1, 2, 3, 4, 0, 1, 2, 3, 4 };  // 给部分数组元素赋值
bool* ledGroup[FUNCTION_LENGTH] = { Led_W, Led_S, Led_P, Led_V, Led_F, Led_Q, Led_A, Led_R };

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KNOB_PIN, 50, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化led引脚
  startMozzi(CONTROL_RATE);                          //启动mozzi库
}

int Wave1 = 0, Wave2 = 0;
int Shape1 = 0, Shape2 = 0;
float Freq1 = 0, Freq2 = 0;
int Lv1 = 0, Lv2 = 0;
int Mod1 = 0, Mod2 = 0;
void updateControl() {

  POSITION = getPostition(POSITION, FUNCTION_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);         //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                      //display  //用以展示控制

  Serial.print(POSITION + function[POSITION] + param[POSITION]);  //func param
  // for (int ii = 0; ii < 10; ii++) Serial.print("-" + function[ii] + param[ii] + " ");  //func param list
  Serial.println("                  ");

  Wave1 = param[0] >> 8;   //4种波形
  Wave2 = param[5] >> 8;   //4种波形
  Shape1 = param[1] >> 3;  //128个渐变
  Shape2 = param[6] >> 3;  //128个渐变
  Freq1 = param[2] / 100;  //0.01-10hz
  Freq2 = param[7] / 100;  //0.01-10hz
  Lv1 = param[3] >> 2;     //256
  Lv2 = param[8] >> 2;     //256
  Mod1 = param[4] >> 2;    //256
  Mod2 = param[9] >> 2;    //256

  switch (Wave1) {
    default:
      aSin1.setFreq(Freq1);  //设置频率  // aSin1.setFreq(analogRead(0));
      break;
    case 1:
      aHSin1.setFreq(Freq1);  //设置频率  // aSin1.setFreq(analogRead(0));
      break;
    case 2:
      aPha1.setFreq(Freq1);
      break;
    case 3:
      aSqu1.setFreq(Freq1);
      break;
  }
  switch (Wave2) {
    default:
      aTra2.setFreq(Freq2);
      break;
    case 1:
      aSaw2.setFreq(Freq2);
      break;
    case 2:
      aCheb2.setFreq(Freq2);
      break;
    case 3:
      aSqu2.setFreq(Freq2);
      break;
  }
}

int updateAudio() {
  int asig1 = 0, asig2 = 0;
  switch (Wave1) {
    default:
      analogWrite(OUTA_PIN, aSin1.next() * Lv2 >> 6);  //对OUTA_PIN输出pwm
      break;
    case 1:
      analogWrite(OUTA_PIN, aHSin1.next() * Lv2 >> 6);  //对OUTA_PIN输出pwm
      break;
    case 2:
      analogWrite(OUTA_PIN, aPha1.next() * Lv2 >> 6);  //对OUTA_PIN输出pwm
      break;
    case 3:
      analogWrite(OUTA_PIN, aSqu1.next() * Lv2 >> 6);  //对OUTA_PIN输出pwm
      break;
  }

  switch (Wave2) {
    default:
      return (aTra2.next() * Lv2) >> 8;  // return an int signal centred around 0
    case 1:
      return (aSaw2.next() * Lv2) >> 8;
    case 2:
      return (aCheb2.next() * Lv2) >> 8;
    case 3:
      return (aSqu2.next() * Lv2) >> 8;
  }
}

void loop() {   //这里实时音频处理 尽量不要额外的事情
  audioHook();  //Mozzi required here
}
