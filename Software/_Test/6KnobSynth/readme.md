OG: https://github.com/adelrune/Llammas

Llammas
Llammas is Literally Another Monophonic Mozzi Arduino Synth. This MIDI synthesizer has been developped with the Mozzi library, a sound synthesis library for Arduino.

Features
Llammas is a Monophonic synthesizer, with 3 oscillators (supporting different waveforms). It includes 2 LFO with flexible routing, 2 enveloppes (filter & amplitude), 3 operator FM synthesis and a multimode resonnant filter.

Supported filters
Highpass
Lowpass
Bendpass
Notch
Supported waveforms
Sine
Saw
Square
Triangle
Protocole
The synthesizer supports the MIDI standard and works with any MIDI controller. Theoretically. In fact, too much MIDI signals at once is badly handled, because of the Arduino's capacity.

Usage
The first potentiometer selects what the four others modify.

Mode 0
Amplitude modulation of osc 1 by lfo 1;
Note played by osc 1;
Osc 1 level;
Waveform of osc 1;
Mode 1
Amplitude modulation of osc 2 by lfo 1;
Note played by osc 2;
Osc 2 detune;
Osc 2 level;
Mode 2
Sinewave of osc 2;
Modulation of osc 3 amplitude;
Note played by osc 3;
Osc 3 level;
Mode 3
Waveform of osc 3;
Osc 3 detune;
Attack of amplitude enveloppe
Decay of amplitude enveloppe
Mode 4
Sustain of amplitude enveloppe;
Release of amplitude enveloppe;
Attack of filter enveloppe;
Decay of filter enveloppe;
Mode 5
Sustain of filter enveloppe;
Release of amplitude enveloppe;
LFO 1 frequency;
Waveform of LFO 1;
Mode 6
LFO 2 Frequency;
Waveform of LFO 2;
Filter type;
Filter cutoff;
Mode 7
Modulation of filter cutoff by LFO 1;
Modulation of filter cutoff by LFO 2;
Amount of glitch;
Modulation of global amplitude by LFO 2;
Licence
This synthesizer has been conceived by Guillaume Riou, Aude Forcione-Lambert & Nicolas Hurtubise and is distributed under the terms of the GNU General Public License v3. See the LICENSE file for more details.