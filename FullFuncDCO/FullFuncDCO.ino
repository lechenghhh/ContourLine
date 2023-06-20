//主程序 逻辑
#include "Modual_LEDDisplay.h"
#include "Modual_Ctrl.h"
#include "Modual_Const.h"

#include <MozziGuts.h>
#include <Oscil.h>  // oscillator template
#include <LowPassFilter.h>
#include <ADSR.h>

#include <tables/sin256_int8.h>  // sine table for oscillator
#include <tables/triangle_analogue512_int8.h>
#include <tables/square_no_alias512_int8.h>
#include <tables/saw256_int8.h>
#include <tables/phasor256_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/waveshape_chebyshev_4th_256_int8.h>
#include <tables/whitenoise8192_int8.h>

#define FUNCTION_LENGTH 10  //总菜单数
#define CONTROL_RATE 64     //控制速率
#define KNOB_PIN 0          //旋钮引脚
#define BTN1_PIN 12         //按钮引脚
#define BTN2_PIN 13         //按钮引脚

String function[FUNCTION_LENGTH] = {
  "Wave", "Shape", "Pitch", "Vol", "Cutof", "Reso.", "Attk.", "Decay", "Sus.", "Rel.",
  // "FM", "AM"
  // "L1F", "L1A", // "L1Wav", "L1Vol", "L1Cut", "L1Res"  //function[10]
};
int param[FUNCTION_LENGTH] = {
  0,
  3,
  440,
  512,
  972,
  128,
  4,
  128,
  256,
  12,
  // 0,  //FM
  // 0,  //AM
  // 0,
  // 0,
  // 0,
  // 0,
  // 0,
};  // 给部分数组元素赋值
bool* ledGroup[FUNCTION_LENGTH] = { Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL, Led_NULL };
// bool* ledGroup[FUNCTION_LENGTH] = { Led_W, Led_S, Led_P, Led_V, Led_C, Led_R, Led_A, Led_D, Led_S, Led_R };
// bool* ledGroup[FUNCTION_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5, Led_6, Led_7, Led_8, Led_9, Led_0 };
// bool* ledGroup[FUNCTION_LENGTH] = { Led_A, Led_B, Led_C, Led_D, Led_E, Led_F, Led_G, Led_H, Led_I, Led_J };

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> aSin(SIN256_DATA);
Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> aTra(TRIANGLE_ANALOGUE512_DATA);
Oscil<SQUARE_NO_ALIAS512_NUM_CELLS, AUDIO_RATE> aSqu(SQUARE_NO_ALIAS512_DATA);
Oscil<SAW256_NUM_CELLS, AUDIO_RATE> aSaw(SAW256_DATA);
Oscil<PHASOR256_NUM_CELLS, AUDIO_RATE> aPha(PHASOR256_DATA);
Oscil<HALFSIN256_NUM_CELLS, AUDIO_RATE> aHSin(HALFSIN256_DATA);
Oscil<CHEBYSHEV_4TH_256_NUM_CELLS, AUDIO_RATE> aCheb(CHEBYSHEV_4TH_256_DATA);
Oscil<WHITENOISE8192_NUM_CELLS, AUDIO_RATE> aNos(WHITENOISE8192_DATA);

ADSR<AUDIO_RATE, AUDIO_RATE> envelope;
LowPassFilter lpf;

void setup() {
  Serial.begin(115200);                              // 使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KNOB_PIN, 50, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数
  initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化led引脚

  startMozzi(CONTROL_RATE);  // :)
}

int Wave = 0;
int Pitch = 0;
int Vol = 0;
int Cutof = 0;
int Reso = 0;

int FM = 0;
int AM = 0;
void updateControl() {
  
  param[POSITION] = getCtrl(param);  //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);    //display  //用以展示控制

  Serial.print(function[POSITION] + param[POSITION] + " | ");  //func param
  for (int ii = 0; ii < 10; ii++) {
    Serial.print(function[ii] + param[ii] + " ");  //func param
  }
  Serial.println("                  ");

  //在这里修改参数 先做运算 再设置配置
  Wave = param[0] >> 7;  //波表  将1023分成8个波表类型
  Pitch = param[2];
  Vol = param[3] >> 3;
  Cutof = param[4] >> 2;
  Reso = param[5] >> 2;
  lpf.setCutoffFreq(Cutof);
  lpf.setResonance(Reso);

  // lfo.setFreq(param[10]);
  // int oct_cv_val = analogRead(2);
  // int toneFreq = (2270658 + Pitch * 5000) * pow(2, (pgm_read_float(&(voctpow[oct_cv_val]))));

  //设置波形 设置频率
  switch (Wave) {
    default:
      aSin.setFreq(Pitch);  //设置频率  // aSin.setFreq(analogRead(0));
      break;
    case 1:
      aTra.setFreq(Pitch);  //设置频率  // aSin.setFreq(analogRead(0));
      break;
    case 2:
      aSqu.setFreq(Pitch);
      break;
    case 3:
      aSaw.setFreq(Pitch);
      break;
    case 4:
      aPha.setFreq(Pitch);
      break;
    case 5:
      aHSin.setFreq(Pitch);
      break;
    case 6:
      aCheb.setFreq(Pitch);
      break;
    case 7:
      aNos.setFreq(Pitch);
      break;
  }
}

int updateAudio() {
  // return aSin.next()  << 6;  // 8 bits scaled up to 14 bits
  // Q15n16 vibrato = (Q15n16)L1A * lfo.next();
  switch (Wave) {
    default:
      return lpf.next((aSin.next() * Vol) >> 8);  // return an int signal centred around 0
      // return MonoOutput::fromNBit(16, (aSin.phMod(Q15n16(param[12] * analogRead(3) >> 8)) / 2 * Vol) * (analogRead(4) / 2));
    case 1:
      return lpf.next((aTra.next() * Vol) >> 8);
    case 2:
      return lpf.next((aSqu.next() * Vol) >> 8);
    case 3:
      return lpf.next((aSaw.next() * Vol) >> 8);
    case 4:
      return lpf.next((aPha.next() * Vol) >> 8);
    case 5:
      return lpf.next((aHSin.next() * Vol) >> 8);
    case 6:
      return lpf.next((aCheb.next() * Vol) >> 8);
    case 7:
      return lpf.next((aNos.next() * Vol) >> 8);
  }
}

void loop() {   //这里实时音频处理 尽量不要额外的事情
  audioHook();  //Mozzi required here
}
