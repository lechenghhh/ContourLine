/*  使用波形整形来修改音频信号频谱的示例
    使用Mozzi声库。

    演示WaveShaper（）、EventDelay（）、Smooth（）的使用，
    rand（）和定点数。

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
#include <Oscil.h>
#include <WaveShaper.h>
#include <EventDelay.h>
#include <mozzi_rand.h>
#include <mozzi_midi.h>
#include <Smooth.h>
#include <tables/sin2048_int8.h>
#include <tables/waveshape_chebyshev_3rd_256_int8.h>
#include <tables/waveshape_chebyshev_6th_256_int8.h>
#include <tables/waveshape_compress_512_to_488_int16.h>

// use: Oscil <table_size, update_rate> oscilName (wavetable), look in .h file of table #included above
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);    // sine wave sound source
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aGain1(SIN2048_DATA);  // to fade sine wave in and out before waveshaping
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> aGain2(SIN2048_DATA);  // to fade sine wave in and out before waveshaping

// Chebyshev polynomial curves, The nth curve produces the n+2th harmonic component.
WaveShaper<char> aCheby3rd(CHEBYSHEV_3RD_256_DATA);             // 5th harmonic
WaveShaper<char> aCheby6th(CHEBYSHEV_6TH_256_DATA);             // 8th harmonic
WaveShaper<int> aCompress(WAVESHAPE_COMPRESS_512_TO_488_DATA);  // to compress instead of dividing by 2 after adding signals

// for scheduling note changes
EventDelay kChangeNoteDelay;

// for random notes
Q8n0 octave_start_note = 42;
Q24n8 carrier_freq;  // unsigned long with 24 integer bits and 8 fractional bits

// smooth transitions between notes
Smooth<unsigned int> kSmoothFreq(0.85f);
int target_freq, smoothed_freq;

void setup() {
  startMozzi();         // :)
  randSeed();           // reseed the random generator for different results each time the sketch runs
  aSin.setFreq(110);    // set the frequency
  aGain1.setFreq(2.f);  // use a float for low frequencies, in setup it doesn't need to be fast
  aGain2.setFreq(.4f);
  kChangeNoteDelay.set(4000);  // note duration ms, within resolution of CONTROL_RATE
}

byte rndPentatonic() {
  byte note = rand((byte)5);
  switch (note) {
    case 0:
      note = 0;
      break;
    case 1:
      note = 3;
      break;
    case 2:
      note = 5;
      break;
    case 3:
      note = 7;
      break;
    case 4:
      note = 10;
      break;
  }
  return note;
}

void updateControl() {
  if (kChangeNoteDelay.ready()) {
    if (rand((byte)5) == 0) {  // about 1 in 5 or so
      // change octave to midi 24 or any of 3 octaves above
      octave_start_note = (rand((byte)4) * 12) + 36;
    }
    Q16n16 midi_note = Q8n0_to_Q16n16(octave_start_note + rndPentatonic());
    target_freq = Q16n16_to_Q16n0(Q16n16_mtof(midi_note));  // has to be 16 bits for Smooth
    kChangeNoteDelay.start();
  }
  smoothed_freq = kSmoothFreq.next(target_freq * 4);  //暂时放大target_freq以在低值时获得更好的int平滑 temporarily scale up target_freq to get better int smoothing at low values
  aSin.setFreq(smoothed_freq / 4);                    // 平滑后再缩小then scale it back down after it's smoothed
}

int updateAudio() {
  char asig0 = aSin.next();  // sine wave source
  // 用切比雪夫多项式曲线进行波形整形时，使两个信号渐隐以显示幅度的影响 make 2 signals fading in and out to show effect of amplitude when waveshaping with Chebyshev polynomial curves
  // 将信号偏移128以适应波形表查找的0-255范围 offset the signals by 128 to fit in the 0-255 range for the waveshaping table lookups
  byte asig1 = (byte)128 + ((asig0 * ((byte)128 + aGain1.next())) >> 8);  //lecheng: 这里进行调制 mozziAnalogRead()
  byte asig2 = (byte)128 + ((asig0 * ((byte)128 + aGain2.next())) >> 8);  //lecheng: 这里进行调制 mozziAnalogRead()
  // 得到波形信号 get the waveshaped signals
  char awaveshaped1 = aCheby3rd.next(asig1);
  char awaveshaped2 = aCheby6th.next(asig2);
  // 使用波形表将2个相加的8位信号压缩到-244到243的范围内 use a waveshaping table to squeeze 2 summed 8 bit signals into the range -244 to 243
  int awaveshaped3 = aCompress.next(256u + awaveshaped1 + awaveshaped2);
  return awaveshaped3;
}

void loop() {
  audioHook();  // required here
}