#include <MozziGuts.h>
#include <Oscil.h>                // oscillator template
#include <tables/sin2048_int8.h>  // sine table for oscillator
//#include <mozzi_midi.h>
#include <tables/cos2048_int8.h>  // table for Oscils to play
#include <Smooth.h>
#include <AutoMap.h>  // maps unpredictable inputs to a range
#include <tables/cos8192_int8.h>
#include <tables/envelop2048_uint8.h>
#define CONTROL_RATE 512  // quite fast, keeps modulation smooth
#include <tables/cos2048_int8.h>
//#include <EventDelay.h>
#include <mozzi_rand.h>
//#include <mozzi_midi.h>
//#include <WaveShaper.h>
//#include <mozzi_analog.h>
#include <ReverbTank.h>
//#include <WavePacket.h>
//#include <RollingAverage.h>
//
//#define FUNDAMENTAL_PIN 0
//#define BANDWIDTH_PIN 1
//#define CENTREFREQ_PIN 2

//#include <tables/waveshape_chebyshev_3rd_256_int8.h>
//#include <tables/waveshape_chebyshev_6th_256_int8.h>
//#include <tables/waveshape_compress_512_to_488_int16.h>
#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"
#include "Module_Const.h"

const int MIN_CARRIER_FREQ = 22;
const int MAX_CARRIER_FREQ = 440;
const int MIN = 1;
const int MAX = 10;
const int MIN_2 = 1;
const int MAX_2 = 15;
const int MIN_INTENSITY = 700;
const int MAX_INTENSITY = 10;
const int MIN_MOD_SPEED = 10000;
const int MAX_MOD_SPEED = 1;
int mod_ratio = 5;
long fm_intensity;
float smoothness = 0.95f;
//byte lo_note = 24; // midi note numbers
//byte hi_note = 36;
//long audio_steps_per_gliss = AUDIO_RATE / 4; // ie. 4 glisses per second
//long control_steps_per_gliss = CONTROL_RATE / 4;
//int counter = 0;
//byte gliss_offset = 0;
//byte gliss_offset_step = 2;
//byte  gliss_offset_max = 36;
//int preset_num;
//int target_freq, smoothed_freq;
//byte note = 0;

//Owen's First Preset
Smooth<long> aSmoothIntensity(smoothness);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod(COS2048_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCarrier(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModulator(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModWidth(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kModFreq1(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kModFreq2(COS8192_DATA);
Oscil<ENVELOP2048_NUM_CELLS, AUDIO_RATE> aEnvelop(ENVELOP2048_DATA);
// Phil's Preset
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aGain1(SIN2048_DATA); // to fade sine wave in and out before waveshaping
//Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aGain2(SIN2048_DATA); // to fade sine wave in and out before waveshaping
//WaveShaper <char> aCheby3rd(CHEBYSHEV_3RD_256_DATA); // 5th harmonic
//WaveShaper <char> aCheby6th(CHEBYSHEV_6TH_256_DATA); // 8th harmonic
//WaveShaper <int> aCompress(WAVESHAPE_COMPRESS_512_TO_488_DATA); // to compress instead of dividing by 2 after adding signals
// Owen's reverb preset
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aCarrier_b(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModulator_b(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, AUDIO_RATE> aModWidth_b(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kModFreq1_b(COS8192_DATA);
Oscil<COS8192_NUM_CELLS, CONTROL_RATE> kModFreq2_b(COS8192_DATA);
Oscil<ENVELOP2048_NUM_CELLS, AUDIO_RATE> aEnvelop_b(ENVELOP2048_DATA);


Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aCarrier_c(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, AUDIO_RATE> aModulator_c(COS2048_DATA);
Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kIntensityMod_c(COS2048_DATA);

ReverbTank reverb;

AutoMap kMapCarrierFreq(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0, 1023, MIN_INTENSITY, MAX_INTENSITY);
AutoMap kMapModSpeed(0, 1023, MIN_MOD_SPEED, MAX_MOD_SPEED);
AutoMap mapThis(0, 1023, MIN, MAX);
AutoMap mapThisToo(0, 1023, MIN_2, MAX_2);

byte POSITION = 0;
#define FUNC_LENGTH 5
char function[FUNC_LENGTH][6] = { "1", "2", "3", "4", "5" };
int param[FUNC_LENGTH] = { 0, 1, 32, 3, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_1, Led_2, Led_3, Led_4, Led_5 };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率

  initCtrl(4, 50, 13, 12, 1);
  initLED(2, 3, 4, 5, 6, 7, 8);  //初始化Led引脚
  pinMode(11, INPUT);

  //  aSin.setFreq(440); // set the frequency
  //  ratio = float_to_Q8n8(3.0f);   // define modulation ratio in float and convert to fixed-point
  //  kNoteChangeDelay.set(200); // note duration ms, within resolution of CONTROL_RATE
  //  bModDepth.setFreq(13.f);     // vary mod depth to highlight am effects
  randSeed();  // reseed the random generator for different results each time the sketch runs
  aCarrier.setFreq(220);
  kModFreq1.setFreq(1.78f);
  kModFreq2.setFreq(0.1757f);
  aModWidth.setFreq(0.1434f);
  aEnvelop.setFreq(9.0f);

  //  aSin.setFreq(110); // set the frequency
  //  aGain1.setFreq(2.f); // use a float for low frequencies, in setup it doesn't need to be fast
  //  aGain2.setFreq(.4f);
  //  kChangeNoteDelay.set(2000);

  aCarrier_b.setFreq(55);
  kModFreq1_b.setFreq(3.98f);
  kModFreq2_b.setFreq(3.31757f);
  aModWidth_b.setFreq(2.52434f);
  aEnvelop_b.setFreq(9.0f);

  startMozzi(CONTROL_RATE);
}


byte preset_num = 0;

void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  // Serial.print(POSITION);           //func param
  // Serial.println("func");           //func param
  // Serial.println(param[POSITION]);  //func param
  // Serial.println(" ");

  // Change Presets
  int preset_knob = param[0];
  //  if(preset_knob <= 342){
  //      preset_num = 0;
  //  }
  //  else if (preset_knob <= 684 && preset_knob > 342){
  //    preset_num = 1;
  //  }
  //  else{
  //      preset_num = 2;
  //  }
  preset_num = map(preset_knob, 100, 900, 0, 2);


  //Preset 2
  int freqVal = param[1];  // value is 0-1023
  int FRQ = mapThis(freqVal);
  int knob2 = param[2];  // value is 0-1023
  int knob2Val = mapThis(knob2);
  aModWidth.setFreq(knob2);
  int knob3 = param[3];
  int carrier_freq = kMapCarrierFreq(knob3);
  kModFreq2.setFreq(knob3);
  int mod_freq = carrier_freq * mod_ratio * FRQ;
  aCarrier.setFreq(carrier_freq);
  aModulator.setFreq(mod_freq);
  int knob4 = param[4];
  int LDR1_calibrated = mapThis(knob3);
  fm_intensity = ((long)LDR1_calibrated * knob2Val * (kModFreq1.next() + 128)) >> 8;
  //int LDR2_value= param[3]; // value is 0-1023
  float mod_speed = (float)kMapModSpeed(LDR1_calibrated) / 1000;
  kIntensityMod.setFreq(LDR1_calibrated);

  //Preset 0
  //   if(kChangeNoteDelay.ready()){
  //    Q16n16 midi_note = Q8n0_to_Q16n16(octave_start_note+rndPentatonic());
  //    target_freq = Q16n16_to_Q16n0(Q16n16_mtof(midi_note)); // has to be 16 bits for Smooth
  //    kChangeNoteDelay.start();
  //   }
  //  smoothed_freq = kSmoothFreq.next(target_freq*4); // temporarily scale up target_freq to get better int smoothing at low values
  //  aSin.setFreq(smoothed_freq/4); // then scale it back down after it's smoothed
  //  int mult = map(analogRead(A1), 0, 1024, 1, 5);
  //  int mult2 = map(analogRead(A2), 0, 1024, 1, 5);
  //
  //  int sin_freq = map(analogRead(A3), 0, 1024, 50, 100);
  //  aSin.setFreq(sin_freq*mult);
  //  int gain_freq = map(analogRead(A4), 0, 1024, 0.f, 5.f);
  //  aGain1.setFreq(gain_freq*mult2);

  //  wavey.set(kAverageF.next(param[2] * param[1])+1,
  //  kAverageBw.next(param[3]),
  //  kAverageCf.next(param[4]));

  // Preset 1
  int k1 = param[1] * param[3];
  aCarrier_b.setFreq(k1);

  int k2 = param[2] * param[4];
  aModWidth_b.setFreq(k2);

  aModulator_b.setFreq(277.0f + 0.4313f * kModFreq1_b.next() + kModFreq2_b.next());

  //Last preset

  int freq2 = param[1];  // value is 0-1023
  int FRQ2 = mapThis(freq2);

  int knob22 = param[2];  // value is 0-1023
  int knob22Val = mapThis(knob22);

  // read the knob
  int knob32 = param[3];  // value is 0-1023

  // map the knob to carrier frequency
  int carrier_freq2 = kMapCarrierFreq(knob32);

  //calculate the modulation frequency to stay in ratio
  int mod_freq2 = carrier_freq2 * mod_ratio * FRQ2;

  // set the FM oscillator frequencies
  aCarrier_c.setFreq(carrier_freq2);
  aModulator_c.setFreq(mod_freq2);

  int knob42 = param[4];  // value is 0-1023
  // print the value to the Serial monitor for debugging

  int knob4_calibrated = kMapIntensity(knob42);
  // calculate the fm_intensity
  fm_intensity = ((long)knob4_calibrated * knob22Val * (kIntensityMod.next() + 128)) >> 8;  // shift back to range after 8 bit multiply

  // read the light dependent resistor on the speed Analog input pin
  //int LDR2_value= param[4]; // value is 0-1023

  // use a float here for low frequencies
  float mod_speed2 = (float)kMapModSpeed(knob42) / 1000;

  kIntensityMod.setFreq(mod_speed);
}


int updateAudio() {
  if (preset_num == 0) {
    int asig = aCarrier.phMod((int)aModulator.next() * (260u + aModWidth.next()));
    return (asig * (byte)aEnvelop.next()) >> 8;
  }

  else if (preset_num == 1) {
    int synth = aCarrier_b.phMod((int)aModulator_b.next() * (150u + aModWidth_b.next()));
    synth *= (byte)aEnvelop_b.next();
    synth >>= 8;
    // here's the reverb
    int arev = reverb.next(synth);
    // add the dry and wet signals
    return synth + (arev >> 3);
  } else if (preset_num == 2) {
    startMozzi(64);
    long modulation = aSmoothIntensity.next(fm_intensity) * aModulator.next();
    return aCarrier.phMod(modulation);
  }
}


void loop() {
  audioHook();  // required here
}
