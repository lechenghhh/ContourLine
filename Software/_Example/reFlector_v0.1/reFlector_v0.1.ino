/*  Using Mozzi sonification library.
  
	Included far too many wavetables with purpose to 
	select the most appropriate sound for the job.
*/


//#include <ADC.h>  // Teensy 3.0/3.1 uncomment this line and install http://github.com/pedvide/ADC
#include <MozziGuts.h>
#include <Oscil.h>
#include <AudioDelay.h>
#include <mozzi_midi.h> // for mtof
#include <EventDelay.h>
#include <ControlDelay.h>
#include <tables/smoothsquare8192_int8.h>
#include <tables/square_analogue512_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle_analogue512_int8.h> // wavetable
#include <tables/triangle512_int8.h> // wavetable
#include <tables/triangle1024_int8.h> // wavetable
#include <tables/triangle_valve_2048_int8.h>
#include <tables/triangle_valve_2_2048_int8.h>
#include <tables/triangle_warm8192_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <tables/triangle_dist_squared_2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/waveshape_chebyshev_3rd_256_int8.h>
#include <tables/waveshape_chebyshev_5th_256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_sigmoid_int8.h>
#include <tables/waveshape1_softclip_int8.h>
#include <tables/saw_analogue512_int8.h>
#include <tables/sin2048_int8.h>

#include <tables/cos2048_int8.h> // wavetable

#define CONTROL_RATE 128 // powers of 2 please
#define CONTROL A4 // Arpeggiator buttons
#define MELODY A3 // Melody buttons

Oscil<SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> wave(SMOOTHSQUARE8192_DATA); // audio oscillator
//Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> wave(SQUARE_ANALOGUE512_DATA); // audio oscillator
//Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> wave(SQUARE_NO_ALIAS_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_ANALOGUE512_DATA); // audio oscillator
//Oscil<TRIANGLE512_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE512_DATA); // audio oscillator
//Oscil<TRIANGLE1024_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE1024_DATA); // audio oscillator
//Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_VALVE_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_VALVE_2_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_WARM8192_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_WARM8192_DATA); // audio oscillator
//Oscil<TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_DIST_CUBED_2048_DATA); // audio oscillator - NIIIICE METALIC
//Oscil<TRIANGLE_DIST_SQUARED_2048_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_DIST_SQUARED_2048_DATA); // audio oscillator - SOOO HIGH
//Oscil<TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> wave(TRIANGLE_HERMES_2048_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_3RD_256_NUM_CELLS, AUDIO_RATE> wave(CHEBYSHEV_3RD_256_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_5TH_256_NUM_CELLS, AUDIO_RATE> wave(CHEBYSHEV_5TH_256_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_6TH_256_NUM_CELLS, AUDIO_RATE> wave(CHEBYSHEV_6TH_256_DATA); // audio oscillator
//Oscil<WAVESHAPE_SIGMOID_NUM_CELLS, AUDIO_RATE> wave(WAVESHAPE_SIGMOID_DATA); // audio oscillator
//Oscil<WAVESHAPE1_SOFTCLIP_NUM_CELLS, AUDIO_RATE> wave(WAVESHAPE1_SOFTCLIP_DATA); // audio oscillator
//Oscil<SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> wave(SAW_ANALOGUE512_DATA); // audio oscillator

//Oscil<SMOOTHSQUARE8192_NUM_CELLS, AUDIO_RATE> waveMelody(SMOOTHSQUARE8192_DATA); // audio oscillator
//Oscil<SQUARE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> waveMelody(SQUARE_ANALOGUE512_DATA); // audio oscillator
//Oscil<SQUARE_NO_ALIAS_2048_NUM_CELLS, AUDIO_RATE> waveMelody(SQUARE_NO_ALIAS_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_ANALOGUE512_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_ANALOGUE512_DATA); // audio oscillator
//Oscil<TRIANGLE512_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE512_DATA); // audio oscillator
//Oscil<TRIANGLE1024_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE1024_DATA); // audio oscillator
//Oscil<TRIANGLE_VALVE_2048_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_VALVE_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_VALVE_2_2048_DATA); // audio oscillator
Oscil<TRIANGLE_WARM8192_NUM_CELLS, AUDIO_RATE> waveMelody2(TRIANGLE_WARM8192_DATA); // audio oscillator
//Oscil<TRIANGLE_DIST_CUBED_2048_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_DIST_CUBED_2048_DATA); // audio oscillator
//Oscil<TRIANGLE_DIST_SQUARED_2048_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_DIST_SQUARED_2048_DATA); // audio oscillator  
//Oscil<TRIANGLE_HERMES_2048_NUM_CELLS, AUDIO_RATE> waveMelody(TRIANGLE_HERMES_2048_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_3RD_256_NUM_CELLS, AUDIO_RATE> waveMelody(CHEBYSHEV_3RD_256_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_5TH_256_NUM_CELLS, AUDIO_RATE> waveMelody(CHEBYSHEV_5TH_256_DATA); // audio oscillator  
//Oscil<CHEBYSHEV_6TH_256_NUM_CELLS, AUDIO_RATE> waveMelody(CHEBYSHEV_6TH_256_DATA); // audio oscillator
//Oscil<WAVESHAPE_SIGMOID_NUM_CELLS, AUDIO_RATE> waveMelody(WAVESHAPE_SIGMOID_DATA); // audio oscillator
//Oscil<WAVESHAPE1_SOFTCLIP_NUM_CELLS, AUDIO_RATE> waveMelody(WAVESHAPE1_SOFTCLIP_DATA); // audio oscillator
//Oscil<SAW_ANALOGUE512_NUM_CELLS, AUDIO_RATE> waveMelody(SAW_ANALOGUE512_DATA); // audio oscillator
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> waveMelody(SIN2048_DATA); // audio oscillator

Oscil<TRIANGLE512_NUM_CELLS, CONTROL_RATE> kFreq(TRIANGLE512_DATA);

// the delay time, measured in samples, updated in updateControl, and used in updateAudio 
unsigned int del_samps;

EventDelay kNoteChangeDelay;

AudioDelay <256> aDel;

ControlDelay <128, int> kDelay; // 2seconds
ControlDelay <128, int> kDelayMelody; // 2seconds

uint8_t echo_cells_1 = 32;
uint8_t echo_cells_2 = 60;
uint16_t echo_cells_3 = 127;

uint8_t echo_cells_1Melody = 32;
uint8_t echo_cells_2Melody = 60;
uint16_t echo_cells_3Melody = 127;

uint8_t note;
uint8_t noteMelody;
uint8_t noteDuration = 170;
uint8_t counter;

uint8_t Cmaj7[4]{
    47, 43, 40, 36};
uint8_t Dmin7[4]{
    48, 45, 41, 38};
uint8_t Emin7[4]{
    50, 47, 43, 40};
uint8_t Fmaj7[4]{
    52, 48, 45, 41};
uint8_t G7[4]{
    53, 50, 47, 43};
uint8_t Amin7[4]{
    55, 52, 48, 45};
uint8_t Bdim7[4]{
    57, 53, 50, 47};
uint8_t silence[4]{
    0, 0, 0, 0};

uint8_t arpeggioChoice; 

uint8_t arrpegioTable[8] = {
    0, 1, 2, 3, 4, 5, 6, 7};
    
uint8_t noteTable[9] = {
    0, 48, 50, 52, 53, 55, 57, 59, 60};

uint8_t mapArrpegio(uint16_t input)
{
  uint8_t value = (1023 - input) / (1024/30);
  return (arrpegioTable[value]);
}

uint8_t mapNote(uint16_t input)
{
  uint8_t value = (1023 - input) / (1024/30);
  return (noteTable[value]);
}

uint8_t *arpeggio;

Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin0(TRIANGLE_VALVE_2_2048_DATA);
Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin1(TRIANGLE_VALVE_2_2048_DATA);
Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin2(TRIANGLE_VALVE_2_2048_DATA);

Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin0Melody(TRIANGLE_VALVE_2_2048_DATA);
Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin1Melody(TRIANGLE_VALVE_2_2048_DATA);
Oscil <TRIANGLE_VALVE_2_2048_NUM_CELLS, AUDIO_RATE> aSin2Melody(TRIANGLE_VALVE_2_2048_DATA);

void setup(){
  
  // ARPEGGIATOR SECTION -----------------------------------------------------

  kDelay.set(echo_cells_1);
  kNoteChangeDelay.set(noteDuration);
  kFreq.setFreq(.33f);
  counter = 0;

  // MELODY SECTION ----------------------------------------------------------

  kDelayMelody.set(echo_cells_1Melody);
  
  // VARIUOS SECTION ---------------------------------------------------------

  Serial.begin(9600);
  startMozzi(CONTROL_RATE);
}

void updateControl(){
  
  // ARPEGGIATOR SECTION -----------------------------------------------------
  
  arpeggioChoice = mapArrpegio(mozziAnalogRead(CONTROL));
  
  switch(arpeggioChoice) {
    case 0:
      arpeggio = silence;
      break;
    case 1:
      arpeggio = Cmaj7;
      break;
    case 2:
      arpeggio = Dmin7;
      break;
    case 3:
      arpeggio = Emin7;
      break;
    case 4:
      arpeggio = Fmaj7;
      break;
    case 5:
      arpeggio = G7;
      break;
    case 6:
      arpeggio = Amin7;
      break;
    case 7:
      arpeggio = Bdim7;
      break;
    default:
      arpeggio = Cmaj7;
      break;
  }  
 
  if(kNoteChangeDelay.ready()){
    if (counter < 4) {
      note = arpeggio[counter];
      counter++;
      kNoteChangeDelay.start();
    } else {      
      counter = 0;     
    }    
  }

  // Echo for arpeggiator
  aSin0.setFreq(kDelay.next(mtof(note)));
  aSin1.setFreq(kDelay.read(echo_cells_2));
  aSin2.setFreq(kDelay.read(echo_cells_3));    
  
  // MELODY SECTION ----------------------------------------------------------
  
  //noteMelody = mapNote(mozziAnalogRead(MELODY));
  
  Serial.println(mozziAnalogRead(MELODY));
  
  waveMelody.setFreq(mtof(noteMelody));
  waveMelody2.setFreq(mtof(noteMelody));  
  
  // Echo for melody
  aSin0Melody.setFreq(kDelayMelody.next(mtof(noteMelody)));
  aSin1Melody.setFreq(kDelayMelody.read(echo_cells_2Melody));
  aSin2Melody.setFreq(kDelayMelody.read(echo_cells_3Melody));

  // VARIUOS SECTION ---------------------------------------------------------
  
   del_samps = 128+kFreq.next();    
}


int updateAudio(){
  
  // ARPEGGIATOR SECTION -----------------------------------------------------
  
  wave.setFreq(mtof(note));

  // MELODY SECTION ----------------------------------------------------------
  
  int asigMelody = ((int)waveMelody.next() + ((int)waveMelody2.next()>>1));

  // OUTPUT SECTION ----------------------------------------------------------

  return 5*(((int)wave.next()>>1) + (aSin0.next()>>1) + (aSin1.next()>>1) + (aSin2.next()>>2) +
            (asigMelody) + (aSin0Melody.next()>>1) + (aSin1Melody.next()>>2) + (aSin2Melody.next()>>4)) >>3;
}


void loop(){
  audioHook();
}



