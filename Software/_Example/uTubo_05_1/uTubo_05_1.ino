// uTubo MK_I -> "Ao alcance de Todos", Serviço Educativo da Casa da Musica
// Tiago Angelo (March 2013)
//STATUS: adding mapping design

#include <MozziGuts.h>                  // mozzi lib
#include <Oscil.h>                      // Oscil class to generate inpupt pulse
#include <tables/PINKNOISE8192_int8.h>  // pink pinkNoise wavetable
#include <Ead.h>                        // Exponential attack-decay class
#include <EventDelay.h>                 // Event delay class
#include <AudioDelay.h>                 // Audio delay class
#include <FixMath.h>                    // Fixed math

#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

//#include <mozzi_midi.h>// Midi utility (mtof conversion)

#define CONTROL_RATE 128  // or some other power of 2 (min=64) (max for this sketch=128)
#define MAX_DELAY 512     // maximum delay size
#define MIN_DELAY 125     // minimum delay size

#define RAMP 8        // envelope (attack = decay) in ms
#define METRO_MIN 80  // envelope duration in ms


#define ZERO 0
#define SENSORVAL 1023
#define REST_THRESH 6  // resting threshold for flex sensors

#define BUTTON_PIN 11

// IMPULSE VARIABLES
Oscil<PINKNOISE8192_NUM_CELLS, AUDIO_RATE> pinkNoise(PINKNOISE8192_DATA);  // pink noise oscillator
Ead impulseEnvelope(CONTROL_RATE);                                         // envelope
EventDelay impulseDuration;                                                // duration

// DELAY VARIABLES
AudioDelay<MAX_DELAY> leftDelay;
Q0n31 leftDelaySize = 256;
AudioDelay<MAX_DELAY> rightDelay;
Q0n31 rightDelaySize = 256;

// SENSOR VARIABLES
Q0n31 memb, membrane;
uchar leftFlex, rightFlex;
uchar restLFlex, restRFlex;
uchar button;
uchar prev_button;
boolean restTrigger;  // resting state trigger

//SYNTH VARIABLES
boolean metro = true;
Q0n31 metroTime = 2000;
//SIGNAL VARIABLES
char impulse = 0, impulseGain, leftSig = 0, rightSig = 0;

byte POSITION = 0;
#define FUNC_LENGTH 5
char function[FUNC_LENGTH][6] = { "L", "R", "M", "P", "5" };
int param[FUNC_LENGTH] = { 1, 2, 3, 4, 5 };
bool* ledGroup[FUNC_LENGTH] = { Led_L, Led_R, Led_M, Led_P, Led_5 };

///////////////////////////////////////////////INIT////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(4, 50, 13, 12, 1);
  initLED(2, 3, 4, 5, 6, 7, 8);  //初始化Led引脚

  pinMode(BUTTON_PIN, INPUT);

  startMozzi(CONTROL_RATE);

  pinkNoise.setFreq((float)8);
  // pinkNoise.setFreq((float)AUDIO_RATE/PINKNOISE8192_NUM_CELLS); // set pinkNoise osc freq (curr = 2)
  impulseDuration.start(20);

  leftFlex = param[0] - 200;   //在setup（）中设置flex传感器的初始值，这样乐器在通电后就不会开始演奏
  rightFlex = param[1] - 200;  //set flex sensors initial values at setup() so that the instrument doesn't start playing after being powered on
  restLFlex = leftFlex;
  restRFlex = rightFlex;
}

///////////////////////////////////////////////CONTROL/////////////////////////////////////////////////
void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  Serial.print(POSITION);           //func param
  Serial.println("func");           //func param
  Serial.println(param[POSITION]);  //func param
  Serial.println(" ");

  readSensors();
  mapping();     // map sensor to synth params
  setImpulse();  // impulse source
}

///////////////////////////////////////////////AUDIO///////////////////////////////////////////////////
int updateAudio() {
  // IMPULSE
  impulse = (pinkNoise.next() * impulseGain);

  // DELAY BLOCK W/ FEEDBACK
  leftSig = leftDelay.next(impulse + leftSig, leftDelaySize);
  rightSig = rightDelay.next(impulse + rightSig, rightDelaySize);

  //OUTPUT
  return leftSig + rightSig;
}

//////////////////////////////////////////OTHER_FUNCTIONS//////////////////////////////////////////////
void readSensors() {  //READ SENSOR DATA

  // memb = mozziAnalogRead(A2);
  memb = param[2];
  memb = sqrt(memb / 1024.f) * 1024;

  //if(memb>0){ membrane = (SENSORVAL - memb)>>1; } // 1~512 (this should be half of MAX_DELAY) : light calculation
  if (memb > 0) { membrane = (SENSORVAL - memb) / 2.f; }  // 1~512 : heavy calculation
  // NOTE: heavy calculations should be replace by Mozzi's fixedMath methods

  leftFlex = param[0] - 200;
  rightFlex = param[1] - 200;
  // leftFlex = mozziAnalogRead(A0) - 200;
  // rightFlex = mozziAnalogRead(A1) - 200;

  prev_button = button;
  button = digitalRead(BUTTON_PIN);
  //button = digitalRead
  if (button != prev_button) {
    restTrigger = true;
  } else {
    restTrigger = false;
  }

  // SET RESTING STATE
  if (restTrigger) {
    restLFlex = leftFlex;
    restRFlex = rightFlex;
    impulseDuration.start(ZERO);  //restart metro
  }
}

void mapping() {  //SENSOR->SYNTH MAPPING

  // SET METRO
  if (leftFlex < restLFlex + REST_THRESH && leftFlex > restLFlex - REST_THRESH
      && rightFlex < restRFlex + REST_THRESH && rightFlex > restRFlex - REST_THRESH) {
    metro = false;
  } else {
    metro = true;
  }

  // SET METRO TIME
  //metroTime = (((leftFlex * rightFlex)*(membrane/128))>>7) + METRO_MIN; // : light calculation
  metroTime = (((leftFlex * rightFlex) * (membrane / 128.f)) / 128.f) + METRO_MIN;  // : heavy calculation (enables really continuous time changes)
  // NOTE: heavy calculations should be replace by Mozzi's fixedMath methods

  // SET DELAY TIME (wich corresponds to heard pitch)
  leftDelaySize = membrane + leftFlex + MIN_DELAY;
  rightDelaySize = membrane + rightFlex + MIN_DELAY;
}

void setImpulse() {  // SOURCE IMPULSE

  // pinkNoise.setPhase(mozziAnalogRead(A3) << 3);  // cheap way of avoiding a noticeable loop (0~8192 noise) 避免明显环路的廉价方法
  pinkNoise.setPhase(param[3] << 3);  // cheap way of avoiding a noticeable loop (0~8192 noise)

  //IMPULSE ENVELOPE
  if (impulseDuration.ready() == true && metro == true) {
    impulseEnvelope.start(RAMP, RAMP);  // impulse attack-decay values
    impulseDuration.start(metroTime);   // impulse duration
  }
  impulseGain = impulseEnvelope.next();
}

///////////////////////////////////////////////LOOP////////////////////////////////////////////////////
void loop() {
  audioHook();  // fills the audio buffer
}
