#include <MozziGuts.h>
#include <Oscil.h>                // oscillator template
#include <tables/sin2048_int8.h>  // sine table for oscillator

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

// use #define for CONTROL_RATE, not a constant
#define CONTROL_RATE 64  // Hz, powers of 2 are most reliable

void setup() {
  Serial.begin(9600);
  startMozzi(CONTROL_RATE);  // :)
  aSin.setFreq(440);         // set the frequency
}


void updateControl() {
  aSin.setFreq(analogRead(0));  // set the frequency
                                // put changing controls in here
}


int updateAudio() {
  return aSin.next();  // return an int signal centred around 0
}


void loop() {

  Serial.println(digitalRead(2));  //btn
  Serial.println(analogRead(0));   //knob
  Serial.println("----------------------------");

  audioHook();  // required here
}