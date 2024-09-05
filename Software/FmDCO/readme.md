2xOP FM DCO  
base on Arduino nano atmega168p
<a href="https://note.com/solder_state/n/nca6a1dec3921">https://note.com/solder_state/n/nca6a1dec3921</a>  
原作者: Hagiwo  
适配改进: lecheng  
六个接口对应功能:  
Voct Null  
FMFreq FMAmt  
Null AudioOut  

 0.         8位数码管显示字母        参数术语EN                   参数术语CN                    参数说明
 1.         1                       Pitch                       音高调节                      C1-A2
 2.         2                       FM Freq                     调频算子频率                  频率区间可以改进到更为精确 也可以计算倍频调制出较为和谐的声音
 3.         3                       FM Level                    调频算子幅度                  调频量
