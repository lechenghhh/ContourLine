# 2xOP FM 打击乐生成器
base on Arduino nano 328p for ContourLine 2.2
  
作者: SYNSO https://space.bilibili.com/522098852  
E-Mail: 1360751812@qq.com


### 音频接口:  

0.          a0:Pitch              a1:Decay  
1.          a2:Table              a3:FM
2.          d11:Trig              d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         P                       Pitch                       音高调节                      
 2.         D                       Decay                       延音调节                      
 3.         T                       Table                       波形选择                      四种波形：正弦波、锯齿波、棕噪音、白噪音
 4.         F                       FM                          FM频率                        内置FM算子的频率
 6.         A                       FM OP Amout                 算子幅度                      内置FM算子的幅度大小


#### *4种波形(由mozzi提供)为:  

whitenoise8192
brownnoise8192
sin1024
saw1024