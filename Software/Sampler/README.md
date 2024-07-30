# Sampler  
一个可以循环播放采样的采样器 可以实现一些故障或者粒子的效果  
### base on Arduino nano 328p
#### for ContourLine 2.2   
原作者: lecheng  

### 音频接口:  

0.      a0:Freq                   a1:Null
1.      a2:PlayPosition           a3:Length
2.      d11:Trig                  d9d10:AudioOut    

### 显示参数:  

 0.         8位数码管显示字母        参数名词EN                   参数名词CN                    参数说明
 1.         F                       Freq                        播放循环频率                  
 2.         P                       PlayPosition                播放头位置                    
 3.         L                       Length                      播放长度                      
 4.         C                       Cycle                       是否循环                      默认开启 如果关闭 则通过d11接口进行触发
 5.         B                       BitCrush                    降采样                        默认不开启
 6.         S                       Select                      采样选择                      4种采样                    
         
### 将自己的采样转化成数据


Loading a sample into the Sampler
--

You can load whatever samples you like into this box!

Any sample for conversion must be:
1. In headerless unsigned 8 bit raw format
2. Mono
3. Sample rate of 16384
4. Precisely 2048 samples long _(Audacity, for example, can be configured to show sample count for selections)_

From Audacity, it would look like this:

![img/correct-sample-from-audacity.jpg](img/correct-sample-from-audacity.jpg)

Export to "Other uncompressed files" like:

![img/export-from-audacity.jpg](img/export-from-audacity.jpg)

If you have a WAV, you can convert to RAW with ffmpeg, something like:

```bash
# Convert a WAV to a signed headerless 8 bit raw file (or output raw from your audio editor)
# let's convert a file called chirp.wav
FILE=chirp

ffmpeg -y -i ${FILE}.wav -f s8 -acodec pcm_s8 ${FILE}.raw
```

Now we can convert that to an 8 bit wavetable containing 2048 samples that the drum machine can use:

```bash
# Use char2mozzi.py to convert to a wavetable of the right length (2048)
# to a C header file that Mozzi can consue
python ../Mozzi/extras/python/char2mozzi.py ${FILE}.raw ${FILE}.h ${FILE} 16384
```
来源:  
https://github.com/gavD/arduino-drum-machine