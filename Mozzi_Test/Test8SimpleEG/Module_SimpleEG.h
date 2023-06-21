#include <avr/io.h>  //for fast PWM


const static byte curve[128] PROGMEM = { 0, 4, 7, 10, 14, 17, 21, 24, 27, 31, 34, 37, 40, 43, 46, 49, 52, 55, 58, 61, 64, 67, 70, 73, 76, 78, 81, 84, 87, 89, 92, 95, 97, 100, 102, 105, 107, 110, 112, 115, 117, 119, 122, 124, 126, 128, 131, 133, 135, 137, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 166, 168, 169, 171, 173, 175, 177, 178, 180, 182, 184, 185, 187, 189, 190, 192, 194, 195, 197, 198, 200, 202, 203, 205, 206, 208, 209, 210, 212, 213, 215, 216, 217, 219, 220, 221, 223, 224, 225, 227, 228, 229, 230, 232, 233, 234, 235, 237, 238, 239, 240, 241, 242, 243, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255 };

int TRI_PIN = 0;
int OUT_PIN = 0;

int egState;  //egState -- 0 is not triggerd , 1 is attack duration , 2 is release duration.
bool old_gate, new_gate;
int egProgress = 0;       //CH1 count
int last_out1, out1 = 0;  //如果在释放时间重新触发，CV电压将从线路中间重新启动，而不是从零开始。
long timer1 = 0;
void initEG(int triggerPin, int egOutPin) {
  TRI_PIN = triggerPin;
  OUT_PIN = egOutPin;
  pinMode(TRI_PIN, INPUT);   //C button
  pinMode(OUT_PIN, OUTPUT);  //accelx
  timer1 = micros();
  TCCR1B &= B11111000;  //fast pwm setting
  TCCR1B |= B00000001;  //fast pwm setting
  delay(50);
}

void generateEG(int atk1, int rel1, int lev1) {
  int atk2, rel2, lev2;
  old_gate = new_gate;
  new_gate = digitalRead(TRI_PIN);  // trig input

  if (old_gate == 0 && new_gate == 1) {  // if trigger
    if (egState == 2) {
      last_out1 = out1;
    } else if (egState != 2) {
      last_out1 = 0;
    }
    egProgress = 0;  //wave table count reset
    egState = 1;     //start attack duration
    atk2 = atk1 * 12;
    rel2 = rel1 * 16;
    lev2 = lev1 / 4;
  }

  else if (egState == egProgress && egProgress > 127) {  //attack duration end
    egState = 2;                                         //start release duration
  } else if (egState == 2 && egProgress > 255) {         //release duration end
    egState = 0;
  }

  //----------------------CV output buffer CH1-------------------------
  if (egState == 1) {  //attack
    if (atk1 <= 32) {  //Minimize atk time
      egProgress = 128;
      egState = 2;
    }
    out1 = (pgm_read_byte(&(curve[egProgress])));  //store output voltage value存储输出电压值
    if (timer1 + atk1 <= micros()) {
      egProgress++;
      timer1 = micros();
    }
  } else if (egState == 2) {                                   //release
    out1 = 255 - (pgm_read_byte(&(curve[egProgress - 128])));  //store output voltage value
    if (timer1 + rel1 <= micros()) {
      egProgress++;
      timer1 = micros();
    }
  }

  //CV out //PWM_OUT();
  //PWM duty setting
  if (egState == 1) {
    out1 = map(out1, 0, 255, last_out1, lev1);
  } else if (egState == 2) {
    out1 = map(out1, 0, 255, 0, lev1);
  }
  analogWrite(OUT_PIN, out1);  //PWM output
}
