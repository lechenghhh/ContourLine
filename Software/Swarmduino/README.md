# Swarmduino
### base on Arduino nano 328p
#### for ContourLine 2.1  
原作者: PeteHaughie  
https://github.com/PeteHaughie/MozziSketches/blob/main/Swarmduino  
修改者: lecheng  
说明: 该模块程序包含八个振荡器 每个振荡器有一定的失谐程度和周期性幅度调制;  
在原有程序基础上增加了多种波形切换(原版只有方波),八个振荡器调幅周期可控;

### 音频接口:  

0.      a0:Freq                   a1:Detune Span  
1.      a2:Detune Tremolo Freq    a3:Null
2.      d11:Null                  d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         F                       Freq                        频率调节                      暂时只能控制频率 无法控制音高
 2.         D                       DetuneSpan                  失谐间隔程度                   失谐的间隔
 3.         T                       Tremolo                     微调每个振荡器的颤音频率        微调
 4.         S                       TremoloShape                颤音波形选择                   8种波形
 5.         W                       WaveType                    波形                          8种波形                    
         

### 以下是原版说明:  

  Swarmduino  
  https://github.com/PeteHaughie/MozziSketches/blob/main/Swarmduino  
  ==========
  An 8 voice swarm square wave oscillator 'instrument'
  Replace the potentiometer on A1 with a softpot or similar for expressive playing potential
  
  For higher quality audio output use HIFI Mode
  https://sensorium.github.io/Mozzi/learn/output/#output-circuit-for-hifi-mode
  Thanks to Sound Simulator on YouTube for the idea
  https://www.youtube.com/watch?v=0H5wQOUrlIY&t=227s
  
Swarmduino
==========
一种8音群方波振荡器“仪器”
将A1上的电位计更换为软电位计或类似装置，以获得富有表现力的演奏潜力
  
要获得更高质量的音频输出，请使用HIFI模式
https://sensorium.github.io/Mozzi/learn/output/#output-hifi模式电路


//   #include <tables/square_analogue512_int8.h>SQUARE_ANALOGUE512_DATA 
// #include <tables/sin512_int8.h>SIN512_DATA
// #include <tables/saw_analogue512_int8.h>SAW_ANALOGUE512_DATA 
// #include <tables/waveshape1_softclip_int8.h>WAVESHAPE1_SOFTCLIP_DATA 
// #include <tables/triangle512_int8.h>TRIANGLE512_DATA 
// #include <tables/halfsin256_uint8.h>HALFSIN256_DATA 
// #include <tables/phasor256_int8.h>PHASOR256_DATA 
// #include <tables/cosphase256_int8.h>COSPHASE256_DATA 