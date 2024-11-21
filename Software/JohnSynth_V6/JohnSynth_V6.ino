/* CHANGES
 - adding fine-grained control over both synth blocks: all frequencies and all waveform selects have their own control knob
 - this requires adding MUX control lines and changing how frequencies and waveforms are set
 - adding mode control over each synth block: requires changes to I/O and changes to updateAudio
 - shorteniing wavetable sizes and adding more of them now that each oscillator's waveform can be easily and finely controlled
 
 */

#include <MozziGuts.h>
#include <Oscil.h>
#include <Smooth.h>
#include <AutoMap.h>
#include <ADSR.h>

#include <tables/sin256_int8.h>
#include <tables/halfsin256_uint8.h>
#include <tables/waveshape1_softclip_int8.h>
#include <tables/saw256_int8.h>

// #include <tables/sinsaw256_int8.h>
//#include <tables/.h>
//#include <tables/.h>
//#include <tables/.h>
#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"


// VARIABLES
AutoMap kMapCarrierFreq(0, 1023, 20, 440);
AutoMap kMapIntensity(0, 1023, 700, 1);
AutoMap kMapModSpeed(0, 1023, 10000, 0);
AutoMap kMapEnvelopeTimes(0, 1023, 20, 500);

Oscil<256, AUDIO_RATE> carrier;
Oscil<256, AUDIO_RATE> modulator;
Oscil<256, CONTROL_RATE> modDepth;

ADSR<AUDIO_RATE, AUDIO_RATE> envelope;

int attack = 20;
int decay = 20;
bool mode = 0;
bool noteTrigger = 0;

int freqs[3];
byte wfs[3];

int mod_ratio = 5;  // brightness (harmonics)
long fm_intensity;  // carries control info from updateControl to updateAudio

float smoothness = 0.95f;
Smooth<long> aSmoothIntensity(smoothness);

const static int PARAM_LENGTH = 9;
byte POSITION = 0;
String param_name[PARAM_LENGTH] = { "Freq1", "Freq2", "Freq3", "WFS1", "WFS2", "WFS3", "mod_ratio", "Decay", "HifiMode" };
int param[PARAM_LENGTH] = { 222, 22, 2, 0, 0, 0, 1, 200, 0 };
bool* ledGroup[PARAM_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5, Led_6, Led_M, Led_D, Led_H };
byte tmp_d11 = 0;


// FUNCTIONS
void setWavetables() {
  if (wfs[0] == 0) {
    carrier.setTable(SIN256_DATA);
  } else if (wfs[0] == 1) {
    carrier.setTable(HALFSIN256_DATA);
  } else if (wfs[0] == 2) {
    carrier.setTable(SAW256_DATA);
  } else if (wfs[0] == 3) {
    carrier.setTable(WAVESHAPE1_SOFTCLIP_DATA);
  }

  if (wfs[1] == 0) {
    modulator.setTable(SIN256_DATA);
  } else if (wfs[1] == 1) {
    modulator.setTable(HALFSIN256_DATA);
  } else if (wfs[1] == 2) {
    modulator.setTable(SAW256_DATA);
  } else if (wfs[1] == 3) {
    modulator.setTable(WAVESHAPE1_SOFTCLIP_DATA);
  }

  if (wfs[2] == 0) {
    modDepth.setTable(SIN256_DATA);
  } else if (wfs[2] == 1) {
    modDepth.setTable(HALFSIN256_DATA);
  } else if (wfs[2] == 2) {
    modDepth.setTable(SAW256_DATA);
  } else if (wfs[2] == 3) {
    modDepth.setTable(WAVESHAPE1_SOFTCLIP_DATA);
  }
}

void readPins() {
  // freqs[0] = mozziAnalogRead(A0);
  // freqs[1] = mozziAnalogRead(A1);
  // freqs[2] = mozziAnalogRead(A2);
  freqs[0] = param[0];
  freqs[1] = param[1];
  freqs[2] = param[2];

  // wfs[0] = mozziAnalogRead(A3) >> 8;
  // wfs[1] = mozziAnalogRead(A4) >> 8;
  // wfs[2] = mozziAnalogRead(A5) >> 8;
  wfs[0] = param[3] >> 8;
  wfs[1] = param[4] >> 8;
  wfs[2] = param[5] >> 8;

  // mod_ratio = (mozziAnalogRead(A6) >> 7) + 1;
  mod_ratio = (param[6] >> 7) + 1;
  //attack = mozziAnalogRead(A6);
  // decay = mozziAnalogRead(A7);
  decay = param[7];

  // mode = digitalRead(2);
  // noteTrigger = digitalRead(3) or not digitalRead(4);
  mode = param[8];
  noteTrigger = digitalRead(11);
}

void setFrequencies() {
  int carrier_freq = kMapCarrierFreq(freqs[0]);
  int mod_freq = carrier_freq * mod_ratio;

  int intensity_calibrated = kMapIntensity(freqs[2]);
  fm_intensity = ((long)intensity_calibrated * (modDepth.next() + 128)) >> 8;

  float mod_speed = (float)kMapModSpeed(freqs[1]) / 1000;

  carrier.setFreq(carrier_freq);
  modulator.setFreq(mod_freq);
  modDepth.setFreq(mod_speed);
}

void setAD() {
  //envelope.setAttackTime(kMapEnvelopeTimes(attack));
  envelope.setReleaseTime(kMapEnvelopeTimes(decay));
}

// ################### THE GOODS! #####################

void setup() {
  // pinMode(4, INPUT_PULLUP);
  // pinMode(2, INPUT);
  Serial.begin(115200);  //串口输出日志
  startMozzi();
  envelope.setLevels(255, 250, 250, 0);
  envelope.setTimes(20, 20, 5000, 20);

  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚
  pinMode(11, INPUT_PULLUP);
}

void updateControl() {
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                   //display  //用以展示控制

  Serial.println(POSITION + param_name[POSITION] + param[POSITION]);  //func param Log

  readPins();
  setWavetables();
  setFrequencies();
  setAD();

  if (noteTrigger == 0) {
    envelope.noteOn();
  } else {
    envelope.noteOff();
  }
}

int updateAudio() {
  int audio;

  if (mode == 0) {
    long mod = aSmoothIntensity.next(fm_intensity) * modulator.next();
    audio = carrier.phMod(mod);
  } else {
    envelope.update();
    long mod = aSmoothIntensity.next(fm_intensity) * modulator.next();
    audio = ((carrier.phMod(mod) * envelope.next()) >> 8);
  }
  return audio << 6;  // 14-bit number for HI-FI mode
}

void loop() {
  audioHook();
}
