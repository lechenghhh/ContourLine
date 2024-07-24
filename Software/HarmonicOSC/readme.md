# Harmonic oscillator
### base on Arduino nano 328p
#### for ContourLine 2.1  
作者: lecheng  
说明: 该振荡器用于模拟一些drone生成器的工作流 简单的振荡器 组合出和谐的音程关系 可以通过外部cv来控制每个音程振荡器的VCA  
演示: https://www.bilibili.com/video/BV17y411B729  

### 音频接口:  

0.      a0:RootNote VOCT          a1:Note1 VCA  
1.      a2:Note2 VCA              a3:Note3 VCA
2.      d11:Wave Type Rand Trig   d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         0                       RootNote Pitch              根音音高                      C1-A2
 2.         1                       RootNote Amp                根音音量
 3.         2                       Note1 Interval              第一个音程               
 4.         3                       Note1 Amp                   第一个音量                    
 5.         4                       Note2 Interval              第二个音程                    
 6.         5                       Note2 Amp                   第二个音量                    
 7.         6                       Note3 Interval              第三个音程                    
 8.         7                       Note3 Amp                   第三个音量        
 9.         T                       Wave Type                   波形类型 八种波形              0-7每个波形都一致 8-15八种波形组合      
                     
