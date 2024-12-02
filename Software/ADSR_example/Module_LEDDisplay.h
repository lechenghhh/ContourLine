/*
  一位共阳数码管显示代码
*/
bool Led_NULL[7] = { 1, 1, 1, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_0[7] = { 0, 1, 0, 0, 0, 0, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_1[7] = { 1, 1, 1, 1, 0, 0, 1 };     //coutourLine2.0 D2-D8引脚
bool Led_2[7] = { 1, 0, 0, 0, 1, 0, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_3[7] = { 1, 0, 1, 0, 0, 0, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_4[7] = { 0, 0, 1, 1, 0, 0, 1 };     //coutourLine2.0 D2-D8引脚
bool Led_5[7] = { 0, 0, 1, 0, 0, 1, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_6[7] = { 0, 0, 0, 0, 0, 1, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_7[7] = { 1, 1, 1, 1, 0, 0, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_8[7] = { 0, 0, 0, 0, 0, 0, 0 };     //coutourLine2.0 D2-D8引脚
bool Led_9[7] = { 0, 0, 1, 0, 0, 0, 0 };     //coutourLine2.0 D2-D8引脚

bool Led_A[7] = { 0, 0, 0, 1, 0, 0, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_B[7] = { 0, 0, 0, 0, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_C[7] = { 0, 1, 0, 0, 1, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_D[7] = { 1, 0, 0, 0, 0, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_E[7] = { 0, 0, 0, 0, 1, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_F[7] = { 0, 0, 0, 1, 1, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_G[7] = { 0, 1, 0, 0, 0, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_H[7] = { 0, 0, 0, 1, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_I[7] = { 0, 1, 0, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_J[7] = { 1, 1, 1, 0, 0, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_K[7] = { 0, 0, 0, 1, 0, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_L[7] = { 0, 1, 0, 0, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_M[7] = { 1, 1, 0, 1, 0, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_N[7] = { 1, 0, 0, 1, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_O[7] = { 1, 0, 0, 0, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_P[7] = { 0, 0, 0, 1, 1, 0, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_Q[7] = { 0, 0, 1, 1, 0, 0, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_R[7] = { 1, 0, 0, 1, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_S[7] = { 0, 0, 1, 0, 0, 1, 0 };  //coutourLine2.0 D2-D8引脚
bool Led_T[7] = { 0, 0, 0, 0, 1, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_U[7] = { 1, 1, 0, 0, 0, 1, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_V[7] = { 0, 0, 1, 0, 1, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_W[7] = { 0, 1, 1, 0, 1, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_X[7] = { 0, 1, 0, 1, 0, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_Y[7] = { 0, 0, 1, 0, 0, 0, 1 };  //coutourLine2.0 D2-D8引脚
bool Led_Z[7] = { 1, 0, 1, 1, 0, 0, 0 };  //coutourLine2.0 D2-D8引脚

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