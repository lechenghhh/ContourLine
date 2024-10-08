#include <Mozzi.h>
#include <Sample.h>  // Sample template
#include <EventDelay.h>
// #include <samples/bamboo/bamboo_02_4096_int8.h>  // Sample data
// #include <samples/bamboo/bamboo_03_4096_int8.h>  // Sample data
// #include <samples/bamboo/bamboo_04_4096_int8.h>  // Sample data
#include "sample1.h"
#include "sample2.h"
#include "sample3.h"
#include "sample4.h"

/*lecheng的控制/显示模块封装*/
#include "Module_LEDDisplay.h"
#include "Module_Ctrl.h"
#include "Module_Const.h"
/*   引脚定义   */
#define KONB_PIN 4   //
#define VOCT_PIN 0   //
#define CV1_PIN 1    //
#define CV2_PIN 2    //
#define CV3_PIN 3    //
#define GATE_PIN 11  //
#define BTN1_PIN 12  //
#define BTN2_PIN 13  //
#define MOZZI_CONTROL_RATE 128

const float playspeed = 1.3;
const int SAMPLE_MAX_LENGTH = 4096;
float playspeedmod = 0;
float speedchange = 0;
unsigned int startPosition;
unsigned int endPosition;
unsigned int length;
unsigned int circle;
unsigned int bit;
unsigned int d11_Trig;
const unsigned int full = (int)(1000.f / playspeed) - 23;  // adjustment approx for MOZZI_CONTROL_RATE difference

#define PARAM_LENGTH 6  //功能列表长度
int POSITION = 0;
String param_name[PARAM_LENGTH] = { "Freq", "Pos", "Length", "Cycle", "BitCru", "Select" };
int param[PARAM_LENGTH] = { 256, 0, 128, 768, 0, 0 };
bool* ledGroup[PARAM_LENGTH] = { Led_F, Led_P, Led_L, Led_C, Led_B, Led_S };

Sample<SAMPLE_MAX_LENGTH, MOZZI_AUDIO_RATE> aSample(sample1_DATA);
EventDelay kTriggerDelay;

void setup() {
  Serial.begin(115200);                              //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  initCtrl(KONB_PIN, 16, BTN1_PIN, BTN2_PIN, HIGH);  //初始化控制参数// 旋钮 旋钮编辑状态启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);
  startMozzi();

  aSample.setFreq(playspeed * ((float)SAMPLE_MAX_LENGTH / (float)SAMPLE_MAX_LENGTH));
  kTriggerDelay.set(full);
  aSample.setLoopingOn();
}

void updateControl() {
  /*控制参数获取与显示逻辑*/
  POSITION = getPostition(POSITION, PARAM_LENGTH);                   //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);                      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                                   //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);                   //如果处在非编辑状态 led将半灭显示
  Serial.println(POSITION + param_name[POSITION] + param[POSITION]);  //func param Log

  // exampleRand();                                                      //old exp
  //set position set length set freq
  playspeedmod = (float)(param[0] + mozziAnalogRead(VOCT_PIN)) / 256 + 0.01;             //播放速度
  aSample.setFreq(playspeedmod);                                                         //设置速度
  startPosition = (param[1] + mozziAnalogRead(CV2_PIN)) * 4;                             //播放头
  if (startPosition > SAMPLE_MAX_LENGTH - 512) startPosition = SAMPLE_MAX_LENGTH - 512;  //播放头位置限制
  aSample.setStart(startPosition);                                                       //设置播放头
  length = (param[2] + mozziAnalogRead(CV3_PIN)) * 4 + 2;                                //长度
  endPosition = startPosition + length;                                                  //播放尾由长度决定
  if (endPosition > SAMPLE_MAX_LENGTH) endPosition = SAMPLE_MAX_LENGTH;                  //播放长度超出限制
  aSample.setEnd(endPosition);                                                           //设置播放尾
                                                                                         //设置播放速度

  //circle (loop)
  circle = param[3] >> 9;
  if (circle == 1) {  //loop mode
    aSample.setLoopingOn();
  } else {  //one shot mode
    aSample.setLoopingOff();
    if (digitalRead(GATE_PIN) != d11_Trig && d11_Trig == 0) {
      d11_Trig = 1;
      aSample.setLoopingOn();  // kTriggerDelay.start();
    }
    if (digitalRead(GATE_PIN) != d11_Trig && d11_Trig == 1) {
      d11_Trig = 0;
      aSample.setLoopingOff();
    }
  }

  //Bit
  bit = 16 - (param[4] >> 7);

  //Sample List Select
  switch (param[5] >> 8) {
    default:
      aSample.setTable(sample1_DATA);
      break;
    case 1:
      aSample.setTable(sample2_DATA);
      break;
    case 2:
      aSample.setTable(sample3_DATA);
      break;
    case 3:
      aSample.setTable(sample4_DATA);
      break;
  }

  // Serial.print(" startPosition");  //func param Log
  // Serial.print(startPosition);     //func param Log
  // Serial.print(" endPosition");    //func param Log
  // Serial.print(endPosition);       //func param Log
  // Serial.print(" playspeedmod");   //func param Log
  // Serial.print(playspeedmod);      //func param Log
  // Serial.print("\n");
}

AudioOutput updateAudio() {
  // return MonoOutput::from8Bit(aSample.next());
  return MonoOutput::fromNBit(bit, aSample.next() << 8);
}

void loop() {
  audioHook();
}

// void exampleRand() {
//   if (kTriggerDelay.ready()) {
//     //chooseStartEnd
//     if (rand((byte)2) == 0) {
//       // s = chooseStart();
//       // e = chooseEnd(s);
//       startPosition = rand((unsigned int)BAMBOO_00_2048_NUM_CELLS);
//       endPosition = rand(startPosition, (unsigned int)BAMBOO_00_2048_NUM_CELLS);
//     } else {
//       startPosition = 0;
//       endPosition = BAMBOO_00_2048_NUM_CELLS;
//     }
//     aSample.setStart(startPosition);
//     aSample.setEnd(endPosition);
//     aSample.setFreq(playspeedmod);

//     // chooseSpeedMod();
//     if (rand((byte)3) == 0) {
//       speedchange = (float)rand((char)-100, (char)100) / 8000;
//       float startspeed = (float)rand((char)-100, (char)100) / 100;
//       playspeedmod = playspeed + startspeed;
//     } else {
//       speedchange = 0;
//       playspeedmod = playspeed;
//     }

//     kTriggerDelay.start();
//   }
//   playspeedmod += speedchange;
//   aSample.setFreq(playspeedmod);
// }