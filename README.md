## ContourLine 
__Arduino DCO__  
__The Eurorack modular is based on the Arduino-Mozzi library__

`Welcome!` 这是一个开源的Eurorack数字振荡器 基于`arduino nano控制器`和`mozzi库`实现  
可以通过上下按钮来切换当前旋钮可调节的功能与参数列表  
当前功能通过8位数显来显示 当旋钮拧到旧的参数附近 才可以解锁当前参数的修改 数码管的亮度增加则表示已解锁当前参数  
gerber/bom文件与清单已经提供 如果不了解如何下单pcb板 可以到b站搜索ciiyte的教程了解如何去诸如嘉立创等厂商定制pcb板。  
画面展示  
焊接环节由于都是贴片元件 可能需要你有插件焊接的基础 使用镊子和电烙铁/焊台来完成  
程序烧录教程 mozzi library依赖说明 视频演示  
<img src="Hardware/img/面板效果图2.jpg" height=400 width=200>

### 模块参数:
电源插口: 10p  
+12v: 约25mA  
-12v: 约5mA  
宽度: 4HP  
深度: 30mm  
操作:  
1x Knob  
2x Button  
1x 8bitLed  
接口:  
1x VOCT In  
3xC V In  
1x Gate In  
1x Audio Out  

### 硬件设计说明：
使用arduino nano控制器，它可以直接读取0-5v的输入值，并且多达8个adc输入，符合一部分eurorack cv信号的范围而无需额外电路进行放大 使得pcb只需要一块主板和比较少的元件，就可以得到整个模块的主体  
设计使用按钮切换 单电位器一一调整参数 8位数显显示参数 可能使模块看起来不太像个电子乐器，更像一个工具插件，但是如果想要加入更多的参数与调制内容，也是完全兼容的，具有可拓展性  
不同的cv输入信号可以有多种电路结构来尽可能还原，比如对于低频控制信号、音频信号、voct信号、门信号等的电路设计就会有所不同

### 现有程序实现:
[Avr-wt-synth故障振荡器](Software/Avr-wt-synth/readme.md "Software/Avr-wt-synth/")  
[虚拟模拟振荡器](Software/AnalogWave/readme.md "Software/AnalogWave/")  
[可调制打击乐发生器](Software/DrumVoltrixion/readme.md "Software/DrumVoltrixion/")  
[和弦/加法/FM振荡器](Software/FmAddChordDCO/readme.md "Software/FmAddChordDCO/")  
[2算子FM振荡器`(入门)`](Software/FmDCO/readme.md "Software/FmDCO/")  
[波型渐变2算子FM振荡器 `(推荐)`](Software/FmWsWtDCO/readme.md "Software/FmWsWtDCO/")  
[波型渐变2算子FM振荡器`(lofi版)`](Software/FmWsWtDCOfor168p/readme.md "Software/FmWsWtDCOfor168p/")  
[4组打击乐发生器](Software/FourTrekPerc/readme.md "Software/FourTrekPerc/")  
[粒子合成器](Software/Granular/readme.md "Software/Granular/")  
[和谐音程振荡器](Software/HarmonicOSC/readme.md "Software/HarmonicOSC/")  
[风噪音发生器](Software/NoiseWindy/readme.md "Software/NoiseWindy/")  
[采样器`(4K)`](Software/Sampler/readme.md "Software/Sampler/")  
[失谐振荡器](Software/Swarmduino/readme.md "Software/Swarmduino/")  
[更多>>](https://github.com/lechenghhh/ContourLine/tree/master/Software "https://github.com/lechenghhh/ContourLine/tree/master/Software")

### 程序拓展说明：
程序依赖于`mozzi library` 强大的声音合成库
arduino nano控制器没有dac模块 是如何输出较高质量的音频的? 原理就是使用pwm+rc电路 跟据奈奎斯特采样定理只要采样速率足够高也是可以的  
主程序代码结构介绍:(以[波型渐变2算子FM振荡器](Software/FmWsWtDCO/readme.md "Software/FmWsWtDCO/readme.md")为例)  

    1.setup方法 这里主要用来启动控制引脚绑定 led引脚绑定 mozzi启动
    2.updateControl方法
    这里主要用于获取当前的控制信息 如当前菜单 当前旋钮参数 当前按钮状态 当前菜单对应的参数 cv参数等
    然后对以上信息做处理和计算 得到基本的合成参数
    3.updateAudio方法
    这里进行最终的合成算法 最终输出成pwm音频 注意：这里是实时音频请不要做复杂控制运算!
    4.loop方法 执行mozzi的audioHook方法 注意:这里不要进行修改!


<a href="https://github.com/lechenghhh/ContourLine/issues">与我讨论问题, 或增加更多创意>></a>
