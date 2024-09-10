# 2xOP FM WaveShape DCO  6HP
### base on Arduino nano 328p
#### for ContourLine 2.1  
作者: lecheng  
演示：https://www.bilibili.com/video/BV1Lq421P76q

### 音频接口:  

0.  a1:Shaper  	      a2:FMFreq  	          a3:FMAmt  
1.	a5:Freq		        a6:AMP		            a7:BitCrush
2.	a0:Voct         	d11:WaveTrig        	d9d10:AudioOut

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         P                       Pitch                       音高调节                      C1-A2
 2.         R                       Range                       震荡范围                      调整振荡器的震荡范围 4种模式 0.1-10hz 30hz-150hz 60hz-300hz 120-600hz
 3.         G                       Wave Shape Gradient         波形渐变算法选择               8种算法*
 4.         S                       Wave Shape Position         波形渐变量                    根据算法的选择进行渐变
 5.         F                       FM OP Freq                  算子频率                      频率区间可以改进到更为精确 也可以通过计算倍频调制出较为和谐的声音
 6.         A                       FM OP Amout                 算子幅度                      调频幅度
 7.         T                       WaveType                    波型选择                      1-16 16种波形*含噪音
 8.         C                       Wave Type Change Position   波型切换一次的序号量           0-7 当Range震荡范围处于0.1-10hz挡位时 该功能用于Reset震荡器起点(rst还有问题 等待修复)

#### *8种算法(由mozzi提供)为:  
WAVESHAPE_TANH     
WAVESHAPE_SIGMOID       
WAVESHAPE1_SOFTCLIP     
WAVESHAPE2_SOFTERCLIP  
CHEBYSHEV_3RD_256  
CHEBYSHEV_4TH_256       
CHEBYSHEV_5TH_256       
CHEBYSHEV_6TH_256  
#### *16种波形(由mozzi提供)为:  
SIN512
WAVESHAPE_TANH          
SQUARE_ANALOGUE512      
WAVESHAPE1_SOFTCLIP  
WAVESHAPE_SIGMOID  
SQUARE_NO_ALIAS512      
SAW256     
SAW_ANALOGUE512  
HALFSINWINDOW512   
CHEBYSHEV_5TH_256       
PHASOR256  
UPHASOR256  
HALFSIN256         
COSPHASE256
COS256     
WHITENOISE8192_DATA
 