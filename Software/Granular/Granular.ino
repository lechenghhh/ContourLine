/*  Example of a polyphonic sketch in which
    11 sound tables are shared between 3 voices,
    using Mozzi sonification library.

    Demonstrates use of Sample() objects as players,
    using setTable() to share many sound tables between
    a few players, to minimise processing in updateAudio().
    Shows how to use Samples and sound tables in arrays,
    EventDelay() for scheduling, and rand() to select
    sound tables and vary the gain of each player.

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.io/Mozzi/

		Mozzi documentation/API
		https://sensorium.github.io/Mozzi/doc/html/index.html

		Mozzi help/discussion/announcements:
    https://groups.google.com/forum/#!forum/mozzi-users

    Tim Barrass 2012, CC by-nc-sa.
*/

#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <mozzi_rand.h>                          // for rand()
#include <samples/bamboo/bamboo_00_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_01_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_02_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_03_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_04_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_05_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_06_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_07_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_08_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_09_2048_int8.h>  // wavetable data
#include <samples/bamboo/bamboo_10_2048_int8.h>  // wavetable data

//lecheng的控制/显示模块封装
// #include "Module_LEDDisplay.h"
// #include "Module_Const.h"
#include "Module_Ctrl.h"
#define CONTROL_RATE 256  // Hz, powers of 2 are most reliable

EventDelay kTriggerDelay;  // for scheduling samples to play

EventDelay kTriggerSlowDelay;

const byte NUM_PLAYERS = 3;  // 3 seems to be enough
const byte NUM_TABLES = 11;

/*   引脚定义   */
#define KONB_PIN 4   //
#define VOCT_PIN 0   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define GATE_PIN 11  //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //

Sample<BAMBOO_00_2048_NUM_CELLS, AUDIO_RATE> aSample[NUM_PLAYERS] = {
  Sample<BAMBOO_00_2048_NUM_CELLS, AUDIO_RATE>(BAMBOO_00_2048_DATA),
  Sample<BAMBOO_01_2048_NUM_CELLS, AUDIO_RATE>(BAMBOO_01_2048_DATA),
  Sample<BAMBOO_02_2048_NUM_CELLS, AUDIO_RATE>(BAMBOO_02_2048_DATA),
};

const int8_t* tables[NUM_TABLES] = {  // watch out - tables are const (but you can choose which ones you reference)
  BAMBOO_00_2048_DATA,
  BAMBOO_01_2048_DATA,
  BAMBOO_02_2048_DATA,
  BAMBOO_03_2048_DATA,
  BAMBOO_04_2048_DATA,
  BAMBOO_05_2048_DATA,
  BAMBOO_06_2048_DATA,
  BAMBOO_07_2048_DATA,
  BAMBOO_08_2048_DATA,
  BAMBOO_09_2048_DATA,
  BAMBOO_06_2048_DATA
};
byte gains[NUM_PLAYERS];  // gains for each sample player

#define FUNC_LENGTH 5  //功能列表长度
int POSITION = 0;
String function[FUNC_LENGTH] = { "freq", "dens", "diffu", "fb", "level" };
int param[FUNC_LENGTH] = { 512, 512, 0, 1, 128 };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  initCtrl(KONB_PIN, 16, BTN2_PIN, BTN1_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2

  startMozzi();
}

void updateControl() {

  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //并获取修改成功的旋钮值
  digitalWrite(POSITION + 2, LOW);                 //简易参数显示
  digitalWrite(POSITION + 1, HIGH);                //减少调用次数
  digitalWrite(POSITION + 3, HIGH);                //优化性能

  // Serial.println(POSITION + function[POSITION] + param[POSITION]);  //func param Log

  Serial.print("Func ");            //func param Log
  Serial.println(POSITION);         //func param Log
  Serial.println(param[POSITION]);  //func param Log

  uint8_t freq = (param[0] >> 4) + 6;        //采样频率
  uint8_t density = 1023 - param[1];         //密度 数值越大密度越大
  byte diffusion = param[2];                 //扩散 密度不均匀程度
  density = density + rand(diffusion >> 2);  //密度
  byte feedback = 7 - (param[3] >> 7);       //重复的粒子密度
  uint8_t level = param[4];                  //粒子音量均匀程度
  // byte delaylevel = param[4] >> 2;
  kTriggerDelay.set(density);                 // countdown ms, within resolution of CONTROL_RATE倒计时ms，在CONTROL_RATE的分辨率范围内
  kTriggerSlowDelay.set(density * feedback);  // resolution-dependent inaccuracy leads to polyrhythm :)分辨率相关的不准确性导致多节律
  for (int i = 0; i < NUM_PLAYERS; i++) {     // play at the speed they're sampled at
    aSample[i].setFreq(freq);
    // (aSample[i]).setFreq((float) BAMBOO_00_2048_SAMPLERATE / (float) BAMBOO_00_2048_NUM_CELLS);
  }

  static byte player = 0;
  if (kTriggerDelay.ready()) {
    gains[player] = rand((byte)(level >> 4), (byte)255);
    (aSample[player]).setTable(tables[rand(NUM_TABLES)]);
    (aSample[player]).start();
    player++;
    if (player == NUM_PLAYERS) player = 0;
    kTriggerDelay.start();
  }

  if (kTriggerSlowDelay.ready()) {
    gains[player] = rand((byte)(level >> 8), (byte)level);
    (aSample[player]).setTable(tables[rand(NUM_TABLES)]);
    (aSample[player]).start();
    player++;
    if (player == NUM_PLAYERS) player = 0;
    kTriggerSlowDelay.start();
  }
}

AudioOutput_t updateAudio() {
  long dry = 0;
  for (byte i = 0; i < NUM_PLAYERS; i++) {
    dry += (int)(aSample[i]).next() * gains[i];
  }

  return MonoOutput::fromAlmostNBit(15, dry).clip();  //dry
}

void loop() {
  audioHook();
}
