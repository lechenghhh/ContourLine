/*
  电位器 按钮 控制功能
*/

#define KNOB_MAX_RANGE 1024  //旋钮最大值
// #define FUNCTION_LENGTH 10   //总菜单数
int KNOB;
int BTN1;
int BTN2;
int KNOB_CHANGE_RANGE;
int DEF_LV;
// int position = 0;  //modual ctrl position 当前菜单的位置

int btnHover = 0;    //按下事件判断
int btnTime = 0;     //长按事件判断
int knobEnable = 0;  //旋钮事件允许

/*
  konbPin           旋钮模拟引脚
  konbChangeRange   旋钮启用改变的差值
  btn1Pin           按钮1数字引脚
  btn2Pin           按钮2数字引脚
  default_level     按钮默认电平
*/
void initCtrl(int konbPin, int konbChangeRange, int btn1Pin, int btn2Pin, int default_level) {
  KNOB = konbPin;
  KNOB_CHANGE_RANGE = konbChangeRange;
  BTN1 = btn1Pin;
  BTN2 = btn2Pin;
  DEF_LV = default_level;
  if (DEF_LV == 1) {
    // pinMode(BTN2, INPUT_PULLUP);//D13引脚会因为无法上拉电阻而导致无法默认高电平 因此手动设置
    if (BTN1 == 13) digitalWrite(BTN1, HIGH);
    else pinMode(BTN1, INPUT_PULLUP);
    if (BTN2 == 13) digitalWrite(BTN2, HIGH);
    else pinMode(BTN2, INPUT_PULLUP);
  } else {
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
  }
}

/*
  传入菜单下标
  返回经过控制逻辑处理后的下标
*/
int getPostition(int position, int functionLength) {
  // Serial.println(mozziAnalogRead(KNOB));  //knob
  // Serial.println(digitalRead(BTN1));      //btn1
  // Serial.println(digitalRead(BTN2));      //btn2

  // Serial.println(btnHover);   //btn2
  // Serial.println(btnTime);    //btn2


  //按下事件
  if (digitalRead(BTN1) != DEF_LV) {
    btnHover = 1;
    btnTime++;
  }
  //抬起事件
  if (digitalRead(BTN1) == DEF_LV && btnHover == 1) {
    //长按按钮事件
    if (btnTime > 100) {
      position = 0;
      Serial.println("btn long");  //knob
    }
    //短按按钮事件
    else {
      if (functionLength - 1 <= position) {
        position = -1;
      }
      position++;
      Serial.println("btn short");  //knob
    }
    btnHover = 0;
    btnTime = 0;
    knobEnable = 0;
    Serial.println(knobEnable);  //btn2
  }
  return position;
}

int getParam(int old_param) {
  // Serial.print(knobEnable);   //btn2
  // Serial.println(old_param);  //btn2

  //旋钮事件
  int knob_dec = mozziAnalogRead(KNOB) - old_param;  //检测旋钮进入原参数范围
  if (-KNOB_CHANGE_RANGE < knob_dec && knob_dec < KNOB_CHANGE_RANGE) {
    knobEnable = 1;
  }
  if (knobEnable == 1) {  //进入范围后 则可以调节当前参数
    old_param = mozziAnalogRead(KNOB);
  }
  return old_param;
}