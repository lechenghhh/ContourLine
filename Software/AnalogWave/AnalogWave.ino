#include <Mozzi.h>
#include <Oscil.h>
// #include <tables/cos2048_int8.h> //
#include <tables/sin512_int8.h>                //sub
#include <tables/triangle_analogue512_int8.h>  //TRIANGLE_ANALOGUE512_DATA
#include <tables/square_analogue512_int8.h>    //SQUARE_ANALOGUE512_DATA
#include <tables/saw_analogue512_int8.h>       //SAW_ANALOGUE512_DATA

#include <FixMath.h>
#include "Module_Ctrl.h"

#include "Module_LEDDisplay.h"
#include "Module_Const.h"
#define CONTROL_RATE 256  // Hz, powers of 2 are most reliable

Oscil<512, AUDIO_RATE> awOsc(TRIANGLE_ANALOGUE512_DATA);
Oscil<512, AUDIO_RATE> awSub(SIN512_DATA);
Oscil<512, AUDIO_RATE> awDetune(SIN512_DATA);

const static int FUNC_LENGTH = 6;
byte POSITION = 0;
String function[FUNC_LENGTH] = { "Pitch", "range", "FMA", "Sub", "Detun", "WaveT" };
int param[FUNC_LENGTH] = { 0, 240, 0, 0, 0, 384 };
bool* ledGroup[FUNC_LENGTH] = { Led_P, Led_R, Led_F, Led_S, Led_D, Led_T };
byte tmp_d11 = 0;
int voct = 500;
byte subGain = 256;
byte range = 1;
byte WaveType = 0;
Q16n16 FMA;
Q16n16 toneFreq, FMod, pitch;

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi();
  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚
  pinMode(11, INPUT_PULLUP);
}

//三个旋钮 Carrier A0  ModFreq A1  ModLV A3    C
void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用以展示控制

  Serial.println(POSITION + function[POSITION] + param[POSITION]);  //func param Log

  //VOCT A7  CV-Freq A4  CV-LV A5
  voct = mozziAnalogRead(0);  //由于cltest的voct接口阻抗问题 这里需要乘以一个系数 调谐才比较准确
  pitch = param[0];
  toneFreq = (2143658 + pitch * 5200) * pow(2, (pgm_read_float(&(voctpow[voct]))));  // V/oct apply
  range = param[1] >> 6;
  int rangePitch = (range + 1) / 2;

  Q16n16 FMA = mozziAnalogRead(1) * 60000 * param[2] / 1024;  //调频乘数

  awDetune.setFreq(0.17f);
  int tmpDetune = awDetune.next() * param[4] * 1000;

  awOsc.setFreq_Q16n16((toneFreq + FMA + tmpDetune) * rangePitch);
  awSub.setFreq_Q16n16(toneFreq * rangePitch >> 2);

  int tmpsub = param[3] + mozziAnalogRead(2);
  if (tmpsub > 1023) tmpsub = 1023;
  subGain = tmpsub / 8;


  WaveType = param[5] / 256;
  switch (WaveType) {
    default:
      awOsc.setTable(SIN512_DATA);
      break;
    case 1:
      awOsc.setTable(TRIANGLE_ANALOGUE512_DATA);
      break;
    case 2:  //2345
      awOsc.setTable(SQUARE_ANALOGUE512_DATA);
      break;
    case 3:
      awOsc.setTable(SAW_ANALOGUE512_DATA);
      break;
  }

  // Serial.print("-subGain-");
  // Serial.print(subGain);
  // Serial.print("-WaveType-");
  // Serial.print(WaveType);
  // Serial.println(FMA);
}

AudioOutput_t updateAudio() {
  // return MonoOutput::from8Bit(awOsc.phMod(FMA * awSub.next() >> 8));  //Old:内部仍然只有8位，在HIFI模式下将移位至14位
  // return MonoOutput::fromNBit(16, awOsc.phMod(FMA * awSub.next() >> 8) << 8);  //new:hifi
  return MonoOutput::fromNBit(16, (awOsc.next() << 7) + (awSub.next() * subGain));  //new:hifi
}

void loop() {
  audioHook();
}