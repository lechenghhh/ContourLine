## ContourLine 
__Arduino DCO__  
__The Eurorack modular is based on the Arduino-Mozzi library__

`Welcome!` 这是一个开源的Eurorack数字振荡器 基于arduino nano和mozzi库实现  
接口介绍:1xvoct 5个八度 1xWaveShaper CV Input 1x FM Freq CV 1x FM Amount CV 1x WaveType Gate(可以通过高电平触发波形切换功能) 1x Audio Output  
可以通过上下按钮来切换当前旋钮可调节的功能与参数列表  
当前功能通过8位数显来显示 当旋钮拧到旧的参数附近 才可以解锁当前参数的修改 数码管的亮度增加则表示已解锁当前参数  
gerber/bom文件与清单已经提供 如果不了解如何下单pcb板 可以到b站搜索ciiyte的教程了解如何去诸如嘉立创等厂商定制pcb板。  
画面展示  
焊接环节由于都是贴片元件 可能需要你有插件焊接的基础 使用镊子和焊台来完成  
程序烧录教程 mozzi library依赖说明 视频演示  
<img src="Hardware/img/面板效果图2.jpg" height=400 width=200>

### 硬件设计说明：
1.由于是使用arduino nano控制器，好处就是它可以直接读取0-5v的输入值 符合一部分eurorack cv的范围而无需额外电路进行放大 使得pcb板只需要两块和比较少的元件 就可以得到整个模块的主体  
arduino、atmega推出时间较长，也因此找到了基于该单片机控制器的丰富完善的依赖库，当前模块依赖的mozzi library具有丰富的算法波形调制等功能，相信在大家的开发下还可以变得更强  
不过atmega328p的ROM只有32k 能够实现的功能是有限的 但是对于一个4hp的模块来说 他已经足够强大  
该设计使用按钮切换 单电位器一一调整参数 8位数显显示参数 可能使模块看起来不太像个电子乐器，更像一个工具插件振荡器，但是好处就是如果想要加入更多的参数与调制内容，这套操作界面也是完全兼容的，具有很大的可拓展性。  
不同的cv输入信号可以有多种阻抗的电路结构来尽可能还原，比如对于低频控制信号、音频信号、voct信号、门信号等的电路设计就会有所不同。  

### 程序拓展说明：
`mozzi library`介绍 非常强大的声音合成依赖库
arduino nano没有dac模块 是如何输出较高质量的音频的? 原理就是使用pwm+rc电路 跟据奈奎斯特采样定理只要采样速率足够高也是可以的  
主程序代码结构介绍:(以[波型渐变2算子FM振荡器](Software/FmWsWtDCO/readme.md "Software/FmWsWtDCO/readme.md")为例)  

    1.setup方法 这里主要用来启动控制引脚绑定 led引脚绑定 mozzi启动
    2.updateControl方法
    这里主要用于获取当前的控制信息 如当前菜单 当前旋钮参数 当前按钮状态 当前菜单对应的参数 cv参数等
    然后对以上信息做处理和计算 得到基本的合成参数
    3.updateAudio方法
    这里进行最终的合成算法 最终输出成pwm音频 注意：这里是实时音频请不要做复杂控制运算!
    4.loop方法 执行mozzi的audioHook方法 注意:这里不要进行修改!

程序并不是十分完美 甚至可能存在bug 如果有明显的问题也可以留言区交流推动改进。所以也就当抛砖引玉 让大家了解arduino开发合成器的思路。

### 现有程序实现:
[波型渐变2算子FM振荡器 `(推荐!)`](Software/FmWsWtDCO/readme.md "Software/FmWsWtDCO/readme.md")  
[Avr-wt-synth故障振荡器](Software/Avr-wt-synth/readme.md "Software/Avr-wt-synth/readme.md")  
[和弦/加法/FM振荡器](Software/TripleDCOForCL/readme.md "Software/TripleDCOForCL/readme.md")  
[2算子FM振荡器](Software/FmDCO/readme.md "Software/FmDCO/readme.md")  

