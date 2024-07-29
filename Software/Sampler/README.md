# Sampler  
一个可以循环播放采样的采样器 可以实现一些故障或者粒子的效果  
### base on Arduino nano 328p
#### for ContourLine 2.2   
原作者: lecheng  

### 音频接口:  

0.      a0:Null                   a1:Freq  
1.      a2:PlayPosition           a3:Length
2.      d11:Trig                  d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         P                       PlayPosition                播放头位置                    
 2.         L                       Length                      播放长度                      
 3.         F                       Freq                        播放循环频率                  
 4.         C                       Circle                      是否循环                      默认开启
 4.         B                       BitCrush                    降采样                        默认不开启
 5.         S                       Select                      采样选择                      4种采样                    
         
### 将自己的采样转化成数据
https://github.com/gavD/arduino-drum-machine