#include <MozziGuts.h>
//主程序 逻辑
#include "Module_LEDDisplay.h"  //自定义库封装led 控制 常量
#include "Module_Ctrl.h"
#include "Module_Const.h"

#include <Oscil.h>               // oscillator template
#include <tables/sin256_int8.h>  // sine table for oscillator
#include <tables/triangle_analogue512_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/waveshape_chebyshev_4th_256_int8.h>
#include <tables/whitenoise8192_int8.h>
#include <LowPassFilter.h>
#include <ADSR.h>

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

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> aSin(SIN256_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aTra(TRIANGLE_ANALOGUE512_DATA);
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> aSqu(SQUARE_NO_ALIAS512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw(SAW256_DATA);
Oscil<PHASOR256_NUM_CELLS, AUDIO_RATE> aPha(PHASOR256_DATA);
Oscil<HALFSIN256_NUM_CELLS, AUDIO_RATE> aHSin(HALFSIN256_DATA);
Oscil<CHEBYSHEV_4TH_256_NUM_CELLS, AUDIO_RATE> aCheb(CHEBYSHEV_4TH_256_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNos(WHITENOISE8192_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> aModulator(SIN256_DATA);
LowPassFilter lpf;
ADSR<AUDIO_RATE, AUDIO_RATE> envelope;

int POSITION = 0;  //菜单下标
String function[FUNCTION_LENGTH] = { "Wave", "Shape", "Pitch", "Vol", "Cutof", "ResQ", "Attk", "Rele", "FM", "FMA" };
int param[FUNCTION_LENGTH] = { 0, 3, 440, 1024, 972, 128, 0, 1024, 0, 0 };  // 给部分数组元素赋值
// bool* ledGroup[FUNCTION_LENGTH] = { Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, };
bool* ledGroup[FUNCTION_LENGTH] = { Led_W, Led_S, Led_P, Led_V, Led_C, Led_Q, Led_A, Led_R, Led_F, Led_M };

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KNOB_PIN, 50, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化Led引脚
  startMozzi(CONTROL_RATE);                          //启动mozzi库
}

int Wave = 0;
int Pitch = 0;
int Vol = 0;
int Cutof = 0;
int ResQ = 0;
int FM = 0;
int FMA = 0;

void updateControl() {

  POSITION = getPostition(POSITION, FUNCTION_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);         //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                      //display  //用以展示控制

  Serial.print(POSITION + function[POSITION] + param[POSITION]);  //func param
  // for (int ii = 0; ii < 10; ii++) Serial.print("-" + function[ii] + param[ii] + " ");  //func param list
  Serial.println("                  ");

  //在这里修改参数 先将1024转化成具体数值 再设置配置
  Wave = param[0] >> 7;   //波表  将1023分成8个波表类型
  Pitch = param[2] + 20;  //频率调整 20hz-2048hz
  Vol = param[3] >> 3;    //0-256
  Cutof = param[4] >> 2;
  ResQ = param[5] >> 2;

  // 获取频率
  int toneFreq = Pitch * pow(2, (pgm_read_float(&(voctpow[mozziAnalogRead(IN0_PIN)]))));  // V/oct no fm

  // 设置调频
  // int toneFreq2 = (2270658 + Pitch * 5000) * pow(2, (pgm_read_float(&(voctpow[analogRead(IN0_PIN)]))));  // V/oct fm
  // FM = (toneFreq * ((param[8] + mozziAnalogRead(IN1_PIN)) >> 6));  //test
  FM = (int)toneFreq * (((float)map(param[8], 0, 1023, 0.5, 16) + (float)map(mozziAnalogRead(IN1_PIN), 0, 1023, 0.5, 16)));
  FMA = (FM * (param[9] + mozziAnalogRead(IN3_PIN)) >> 8);
  Serial.print(FM);  //(0.5~16 + 0.5~16) x pitch
  Serial.print("-----");
  Serial.println(FMA);
  aModulator.setFreq(FM);

  //设置频率
  switch (Wave) {
    default:
      aSin.setFreq(toneFreq);
      break;
    case 1:
      aTra.setFreq(toneFreq);
      break;
    case 2:
      aSqu.setFreq(toneFreq);
      break;
    case 3:
      aSaw.setFreq(toneFreq);
      break;
    case 4:
      aPha.setFreq(toneFreq);
      break;
    case 5:
      aHSin.setFreq(toneFreq);
      break;
    case 6:
      aCheb.setFreq(toneFreq);
      break;
    case 7:
      aNos.setFreq(toneFreq);
      break;
  }

  // 设置滤波器
  lpf.setCutoffFreq(Cutof);
  lpf.setResonance(ResQ);

  //设置包络
  if (param[7] < 1000) {  //如果release大于1000 则启用持续震荡模式
    envelope.setADLevels(255, 255);
    envelope.setTimes(param[6] >> 4, param[7] >> 4, param[7] >> 4, param[7] >> 4);
    if (mozziAnalogRead(IN2_PIN) > 800)
      envelope.noteOn();
    else
      envelope.noteOff();
    envelope.update();
    Vol = envelope.next() * Vol / 255;  // 这就是它与音频速率包络不同的地方

    analogWrite(OUTA_PIN, Vol);  //对OUTA_PIN输出pwm
  }
}

int updateAudio() {
  char asig = 0;
  //FM运算
  int tmpMod = FMA * aModulator.next();
  switch (Wave) {
    default:
      asig = aSin.phMod(tmpMod);  // asig = aSin.next();
      break;
    case 1:
      asig = aTra.phMod(tmpMod);
      break;
    case 2:
      asig = aSqu.phMod(tmpMod);
      break;
    case 3:
      asig = aSaw.phMod(tmpMod);
      break;
    case 4:
      asig = aPha.phMod(tmpMod);
      break;
    case 5:
      asig = aHSin.phMod(tmpMod);
      break;
    case 6:
      asig = aCheb.phMod(tmpMod);
      break;
    case 7:
      asig = aNos.phMod(tmpMod);
      break;
  }
  return lpf.next((asig * Vol) >> 8);
}

void loop() {   //这里实时音频处理 尽量不要额外的事情
  audioHook();  //Mozzi required here
}
