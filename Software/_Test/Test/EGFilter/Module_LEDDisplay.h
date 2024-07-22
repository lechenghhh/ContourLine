/*
  一位共阳数码管显示代码
*/

bool CLed1[7] = { 0, 1, 1, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed2[7] = { 1, 0, 1, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed3[7] = { 1, 1, 0, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed4[7] = { 1, 1, 1, 0, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed5[7] = { 1, 1, 1, 1, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed6[7] = { 1, 1, 1, 1, 1, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool CLed7[7] = { 1, 1, 1, 1, 1, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool CLed8[7] = { 1, 1, 1, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚



void initLED(int led2, int led3, int led4, int led5, int led6, int led7, int led8) {
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
}

//显示led 传入bool数组
void displayLED(bool diplayGroup[]) {
  for (int i = 2; i <= 8; i++) {  //D2-D8引脚
    if (diplayGroup[i - 2]) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}

void displayTestPin(int pin) {
  digitalWrite(pin, LOW);
}