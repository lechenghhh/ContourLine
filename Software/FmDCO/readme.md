2xOP FM DCO  
base on Arduino nano atmega168p
<a href="https://note.com/solder_state/n/nca6a1dec3921">https://note.com/solder_state/n/nca6a1dec3921</a>  
原作者: Hagiwo  
适配改进: lecheng  
六个接口对应功能:  
Voct EGShape(同时调制Decay&Sustain&Release)  
FMFreq FMLevel  
EGTrig AudioOut  

 0.         8位数码管显示字母        参数术语EN                   参数术语CN                    参数说明
 1.         P                       Pitch                       音高调节                      音偏移量C1-A2
 1.         O                       Octave                      八度调节                      0-7 0时为lfo模式 1-7共为c0-c7
 3.         F                       FM Freq                     调频算子频率                  频率区间可以改进到更为精确 也可以计算倍频调制出较为和谐的声音
 4.         L                       FM Level                    调频算子幅度                  调频量
 5.         A                       Attack                      算子包络起音                   时长
 6.         D                       Decay                       算子包络衰减                   时长
 7.         S                       Sustain                     算子包络延音                   时长(其实是个四段包络因此延音的逻辑有所不同)
 8.         R                       Release                     算子包络释放                   时长
 9.         W                       WaveType                    波形                          约16种波形
