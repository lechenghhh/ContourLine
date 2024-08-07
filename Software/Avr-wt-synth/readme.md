程序改造自<a href="https://github.com/stosem/avr-wt-synth">https://github.com/stosem/avr-wt-synth<a/>  
avr-wt-synth (Glitch Synth)  
原作者: stosem  
程序适配: lecheng  
改版演示：https://www.bilibili.com/video/BV1TZ421M7dw

### 修改适配:  

1.原版使用d3 d4来显示debug模式与trig触发led发光提示 已经弃用  
2.原版没有手动调整音调的能力,改进版增加了小幅度调整音高的能力  
3.debug模式已经取消  

### 音频接口:  

0.          a0:Voct                      a1:Modulation   
1.          a2:Obertone                  a3:FM   
2.          d11:Trig(有触发才有声音)      d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   *请搜索原作者文档可以找到参数的含义
 1.         P                       Pitch                     
 2.         W                       WaveTable                     
 3.         M                       Modulation        
 4.         O                       Obertone      
 5.         G                       Glitch         

  
##  
### (以下是原作者文档转载) 
# Info

Eurorack Wavetable-based VCO. Atari Glitch. DIY

## Atari Glitch Synthesizer
This is the VCO (voltage controlled oscillator) audio synthesizer module for the Eurorack Modular systems. 
It's based on the Wavetable and FM synthesis using Arduino Mozzi library. 
See the basic circuit diagram in the `cad` folder.

## Interface

We have 4 main potentiometers B100K in order to set Sytentesizer's parameters:
* Wavetable - choose a wavetable 
* Modulation - set modulation depth 
* Obertone - set amount of overtone sound 
* Glitch - FM operator amount 

The same parameters can be set using CV inputs on 3.5mm Jacks. There are several additional CV inputs: 
* Trigger - main gate signal (1-5V) to initiate (trigger) a note 
* 1V/OCT - one volt per octave - the pitch of the note to play 

There is one audio output, also on 3.5mm jack socket: 
* OUT - audio out 

We have also one Switch to choose the Synthesizer mode between two possible modes: 
* Standard FM/Wavetable synth. You can set any parameters using potentiometers or CV signals (higher priority) 
* Random synthesizer. In this mode, all parameters are generated randomly on each step (trigger). Potentiometers now are limiting the maximum possible value of randomly generated values. Set a potentiometer to 0 (zero) if you want to unlimit a random value range. 

## Board
The Synthesizer consists of two boards. 
1. The Interface board, where all potentiometers and 3.5mm jack ports are located
2. The main logic board

All schemes are in the cad directory.

## Demo

Sound demo:

https://www.youtube.com/watch?v=aY03wGdam5E

# User manual

## Switch

* Random On/Off random feature.

If random feature is On, the new sound will be generated at each trigger CV signal

## Knobs

You can switch any knob to 0 value by turning it clockwise till the click.

| Parameter name | Description | Normal mode | Random mode |
| --- | --- | --- | --- |
| WAVE | WaveTable select | Select WaveTable from 0 to max available (MAX\_WT) | Limit Random Wavetable. 0=ALL |
| OBER | Obertone freq | Set Obertone Freq | Limit Obertone Freq. 0=ALL |
| GLITCH | FM intensity | Set FM intensity | Limit FM intensity. 0=ALL |
| MOD | Modulation Rate | Set wah-wah effect | Limit Mod ratio. 0=ALL |

## CVs

CVs have higher priority than knobs

| Parameter name | Description | Normal mode | Random mode |
| --- | --- | --- | --- |
| TRIG | Trigger CV | Make a sound | Generate a random sound |
| V/OCT | Pitch CV | Set pitch (will be quantized to a note) | Set pitch. 0 or no signal = Random pitch |
| GLCH | FM intensity | Set FM intensity | - |
| OBER | Obertone freq | Set obertone freq | - |
| MOD | Modulation Rate | Set wah-wah effect | - |

## Notes

Current range is 5 octaves from C1 to C6

## WaveTables

1. HSN - Half Sinus
2. SIN - Full Sinus
3. SQR - Square
4. SAW - Sawtooth
5. TRI - Triangle

# Developers

## Tips

* Do not use digital pins 11,12,13 if you want to use MOSI/MISO Atmega programmer (especially with capacitors)
* Do not use digital pins 9,10. They are used by Mozzi sound generator
* Change OpIntensityMod WaveTable if you want to get creazy sound
* Use DEBUG\_INPUT to check CVs and knobs. But it's not a good idea to use the Serial messages with Mozzi. Don't forget to undefine debugs.

## Known bugs

* AutoMap doesn't work till it get the maximum value from CV/Knob. It returns always maximum. We can't replace it with map() because it's very slow. Fixed by pushing max value first.
* map() and AutoMap works not so good on the minimum value. For example 0 and 15 - are the same values for it. Therefore C0 and C#0 - are the same notes. Fixed by if-then-hack.
