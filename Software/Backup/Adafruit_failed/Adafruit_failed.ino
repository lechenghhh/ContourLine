#include <Adafruit_GFX.h>      //图形库
#include <Adafruit_SSD1306.h>  //图形库

// #include <MozziGuts.h>
// #include <Oscil.h>                // oscillator template
// #include <tables/sin2048_int8.h>  // sine table for oscillator

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define KNOB_CHANGE_RANGE 100  //旋钮若改变在范围内才允许改变当前参数
#define KNOB_MAX_RANGE 1024    //旋钮最大值
#define FUNCTION_LENGTH 10     //总菜单数

String function[FUNCTION_LENGTH] = { "Wave", "Shape", "Pitch", "Vol", "LPF", "Reso.", "Attk.", "Decay", "Sus.", "Rel." };  // 给部分数组元素赋值
int value[FUNCTION_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int position = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);  //ssd1306显示
// Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA);

void setup() {
  Serial.begin(9600);  // 使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;
  }
  pinMode(2, INPUT_PULLUP);  //将2引脚自动上拉电阻


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.println(F("Hello !"));
  display.setCursor(10, 45);
  display.println(F("Envscape Synth )))"));
  display.display();
  delay(5000);
  display.clearDisplay();
  display.setTextSize(1);
  
  // aSin.setFreq(440);
  // startMozzi(); // :))
}

void loop() {

  // Serial.println(digitalRead(2));  //btn
  // Serial.println(analogRead(0));   //knob

  Serial.println("----------------------------");

  ctrl();
  screenDisplay();
  delay(50);  // 循环间隔
}


int btnHover = 0;    //按下事件判断
int btnTime = 0;     //长按事件判断
int knobEnable = 0;  //旋钮事件允许

void ctrl() {
  //按下事件
  if (digitalRead(2) == 0) {
    btnHover = 1;
    btnTime++;
  }
  //抬起事件
  if (digitalRead(2) == 1 && btnHover == 1) {
    //长按按钮事件
    if (btnTime > 5) {
      position = 0;
    }
    //短按按钮事件
    else {
      if (FUNCTION_LENGTH - 1 <= position) {
        position = -1;
      }
      position++;
    }
    btnHover = 0;
    btnTime = 0;
    knobEnable = 0;
  }

  //旋钮事件
  int knob_dec = analogRead(0) - value[position];  //检测旋钮进入原参数范围
  if (-KNOB_CHANGE_RANGE < knob_dec && knob_dec < KNOB_CHANGE_RANGE) {
    knobEnable = 1;
  }
  if (knobEnable == 1) {  //进入范围后 则可以调节当前参数
    value[position] = analogRead(0);
  }
}

//屏幕展示功能
void screenDisplay() {
  //内部参数展示
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Envscape Synth");
  display.setCursor(90, 0);
  display.println(position);
  display.setCursor(0, 16);
  display.println("btn=");  //参数展示
  display.setCursor(30, 16);
  display.println(digitalRead(2));
  display.setCursor(50, 16);
  display.println("knob=");
  display.setCursor(90, 16);
  display.println(analogRead(0));

  //菜单显示
  display.setCursor(0, 40);
  display.setTextSize(2);
  display.print(function[position] + " " + value[position]);

  display.display();
}
