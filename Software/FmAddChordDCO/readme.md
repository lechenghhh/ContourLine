程序改造自 <a href="https://github.com/Testbild-synth/HAGIWO_arduino_oscillator">HAGIWO_arduino_oscillator<a/>  
原作者: Testbild-synth  
程序适配: lecheng  
改版演示：https://www.bilibili.com/video/BV1zy411h7ez

### 音频接口:  

0.          a0:Voct             a1:P1  
1.          a2:P2               a3:Mode(Deprecated)
2.          d11:null            d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         F                       Freq                        频率调节                      C1-A2
 2.         1                       P1                          参数1调节                      
 3.         2                       P2                          参数2调节               
 4.         M                       Mode                        模式选择 FM/ADD/Chord                    


# HAGIWO 019/022/023 Triple Oscillator (FM/CHORD/ADDITIVE)
<img src="images/in_rack.jpg" width="40%" height="40%">
4HP through hole version of the HAGIWO FM/additive/chord oscillator with mode switch and gain input.

This module is basically my PCB version of the HAGIWO osc, heavily inspired by [luis lutz version](https://github.com/luislutz/Arduino-VDCO).
The main difference is since I could not get his code to work without major audio problems, I rewrote the entire thing myself.

The module combines three different oscillators from HAGIWO,  with a mode switch (and mode cv input) to switch between them.
Additionally, it has a gain input that normals so that it is only used when you plug in a jack.

### STATUS: CONFIRMED WORKING. Took me a long time to figure out the code, but now everything works as intended (I think, if you encounter any problems let me know).

## Hardware and PCB
<img src="images/panel.JPG" width="10%" height="10%"><img src="images/front_pcb.JPG" width="12%" height="12%"><img src="images/main_pcb.JPG" width="13%" height="13%">

You can find the schematic and BOM in the root folder.
For the potentiometers, any value between 1k and 100k should be fine.

The module has three PCBs: a panel PCB, a main PCB and a controls pcb.
You can order them on any common PCB manufacturing service, I used JLCPCB. For the circuit PCBs, standard settings should be fine.
For the panel, since it has copper exposed, make sure to choose a lead free surface finish (LeadFree HASL, ENIG) and/or spraypaint your panels so that you don't get lead on your hands.

Assembly is pretty straight forward. Since it is so crowded, make sure to do the resistors first before the headers so you have more space to work.

Also make sure to put the headers connecting both PCBs in the correct direction so that the arduino faces to the back.

The only thing there is to tune is a trimpot that sets output gain (as arduino is 0-5V output and I wanted to be able to make mine louder). Tune it to your liking.

<img src="images/both_parts.jpg" width="40%" height="40%">
<img src="images/side.jpg" width="40%" height="40%">

