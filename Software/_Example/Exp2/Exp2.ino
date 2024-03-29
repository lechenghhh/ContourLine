
#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator

#include <Adafruit_GFX.h>//图形库
#include <Adafruit_SSD1306.h>//图形库

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

const char INPUT_PIN = 0; // set the input for the knob to analog pin 0

// to convey the volume level from updateControl() to updateAudio()
byte volume;


void setup(){
  Serial.begin(9600); // for Teensy 3.1, beware printout can cause glitches
  // Serial.begin(115200); // set up the Serial output so we can look at the piezo values // set up the Serial output so we can look at the input values
  aSin.setFreq(440);
  startMozzi(); // :))
}


void updateControl(){
  // read the variable resistor for volume
  // int sensor_value = mozziAnalogRead(INPUT_PIN); // value is 0-1023
  int sensor_value = 0; // value is 0-1023

  // map it to an 8 bit range for efficient calculations in updateAudio
  volume = map(sensor_value, 0, 1023, 0, 255);

  // print the value to the Serial monitor for debugging
  Serial.print("volume = ");
  Serial.println((int)volume);
}


int updateAudio(){
  return ((int)aSin.next() * volume)>>8;// 乘以 8 位值后移回范围
}

void loop(){
  audioHook(); // required here
}