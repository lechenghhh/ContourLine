//https://github.com/Testbild-synth/HAGIWO_arduino_oscillator //项目地址
#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/phasor256_int8.h>

#include <mozzi_fixmath.h>
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

int btnHover3 = 0;  //按下事件判断
int btnTime3 = 0;   //长按事件判断

uint16_t freq_pot_val = 0;  //频率旋钮
uint16_t oct_cv_val = 0;    //voct cv
uint16_t p1_pot_val = 0;
uint16_t p1_cv_val = 0;
uint16_t p2_pot_val = 0;
uint16_t p2_cv_val = 0;
int gain_cv_val = 0;
uint8_t mode = 0;
uint8_t mode_val = 0;

////chord variables
int freq1 = 110;  //base freq

int freqv1 = 440;  //apply voct
int freqv2 = 440;
int freqv3 = 440;
int freqv4 = 440;
int freqv5 = 440;

byte note1 = 0;  //Root
byte note2 = 0;  //2nd
byte note3 = 0;  //3rd
byte note4 = 0;  //4th
byte note5 = 0;  //Root

byte inv_aply1 = 0;  //0 = no inv , 1 = inv , Root 转位和弦设置 八度参数
byte inv_aply2 = 0;  //2nd
byte inv_aply3 = 0;  //3rd
byte inv_aply4 = 0;  //4th
bool inv_aply5 = 0;  //0 = no output root sound , 1 = output root sound

int inv = 0;
int inv_knob = 0;
int chord = 0;
byte wave = 0;

byte POSITION = 0;
String function[FUNC_LENGTH] = { "Root", "RAmp", "Note2", "N2Amp", "Note3", "N3Amp", "Note4", "N4Amp", "Note5", "N5Amp", "WaveT" };
int param[FUNC_LENGTH] = { 0, 1024, 0, 128, 0, 128, 0, 128, 0, 128, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5, Led_6, Led_7, Led_8, Led_9, Led_0, Led_T };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);

  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
}

void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  // Serial.print(" mode=");           //mode 0: FM mode 1: add, mode 2: chord
  // Serial.print(mode);               //mode
  Serial.print(" func");            //func param
  Serial.print(POSITION);           //func param
  Serial.print("=");                //func param
  Serial.println(param[POSITION]);  //func param

  freq_pot_val = param[0];
  oct_cv_val = mozziAnalogRead(V_OCT_PIN);
  p1_pot_val = param[1];
  p1_cv_val = mozziAnalogRead(P1_CV_PIN);
  p2_pot_val = param[2];
  p2_cv_val = mozziAnalogRead(P2_CV_PIN);
  gain_cv_val = 255;
  mode_val = mozziAnalogRead(MODE_CV_PIN) / 100;

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
  CHORD_setFreqs();
}

void CHORD_setFreqs() {
  //chord模式选择
  chord = constrain((p2_pot_val / 128) + (p2_cv_val / 128), 0, 7);
  //inversion setting 转位和弦设置
  inv = constrain((p1_pot_val / 64) + (p1_cv_val / 64), 0, 15);

  //setting chord note
  // note1 = (pgm_read_byte(&(chord_table[chord][0])));
  // note2 = (pgm_read_byte(&(chord_table[chord][1])));
  // note3 = (pgm_read_byte(&(chord_table[chord][2])));
  // note4 = (pgm_read_byte(&(chord_table[chord][3])));
  // note5 = (pgm_read_byte(&(chord_table[chord][0])));
  note1 = (pgm_read_byte((param[0] >> 5) * 17));
  note2 = (pgm_read_byte((param[2] >> 5) * 17));
  note3 = (pgm_read_byte((param[4] >> 5) * 17));
  note4 = (pgm_read_byte((param[6] >> 5) * 17));
  note5 = (pgm_read_byte((param[8] >> 5) * 17));

  freq1 = freq_pot_val / 4;

  freqv1 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note1]))));  //ROOT
  freqv2 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note2]))));  //2nd
  freqv3 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note3]))));  //3rd
  freqv4 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note4]))));  //4th
  freqv5 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note5]))));  //5th
  osc1.setFreq(freqv1);                                                       // set the frequency
  osc2.setFreq(freqv2);
  osc3.setFreq(freqv3);
  osc4.setFreq(freqv4);
  osc5.setFreq(freqv5);
}

AudioOutput_t updateAudio() {
  return MonoOutput::fromNBit(16, ((osc1.next() / 8 + osc2.next() / 8 + osc3.next() / 8 + osc4.next() / 8 + osc5.next() / 8 * inv_aply5) * gain_cv_val));
}

void loop() {
  audioHook();
}
