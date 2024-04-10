# 2xOP FM WaveShape DCO 
### 该版本是为atmega168p设计的 内存占用较小 有lofi质感
### base on Arduino nano 168p
#### for ContourLine 2.1  
作者: lecheng  

### 音频接口:  

0.      a0:Voct         a1:Shaper  
1.      a2:FMFreq       a3:FMAmt  
2.      d11:WaveTrig    d9d10:AudioOut    

### 显示参数:  

 0.         无字母显示        参数名词EN                   参数名词CN                    参数说明
 1.         -                       Pitch                       音高调节                      C1-A2
 2.         -                       Range                       震荡范围                      调整振荡器的震荡范围 4种模式 0.1-10hz 30hz-150hz 60hz-300hz 120-600hz
 3.         -                       Wave Shape Position         波形渐变量                    单一算法的渐变(可能有bug 变成两种波形叠加量)
 4.         -                       FM OP Freq                  算子频率                      频率区间可以改进到更为精确 也可以通过计算倍频调制出较为和谐的声音
 5.         -                       FM OP Amout                 算子幅度                      调频幅度
 6.         -                       WaveType                    波型选择                      1-16 8种波形
