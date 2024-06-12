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

#define FUNC_LENGTH 11  // menu length

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc1(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc2(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc3(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc4(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc5(SIN2048_DATA);

byte POSITION = 0;
String function[FUNC_LENGTH] = { "Root", "RAmp", "Note2", "N2Amp", "Note3", "N3Amp", "Note4", "N4Amp", "Note5", "N5Amp", "WaveT" };
int param[FUNC_LENGTH] = { 0, 1024, 0, 128, 0, 128, 0, 128, 0, 128, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5, Led_6, Led_7, Led_8, Led_9, Led_0, Led_T };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  initCtrl(4, 50, 13, 12, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
}

void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  Serial.print(" func");          //func param
  Serial.print(POSITION);         //func param
  Serial.print("=");              //func param
  Serial.print(param[POSITION]);  //func param
  Serial.print(" n1=");           //func param
  Serial.print(param[0] >> 5);    //func param
  Serial.print(" n2=");           //func param
  Serial.print(param[2] >> 5);    //func param
  Serial.print(" n3=");           //func param
  Serial.print(param[4] >> 5);    //func param
  Serial.print(" n4=");           //func param
  Serial.print(param[6] >> 5);    //func param
  Serial.print(" n5=");           //func param
  Serial.print(param[8] >> 5);    //func param
  Serial.println("");             //func param

  switch (param[10] >> 7) {
    default:  //sin
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(SIN2048_DATA);
      osc3.setTable(SIN2048_DATA);
      osc4.setTable(SIN2048_DATA);
      osc5.setTable(SIN2048_DATA);
      break;
    case 1:  //tri hermes
      osc1.setTable(TRIANGLE_HERMES_2048_DATA);
      osc2.setTable(TRIANGLE_HERMES_2048_DATA);
      osc3.setTable(TRIANGLE_HERMES_2048_DATA);
      osc4.setTable(TRIANGLE_HERMES_2048_DATA);
      osc5.setTable(TRIANGLE_HERMES_2048_DATA);
      break;
    case 2:  //tri dist
      osc1.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc2.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc3.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc4.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc5.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      break;
    case 3:  //saw
      osc1.setTable(SAW2048_DATA);
      osc2.setTable(SAW2048_DATA);
      osc3.setTable(SAW2048_DATA);
      osc4.setTable(SAW2048_DATA);
      osc5.setTable(SAW2048_DATA);
      break;
    case 4:  //square
      osc1.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc2.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc3.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc4.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc5.setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
    case 5:  //square
      osc1.setTable(SQUARE_ANALOGUE512_DATA);
      osc2.setTable(SQUARE_ANALOGUE512_DATA);
      osc3.setTable(SQUARE_ANALOGUE512_DATA);
      osc4.setTable(SQUARE_ANALOGUE512_DATA);
      osc5.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 6:  //square
      osc1.setTable(PHASOR256_DATA);
      osc2.setTable(PHASOR256_DATA);
      osc3.setTable(PHASOR256_DATA);
      osc4.setTable(PHASOR256_DATA);
      osc5.setTable(PHASOR256_DATA);
      break;
    case 7:  //square
      osc1.setTable(HALFSIN256_DATA);
      osc2.setTable(HALFSIN256_DATA);
      osc3.setTable(HALFSIN256_DATA);
      osc4.setTable(HALFSIN256_DATA);
      osc5.setTable(HALFSIN256_DATA);
      break;
  }
  byte note1 = (pgm_read_byte((param[0] >> 5) * 17));
  byte note2 = (pgm_read_byte((param[2] >> 5) * 17));
  byte note3 = (pgm_read_byte((param[4] >> 5) * 17));
  byte note4 = (pgm_read_byte((param[6] >> 5) * 17));
  byte note5 = (pgm_read_byte((param[8] >> 5) * 17));

  uint16_t oct_cv_val = mozziAnalogRead(V_OCT_PIN);
  int freq1 = param[0] >> 2;
  int freqv1 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note1]))));  //ROOT
  int freqv2 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note2]))));  //2nd
  int freqv3 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note3]))));  //3rd
  int freqv4 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note4]))));  //4th
  int freqv5 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note5]))));  //5th
  osc1.setFreq(freqv1);                                                           // set the frequency
  osc2.setFreq(freqv2);
  osc3.setFreq(freqv3);
  osc4.setFreq(freqv4);
  osc5.setFreq(freqv5);
}

AudioOutput_t updateAudio() {
  int gain1 = param[1] >> 2;
  int gain2 = param[1] >> 2;
  int gain3 = param[1] >> 2;
  int gain4 = param[1] >> 2;
  int gain5 = param[1] >> 2;
  return MonoOutput::fromNBit(16, ((osc1.next() / 8 + osc2.next() / 8 + osc3.next() / 8 + osc4.next() / 8 + osc5.next() / 8) << 8));
}

void loop() {
  audioHook();
}
