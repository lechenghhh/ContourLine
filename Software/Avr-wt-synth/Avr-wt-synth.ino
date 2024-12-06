/*
 * ATARI GLITCH Eurorack module by Alex Bohrium
 * firmware v1.0
 */

#include <MozziGuts.h>
#include <Oscil.h>  // oscillator

#include "tables/halfsin2048_int8.h"  // table for Oscils to play
#include <tables/sin2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle2048_int8.h>

#include <Smooth.h>
#include <AutoMap.h>  // maps unpredictable inputs to a range

#include <mozzi_rand.h>
#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"

/* constants
 *  
 */

// uncomment to debug messages via serial cable
// #define DEBUG_INPUT
// #define DEBUG
#define DEFAULT_BRIGHT 5  // brightness, harmonics
#define DEFAULT_VOL 10    // volume

// constants for mappings

// desired carrier frequency (pitch)
#define MIN_CARRIER_FREQ 32.70
#define MAX_CARRIER_FREQ 1046.50
#define DEFAULT_FREQ 220  // if no 1V/Oct signal

// Obertone freq
#define MIN_OBER_FREQ 1
#define MAX_OBER_FREQ 100

// desired intensity max and min, (glitch amount)
#define MAX_INTENSITY 700
#define MIN_INTENSITY 1

// desired mod speed max and min, (fm mod rate)
#define MAX_MOD_SPEED 10000
#define MIN_MOD_SPEED 1

// number of wavetables
#define MAX_WT 5

/* mappings
 *
 */
#define ANALOG_ZERO 10  // minimum treshold to detect a zero/non-zero state (0-1023)

// knobs and CVs maps
AutoMap kMapCarrierFreq(0, 1023, MIN_CARRIER_FREQ, MAX_CARRIER_FREQ);
AutoMap kMapIntensity(0, 1023, MIN_INTENSITY, MAX_INTENSITY);
AutoMap kMapModSpeed(0, 1023, MIN_MOD_SPEED, MAX_MOD_SPEED);
AutoMap kMapOberFreq(0, 1023, MIN_OBER_FREQ, MAX_OBER_FREQ);
AutoMap kMapWT(0, 1023, 1, MAX_WT);
// note maps
AutoMap tone_index_map(0, 1023, 0, 59);

// notes from C1 to C6
float note_freq[61] = { 65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
                        130.81, 138.59, 147.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
                        261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
                        523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77,
                        1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1720.00, 1864.60, 1975.50, 2093.00 };

// Hadrware depending configuration
// digital i/o
#define DEBUG_LED 0     // LED for internal debug
#define TRIGGER_LED 1   // Trigger Led will blink when the gate is open
#define RND_SWITCH 13   // Switch RND/Normal 12or13
#define TRIGGER_PIN 11  // Sound trigger pin
// analog input
// knobs
// #define OBER_KNOB A3    // Obertone knob
// #define MOD_KNOB A2     // Modulation rate knob
// #define WT_KNOB A4      // Wavetable select knob
// #define GLITCH_KNOB A5  // Glitch knob
// CVs
#define PITCH_CV A0   // 1/OCT CV               (base carrier freq / pitch)
#define MOD_CV A1     // Modulation rate CV     (wah-wah effect)
#define OBER_CV A2    // Ober CV                (obertone freq / pitch)
#define GLITCH_CV A3  // Glitch amount CV       (FM intensity / glitch)

#define PARAM_LENGTH 6  // menu length

/* variables (may be changed during the work)
 *  
 */

Oscil<2048, AUDIO_RATE> aOpCarrier;
Oscil<2048, AUDIO_RATE> aOpModulator;
Oscil<2048, CONTROL_RATE> cOpIntensityMod;

long fm_intensity;  // carriers control info from updateControl to updateAudio

// smoothing for intensity to remove clicks on transitions
float smoothness = 0.95f;

Smooth<long> aSmoothIntensity(smoothness);

// trigger on flag (change me on trigger button or CV)
bool isTrigger = false;

// regenerate flag
bool isNeedRegenerate = false;

// mode (rnd-generated or manually set)
bool isRND = true;

// define wavetable types
typedef enum WT {
  HSN = 1,
  SIN = 2,
  SQR = 3,
  SAW = 4,
  TRI = 5,
  RND = 6,
} WT;

// define FM-based sound variables
typedef struct tFM {
  int wt;           // wavetable number
  float carr_freq;  // carrier freq
  int ober_freq;    // obertone freq
  int vol;          // volume
  int fmi;          // FM intensity
  int bright;       // Brightness (harmonics)
  float mod;        // Modulation rate
} tFM;

byte POSITION = 0;
char param_name[PARAM_LENGTH][5] = { "pitch", "wt", "mod", "ober", "glitch", "rand" };
bool* ledGroup[PARAM_LENGTH] = { Led_P, Led_W, Led_M, Led_O, Led_G, Led_R };

short param[PARAM_LENGTH] = { 1, 0, 0, 0, 0, 0 };

// use this param_name only on debug! it has delay!
void blink_led(int led) {
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  delay(100);
}

// init
void setup() {
  Serial.begin(115200);  // set up the Serial output

#ifdef DEBUG
#endif
  // define pins
  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  initLED(2, 3, 4, 5, 6, 7, 8);   //初始化Led引脚

  // pinMode(DEBUG_LED, OUTPUT);
  // pinMode(TRIGGER_LED, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT);        // trigger cv
  // pinMode(RND_SWITCH, INPUT_PULLUP);  // Switch connected to GND
  blink_led(DEBUG_LED);
  blink_led(TRIGGER_LED);

  randSeed();
  blink_led(DEBUG_LED);
  blink_led(TRIGGER_LED);

  startMozzi();
  blink_led(DEBUG_LED);
  blink_led(TRIGGER_LED);

#ifdef DEBUG
  Serial.print("Setup finish\n\r");
#endif
}

// update Osc values
void mySetOsc(tFM fmSnd) {
#ifdef DEBUG
  Serial.print("        SetOSC:\tWT=");
  Serial.print(fmSnd.wt);
  Serial.print("\tCARR_FREQ=");
  Serial.print(fmSnd.carr_freq);
  Serial.print("\tOBER_FREQ=");
  Serial.print(fmSnd.ober_freq);
  Serial.print("\tVOL=");
  Serial.print(fmSnd.vol);
  Serial.print("\tGLCH=");
  Serial.print(fmSnd.fmi);
  Serial.print("\tBRIGHT=");
  Serial.print(fmSnd.bright);
  Serial.print("\tMOD=");
  Serial.print(fmSnd.mod);
  Serial.print(".\n\r");
#endif
  // set wavetable
  switch (fmSnd.wt) {
    case HSN:
      aOpCarrier.setTable(HALFSIN2048_DATA);
      aOpModulator.setTable(HALFSIN2048_DATA);
      cOpIntensityMod.setTable(HALFSIN2048_DATA);
      break;
    case SIN:
      aOpCarrier.setTable(SIN2048_DATA);
      aOpModulator.setTable(SIN2048_DATA);
      cOpIntensityMod.setTable(SIN2048_DATA);
      break;
    case SQR:
      aOpCarrier.setTable(SQUARE_NO_ALIAS_2048_DATA);
      aOpModulator.setTable(SQUARE_NO_ALIAS_2048_DATA);
      cOpIntensityMod.setTable(SIN2048_DATA);
      fmSnd.vol = 5;  // reduce volume for this wave
      break;
    case SAW:
      aOpCarrier.setTable(SAW2048_DATA);
      aOpModulator.setTable(SAW2048_DATA);
      cOpIntensityMod.setTable(SIN2048_DATA);
      break;
    case TRI:
      aOpCarrier.setTable(TRIANGLE2048_DATA);
      aOpModulator.setTable(TRIANGLE2048_DATA);
      cOpIntensityMod.setTable(SIN2048_DATA);
      break;
  }
  //calculate the modulation frequency to stay in ratio
  int mod_freq = fmSnd.carr_freq * fmSnd.ober_freq * fmSnd.bright;
  // calculate the fm_intensity
  fm_intensity = ((long)fmSnd.fmi * fmSnd.vol * (cOpIntensityMod.next() + 128)) >> 8;  // shift back to range after 8 bit multiply
  // set the FM oscillator frequencies
  aOpCarrier.setFreq(fmSnd.carr_freq);
  aOpModulator.setFreq(mod_freq);
  cOpIntensityMod.setFreq(fmSnd.mod);
}


// Mozzi Update Controls
void updateControl() {
  POSITION = getPostition(POSITION, PARAM_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);      //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                   //display  //用字母展示控制

  // for (int i = 2; i < 9; i++)                      //display  //简易参数展示//无需导入led显示库
  //   digitalWrite(i, HIGH);
  // digitalWrite(POSITION + 2, LOW);

  Serial.print("func");           //func param
  Serial.print(POSITION);         //func param
  Serial.print("=");              //func param
  Serial.print(param[POSITION]);  //func param
  Serial.print(" isRND=");        //func param
  Serial.print(isRND);            //func param
  Serial.print("\n");             //func param

  // Serial.print(" d12= ");         //func param
  // Serial.print(digitalRead(12));  //func param
  // Serial.print(" d13= ");         //func param
  // Serial.print(digitalRead(13));  //func param

  // digital read
  isTrigger = (digitalRead(TRIGGER_PIN) == HIGH) ? true : false;  // trigger pin connected to an external +5V signal
  // isRND = (digitalRead(RND_SWITCH) == LOW) ? true : false;        // rnd switch connected to the GND
  isRND = param[5] >> 9;  // rnd switch connected to the GND

#ifdef DEBUG_INPUT
  Serial.print("        UpdateControl.Read:\tisTrigger=");
  Serial.print(isTrigger);
  Serial.print("\tisRND=");
  Serial.print(isRND);
  Serial.print("\n\r");
#endif

  // set trigger led
  if (isTrigger) {
    digitalWrite(TRIGGER_LED, HIGH);
  } else {
    digitalWrite(TRIGGER_LED, LOW);
    // need regenerate on the next trigger comming
    if (isRND && !isNeedRegenerate) {
      isNeedRegenerate = true;
    };
    // we don't want do anything without a trigger
    return;
  }

  // analog read
  // knobs
  // int wtKnobVal = mozziAnalogRead(WT_KNOB);       // read wavetable knob        value is 0-1023
  // int modKnobVal = mozziAnalogRead(MOD_KNOB);     // read modulation rate knob  value is 0-1023
  // int oberKnobVal = mozziAnalogRead(OBER_KNOB);   // read obertone knob         value is 0-1023
  // int fmiKnobVal = mozziAnalogRead(GLITCH_KNOB);  // read glitch knob           value is 0-1023
  int carrKnobVal = param[0];  // read pitch knob
  int wtKnobVal = param[1];    // read wavetable knob        value is 0-1023
  int modKnobVal = param[2];   // read modulation rate knob  value is 0-1023
  int oberKnobVal = param[3];  // read obertone knob         value is 0-1023
  int fmiKnobVal = param[4];   // read glitch knob           value is 0-1023

  // CVs
  int carrCVVal = mozziAnalogRead(PITCH_CV) + (carrKnobVal >> 2);  // read 1V/OCT pitch          value is 0-1023
  int modCVVal = mozziAnalogRead(MOD_CV);                          // read modulation rate CV    value is 0-1023
  int oberCVVal = mozziAnalogRead(OBER_CV);                        // read obertone CV           value is 0-1023
  int fmiCVVal = mozziAnalogRead(GLITCH_CV);                       // read glitch CV             value is 0-1023

  tFM fmSnd;  // sound configuration instance
  // AutoMap has an issue: it return wrong value till it doesn't recive the maximum.
  fmSnd.wt = kMapWT(1023);
  fmSnd.carr_freq = kMapCarrierFreq(1023);
  fmSnd.ober_freq = kMapOberFreq(1023);
  fmSnd.vol = DEFAULT_VOL;
  fmSnd.fmi = kMapIntensity(1023);
  fmSnd.bright = DEFAULT_BRIGHT;
  fmSnd.mod = kMapModSpeed(1023);
  byte noteNum = tone_index_map(1023);

#ifdef DEBUG_INPUT
  Serial.print("\tWTknob=");
  Serial.print(wtKnobVal);
  Serial.print("\tMODknob=");
  Serial.print(modKnobVal);
  Serial.print("\toOBERknob=");
  Serial.print(oberKnobVal);
  Serial.print("\tGLCHknob=");
  Serial.print(fmiKnobVal);
  Serial.print("\tCARRcv=");
  Serial.print(carrCVVal);
  Serial.print("\tMODcv=");
  Serial.print(modCVVal);
  Serial.print("\tOBERcv=");
  Serial.print(oberCVVal);
  Serial.print("\tGLCHcv=");
  Serial.print(fmiCVVal);
  Serial.print(".\n\r");
#endif

  if (isRND) {
    // this is RANDOM mode
    // do we need to generate a new sound? (only on a new trigger)
    if (isNeedRegenerate) {
      // set WT limited by WT Knob (if > 0)
      int iMaxWT = (wtKnobVal > ANALOG_ZERO) ? kMapWT(wtKnobVal) : MAX_WT;  // limit WT range by WT knob (if > 0)
      fmSnd.wt = rand((int)1, (int)iMaxWT + 1);
      // set pitch = CV if it's exists, or RND if no 1V/OCT signal
      fmSnd.carr_freq = (carrCVVal > ANALOG_ZERO) ? kMapCarrierFreq(carrCVVal) : rand((int)MIN_CARRIER_FREQ, (int)MAX_CARRIER_FREQ + 1);  // starting not from minimum to skip sub freq
      // set obertone
      int iMaxOber = (oberKnobVal > ANALOG_ZERO) ? kMapOberFreq(oberKnobVal) : MAX_OBER_FREQ;  // limit Obertone Freq by Ober Knob (if > 0)
      fmSnd.ober_freq = rand((int)MIN_OBER_FREQ, (int)iMaxOber + 1);
      // set volume
      fmSnd.vol = DEFAULT_VOL;
      // set glitch
      int iMaxIntensity = (fmiKnobVal > ANALOG_ZERO) ? kMapIntensity(fmiKnobVal) : MAX_INTENSITY;  // limit intensity by Glitch Knob (if > 0)
      fmSnd.fmi = rand((int)MIN_INTENSITY, (int)iMaxIntensity + 1);
      // set modulation rate
      int iMaxMod = (modKnobVal > ANALOG_ZERO) ? kMapModSpeed(modKnobVal) : MAX_MOD_SPEED;  // limit intensity by Glitch Knob (if > 0)
      if (iMaxMod < MIN_MOD_SPEED) { iMaxMod = MIN_MOD_SPEED; };
      fmSnd.mod = (float)rand((int)MIN_MOD_SPEED, (int)iMaxMod - MIN_MOD_SPEED + 1) / 1000;
      // set brightness
      fmSnd.bright = DEFAULT_BRIGHT;  // rand((int)1,(int)32);
      mySetOsc(fmSnd);
      // regeniration is done
      isNeedRegenerate = false;

#ifdef DEBUG
      Serial.print("UpdateControl.RNDGen:\tCARR_FREQ=");
      Serial.print(fmSnd.carr_freq);
      Serial.print("\tlimitWT=");
      Serial.print(iMaxWT);
      Serial.print("\tlimitOBER=");
      Serial.print(iMaxOber);
      Serial.print("\tlimitCLCH=");
      Serial.print(iMaxIntensity);
      Serial.print("\tlimitMOD=");
      Serial.print(iMaxMod);
      Serial.print(".\n\r");
#endif
    }
    return;  // end of RND mode

  } else {
    // this is NORMAL mode

    // define FM
    fmSnd.wt = kMapWT(wtKnobVal);
    fmSnd.vol = DEFAULT_VOL;  // constant volume
    // set pitch
    // fmSnd.carr_freq = kMapCarrierFreq( carrCVVal ); // raw freq mapping
    noteNum = tone_index_map(carrCVVal) + 1;
    // hack: map() detect 0 and 15 as the same value. It returns 0.
    if (carrCVVal <= 10) { noteNum = 0; };
    if ((carrCVVal > 10) && (carrCVVal < 30)) { noteNum = 1; };
    fmSnd.carr_freq = (float)note_freq[noteNum];  // quantized notes
#ifdef DEBUG
    Serial.print("\tnoteNum=");
    Serial.print(noteNum);
    Serial.print(".\n\r");
#endif
    // set obertone freq
    fmSnd.ober_freq = (oberCVVal > ANALOG_ZERO) ? kMapOberFreq(oberCVVal) : kMapOberFreq(oberKnobVal);
    // set FM intensity (glitch)
    fmSnd.fmi = (fmiCVVal > ANALOG_ZERO) ? kMapIntensity(fmiCVVal) : kMapIntensity(fmiKnobVal);
    // set modulation rate
    fmSnd.mod = (modCVVal > ANALOG_ZERO) ? (float)kMapModSpeed(modCVVal) / 1000 : (float)kMapModSpeed(modKnobVal) / 1000;
    fmSnd.bright = DEFAULT_BRIGHT;
    // set resutl
    mySetOsc(fmSnd);
    return;  // end of NORMAL mode
  }
}


int updateAudio() {
  // silence when not triggered
  if (!isTrigger) { return; }

  long modulation = aOpModulator.next() * aSmoothIntensity.next(fm_intensity);
  return aOpCarrier.phMod(modulation);
}


void loop() {
  audioHook();
}
