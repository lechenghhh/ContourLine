//https://github.com/Testbild-synth/HAGIWO_arduino_oscillator //项目地址
#include <Mozzi.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/phasor256_int8.h>
#include <FixMath.h>

#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

#define CONTROL_RATE 128
#define V_OCT_PIN A0
#define P1_CV_PIN A1
#define P2_CV_PIN A2
// #define GAIN_CV_PIN A6
#define MODE_CV_PIN A3

#define FUNC_LENGTH 9  // menu length

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc1(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc2(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc3(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc4(SIN2048_DATA);

byte POSITION = 0;
String function[FUNC_LENGTH] = { "Root", "RAmp", "Note2", "N2Amp", "Note3", "N3Amp", "Note4", "N4Amp", "WaveT" };
int param[FUNC_LENGTH] = { 0, 1023, 0, 128, 0, 128, 0, 128, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_0, Led_1, Led_2, Led_3, Led_4, Led_5, Led_6, Led_7, Led_T };
int tmp_d11 = 0;

int gain1 = 255;  //0-255
int gain2 = 255;
int gain3 = 255;
int gain4 = 255;
int gain5 = 255;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
}

void updateControl() {
  //control & display
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  //log
  Serial.print(" func");          //func param
  Serial.print(POSITION);         //func param
  Serial.print("=");              //func param
  Serial.print(param[POSITION]);  //func param
  // Serial.print(" n1=");           //func param
  // Serial.print(param[0] >> 5);    //func param
  // Serial.print(" n2=");           //func param
  // Serial.print(param[2] >> 5);    //func param
  // Serial.print(" n3=");           //func param
  // Serial.print(param[4] >> 5);    //func param
  // Serial.print(" n4=");           //func param
  // Serial.print(param[6] >> 5);    //func param
  // Serial.print(" gain1=");        //func param
  // Serial.print(gain1);            //func param
  // Serial.print(" gain2=");        //func param
  // Serial.print(gain2);            //func param
  // Serial.print(" gain3=");        //func param
  // Serial.print(gain3);            //func param
  // Serial.print(" gain4=");        //func param
  // Serial.print(gain4);            //func param
  Serial.println("");  //func param

  //setwaves
  switch (param[8] >> 6) {
    case 0:  //sin
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(SIN2048_DATA);
      osc3.setTable(SIN2048_DATA);
      osc4.setTable(SIN2048_DATA);
      break;
    case 1:  //tri hermes
      osc1.setTable(TRIANGLE_HERMES_2048_DATA);
      osc2.setTable(TRIANGLE_HERMES_2048_DATA);
      osc3.setTable(TRIANGLE_HERMES_2048_DATA);
      osc4.setTable(TRIANGLE_HERMES_2048_DATA);
      break;
    case 2:  //tri dist
      osc1.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc2.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc3.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc4.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      break;
    case 3:  //saw
      osc1.setTable(SAW2048_DATA);
      osc2.setTable(SAW2048_DATA);
      osc3.setTable(SAW2048_DATA);
      osc4.setTable(SAW2048_DATA);
      break;
    case 4:  //square
      osc1.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc2.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc3.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc4.setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
    case 5:  //square
      osc1.setTable(SQUARE_ANALOGUE512_DATA);
      osc2.setTable(SQUARE_ANALOGUE512_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 6:  //square
      osc1.setTable(PHASOR256_DATA);
      osc2.setTable(PHASOR256_DATA);
      osc3.setTable(PHASOR256_DATA);
      osc4.setTable(PHASOR256_DATA);
      break;
    case 7:  //square
      osc1.setTable(HALFSIN256_DATA);
      osc2.setTable(HALFSIN256_DATA);
      osc3.setTable(HALFSIN256_DATA);
      osc4.setTable(HALFSIN256_DATA);
      break;
    case 8:  //sin
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(TRIANGLE_HERMES_2048_DATA);
      osc3.setTable(TRIANGLE_HERMES_2048_DATA);
      osc4.setTable(SAW2048_DATA);
      break;
    case 9:  //tri hermes
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc3.setTable(SAW2048_DATA);
      osc4.setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
    case 10:  //tri dist
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc3.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc4.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      break;
    case 11:  //saw
      osc1.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc2.setTable(SAW2048_DATA);
      osc3.setTable(PHASOR256_DATA);
      osc4.setTable(HALFSIN256_DATA);
      break;
    case 12:  //square
      osc1.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc2.setTable(SAW2048_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
    case 13:  //square
      osc1.setTable(SQUARE_ANALOGUE512_DATA);
      osc2.setTable(PHASOR256_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(PHASOR256_DATA);
      break;
    case 14:  //square
      osc1.setTable(SAW2048_DATA);
      osc2.setTable(HALFSIN256_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(PHASOR256_DATA);
      break;
    case 15:  //square
      osc1.setTable(SAW2048_DATA);
      osc2.setTable(SQUARE_ANALOGUE512_DATA);
      osc3.setTable(PHASOR256_DATA);
      osc4.setTable(HALFSIN256_DATA);
      break;
  }

  //random waves
  if (POSITION != 9) {
    //EXT_ADV_IN adv输入判断 出现上升沿
    if (digitalRead(11) == 1 && tmp_d11 == 0) {
      tmp_d11 = 1;
      param[8] = random(0, 1023);
    }
    if (digitalRead(11) == 0 && tmp_d11 == 1) {
      tmp_d11 = 0;
    }
  }

  //set freq
  int note1 = 0;
  int note2 = (param[2] >> 5) * 17;
  int note3 = (param[4] >> 5) * 17;
  int note4 = (param[6] >> 5) * 17;

  uint16_t oct_cv_val = mozziAnalogRead(V_OCT_PIN);
  int freq1 = param[0] >> 2;
  int freqv1 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note1]))));  //ROOT
  int freqv2 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note2]))));  //2nd
  int freqv3 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note3]))));  //3rd
  int freqv4 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note4]))));  //4th

  // Serial.print(" freqv1=");  //func param
  // Serial.print(freqv1);      //func param
  // Serial.print(" freqv2=");  //func param
  // Serial.print(freqv2);      //func param

  osc1.setFreq(freqv1);  // set the frequency
  osc2.setFreq(freqv2);
  osc3.setFreq(freqv3);
  osc4.setFreq(freqv4);

  //set gain
  gain1 = 32;
  gain2 = (param[3] + mozziAnalogRead(1)) >> 5;
  gain3 = (param[5] + mozziAnalogRead(2)) >> 5;
  gain4 = (param[7] + mozziAnalogRead(3)) >> 5;
  if (gain1 > 32) gain1 = 32;
  if (gain2 > 32) gain2 = 32;
  if (gain3 > 32) gain3 = 32;
  if (gain4 > 32) gain4 = 32;
}

AudioOutput_t updateAudio() {
  int value1 = (osc1.next()) * gain1;  // 1/8
  int value2 = (osc2.next()) * gain2;
  int value3 = (osc3.next()) * gain3;
  int value4 = (osc4.next()) * gain4;

  return MonoOutput::fromNBit(16, (value1 + value2 + value3 + value4));
  // return MonoOutput::fromNBit(16, ((osc1.next() / 8 + osc2.next() / 8 + osc3.next() / 8 + osc4.next() / 8 + osc5.next() / 8) << 8));
}

void loop() {
  audioHook();
}
