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
#include <ReverbTank.h>

//lecheng的控制/显示模块封装
// #include "Module_LEDDisplay.h"
#include "Module_Ctrl.h"
#include "Module_Const.h"
// for scheduling samples to play
EventDelay kTriggerDelay;
EventDelay kTriggerSlowDelay;
ReverbTank reverb;

byte ms_per_note = 111;  // subject to CONTROL_RATE

const byte NUM_PLAYERS = 3;  // 3 seems to be enough
const byte NUM_TABLES = 11;
#define FUNC_LENGTH 4  //功能列表长度

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

// watch out - tables are const (but you can choose which ones you reference)
const int8_t* tables[NUM_TABLES] = {
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

// gains for each sample player
byte gains[NUM_PLAYERS];

Q16n16 POSITION = 0;
String function[FUNC_LENGTH] = { "freq", "dens", "seed", "rvb" };
int param[FUNC_LENGTH] = { 64, 100, 320, 0 };
// bool* ledGroup[FUNC_LENGTH] = { Led_F, Led_D, Led_S, Led_R};

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN2_PIN, BTN1_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  // initLED(2, 3, 4, 5, 6, 7, 8);                      //初始化Led引脚

  startMozzi();
}

void updateControl() {

  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  // for (byte i = 2; i < 9; i++)  //display  //简易参数展示
  //   digitalWrite(i, HIGH);
  // digitalWrite(POSITION + 2, LOW);
  // Serial.println(POSITION + function[POSITION] + param[POSITION]);  //func param Log
  Serial.print("POSITION");         //func param Log
  Serial.println(POSITION);         //func param Log
  Serial.println(param[POSITION]);  //func param Log

  ms_per_note = param[1];
  kTriggerDelay.set(ms_per_note);          // countdown ms, within resolution of CONTROL_RATE倒计时ms，在CONTROL_RATE的分辨率范围内
  kTriggerSlowDelay.set(ms_per_note * 6);  // resolution-dependent inaccuracy leads to polyrhythm :)分辨率相关的不准确性导致多节律
  for (int i = 0; i < NUM_PLAYERS; i++) {  // play at the speed they're sampled at
    // (aSample[i]).setFreq(32);
    // (aSample[i]).setFreq((mozziAnalogRead(4) >> 4) + 6);
    (aSample[i]).setFreq((param[0] >> 4) + 6);
    // (aSample[i]).setFreq((float) BAMBOO_00_2048_SAMPLERATE / (float) BAMBOO_00_2048_NUM_CELLS);
  }
  static byte player = 0;

  if (kTriggerDelay.ready()) {
    gains[player] = rand((byte)(param[2] >> 2), (byte)255);
    (aSample[player]).setTable(tables[rand(NUM_TABLES)]);
    (aSample[player]).start();
    player++;
    if (player == NUM_PLAYERS) player = 0;
    kTriggerDelay.start();
  }

  if (kTriggerSlowDelay.ready()) {
    gains[player] = rand((byte)(param[2] >> 8), (byte)255);
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
  int wet = reverb.next(dry);

  int dryGain = (1023 - param[3]) >> 2;
  int wetGain = param[3] >> 2;
  int dry2 = dryGain * dry >> 8;
  int wet2 = wetGain * wet >> 8;
  // return MonoOutput::fromAlmostNBit(15, dry).clip();  //dry
  return MonoOutput::fromAlmostNBit(15, wet).clip();  //wet
  // return MonoOutput::fromAlmostNBit(15, (dry2 + wet2) >> 1).clip();  //wet
}

void loop() {
  audioHook();
}
