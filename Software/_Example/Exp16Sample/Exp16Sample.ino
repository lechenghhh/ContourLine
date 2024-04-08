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

// for scheduling samples to play
EventDelay kTriggerDelay;
EventDelay kTriggerSlowDelay;
ReverbTank reverb;

byte ms_per_note = 111;  // subject to CONTROL_RATE

const byte NUM_PLAYERS = 3;  // 3 seems to be enough
const byte NUM_TABLES = 11;

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
  BAMBOO_10_2048_DATA
};

// gains for each sample player
byte gains[NUM_PLAYERS];

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  startMozzi();
}

void updateControl() {
  for (byte i = 2; i < 9; i++)  //display  //简易参数展示
    digitalWrite(i, HIGH);
  digitalWrite(1 + 2, LOW);
  Serial.println("1111");              //func param Log
  Serial.println(mozziAnalogRead(4));  //func param Log

  ms_per_note = mozziAnalogRead(4);
  kTriggerDelay.set(ms_per_note);          // countdown ms, within resolution of CONTROL_RATE倒计时ms，在CONTROL_RATE的分辨率范围内
  kTriggerSlowDelay.set(ms_per_note * 6);  // resolution-dependent inaccuracy leads to polyrhythm :)分辨率相关的不准确性导致多节律
  for (int i = 0; i < NUM_PLAYERS; i++) {  // play at the speed they're sampled at
    (aSample[i]).setFreq(32);
    // (aSample[i]).setFreq((mozziAnalogRead(4) >> 4) + 6);
    // (aSample[i]).setFreq((float) BAMBOO_00_2048_SAMPLERATE / (float) BAMBOO_00_2048_NUM_CELLS);
  }
  static byte player = 0;

  if (kTriggerDelay.ready()) {
    gains[player] = rand((byte)80, (byte)255);
    (aSample[player]).setTable(tables[rand(NUM_TABLES)]);
    (aSample[player]).start();
    player++;
    if (player == NUM_PLAYERS) player = 0;
    kTriggerDelay.start();
  }

  if (kTriggerSlowDelay.ready()) {
    gains[player] = rand((byte)80, (byte)255);
    (aSample[player]).setTable(tables[rand(NUM_TABLES)]);
    (aSample[player]).start();
    player++;
    if (player == NUM_PLAYERS) player = 0;
    kTriggerSlowDelay.start();
  }
}

AudioOutput_t updateAudio() {
  long asig = 0;
  for (byte i = 0; i < NUM_PLAYERS; i++) {
    asig += (int)(aSample[i]).next() * gains[i];
  }
  int arev = reverb.next(asig);

  //clip any stray peaks to max output range
  return MonoOutput::fromAlmostNBit(15, arev).clip();
}

void loop() {
  audioHook();
}