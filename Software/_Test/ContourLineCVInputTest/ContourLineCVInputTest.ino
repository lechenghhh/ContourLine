/*
  Uart output and input
*/

// 初始化串口波特率为11520:
void setup() {
  // 使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  Serial.begin(115200);
}

int mod_cc = 0;
int num = 9;
// the loop routine runs over and over again forever:0
void loop() {
  // Serial.println()用来串口打印信息，其自带换行，可不用在结尾添加\n
  // Serial.println("Hello warld!");
  // Serial.print()用来串口打印信息，其不带换行，可需要在结尾添加\n
  // Serial.print("Hello warld!\n");
  // delay(500);        // 延时500ms

  // if (digitalRead(12) == 1) {
  //   num++;
  // }

  //测试contourline1.0输入阻抗的代码
  int select = analogRead(4) >> 8;

  int analog = analogRead(select);
  int length = analog >> 2;
  Serial.print(select);
  Serial.print("-");
  Serial.print(analog);
  for (int i = 0; i < length; i++) {
    Serial.print("=");
  }
  Serial.print("\n");


  // num++;
  // // if (num % 10 == 0)
  // mod_cc++;
  // if (mod_cc > 255) mod_cc = 0;
  // analogWrite(3, mod_cc);
  // analogWrite(6, mod_cc);
  // analogWrite(5, mod_cc);
  // analogWrite(10, mod_cc);

  // analogWrite(3, analogRead(0)>>2);
}
