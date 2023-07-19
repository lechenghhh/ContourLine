#define ENCODER_OPTIMIZE_INTERRUPTS  //エンコーダノイズ対策 编码器噪声对策
#include <Wire.h>
#include <Encoder.h>


//CV入出力用定数
float CV_IN = 512;      //CV_IN*60/1024の値。後にroundで四捨五入する
float old_CV_IN = 512;  //
int CV_INr = 512;       //CV_INを四捨五入した値
int CV_INh = 10;        //閾値の判定結果
int i = 0;              //用于缩放搜索
long old_CV_OUT = 0;

//slide設定用定数
int slide_CV = 0;    //slide_knobの読取値
int slide_time = 1;  //タイマー周期を掛けた値が実際のslide time。カウントはこの値まで増え続ける。
int scale = 0;
byte slide = 0;     //0=OFF,1=ON
byte slide_sw = 0;  //0=EXT,1=ON
int j = 0;

//illumination mode
byte slide_IN = 0;  //突入触发条件之一 突入トリガー条件のひとつ
int k = 0;          //illumination変調用
int illR = 0;
int illG = 100;
int illB = 200;

//LED用
int now_R = 100;
int now_G = 100;
int now_B = 100;

//ロータリーエンコーダ設定
Encoder myEnc(2, 3);       //旋转编码器库用
float oldPosition = -999;  //旋转编码器库用
float newPosition = -999;


//--------------------scale list---------------------------------
//0=major
const static word DAC_LSB_maj[61] PROGMEM = {
  0, 137, 205, 341, 478, 546, 683, 819, 956, 1024, 1161, 1297, 1365, 1502, 1638, 1775, 1843, 1980, 2116, 2185, 2321, 2458, 2594, 2662, 2799, 2935, 3004, 3140, 3277, 3413, 3482, 3618, 3755, 3823, 3959, 4095
};

const static word CVIN_th_maj[62] PROGMEM = {
  0, 15, 44, 73, 102, 131, 160, 189, 218, 247, 276, 305, 334, 363, 392, 421, 450, 479, 508, 537, 566, 595, 624, 653, 682, 711, 740, 769, 798, 827, 856, 885, 914, 943, 972, 1001, 1024
};

//1=major_pentatonic
const static word DAC_LSB_mp[61] PROGMEM = {
  0, 205, 341, 478, 683, 819, 1024, 1161, 1297, 1502, 1638, 1843, 1980, 2116, 2321, 2458, 2662, 2799, 2935, 3140, 3277, 3482, 3618, 3755, 3959, 4095
};

const static word CVIN_th_mp[62] PROGMEM = {
  0, 20, 61, 102, 143, 184, 225, 266, 307, 348, 389, 430, 471, 512, 553, 594, 635, 676, 717, 758, 799, 840, 881, 922, 963, 1004, 1024
};

//2=major_pentatonic+7
const static word DAC_LSB_mp7[61] PROGMEM = {
  0, 137, 205, 341, 478, 683, 819, 956, 1024, 1161, 1297, 1502, 1638, 1775, 1843, 1980, 2116, 2321, 2458, 2594, 2662, 2799, 2935, 3140, 3277, 3413, 3482, 3618, 3755, 3959, 4095
};

const static word CVIN_th_mp7[62] PROGMEM = {
  0, 17, 51, 85, 119, 153, 187, 221, 255, 289, 323, 357, 391, 425, 459, 493, 527, 561, 595, 629, 663, 697, 731, 765, 799, 833, 867, 901, 935, 969, 1003, 1024
};

//3=harmonic_minor
const static word DAC_LSB_hm[61] PROGMEM = {
  68, 137, 205, 341, 410, 546, 683, 887, 956, 1024, 1161, 1229, 1365, 1502, 1707, 1775, 1843, 1980, 2048, 2185, 2321, 2526, 2594, 2662, 2799, 2867, 3004, 3140, 3345, 3413, 3482, 3618, 3686, 3823, 3959
};

const static word CVIN_th_hm[62] PROGMEM = {
  0, 15, 45, 75, 105, 135, 165, 195, 225, 255, 285, 315, 345, 375, 405, 435, 465, 495, 525, 555, 585, 615, 645, 675, 705, 735, 765, 795, 825, 855, 885, 915, 945, 975, 1005, 1024
};

//4=monor_pentatonic
const static word DAC_LSB_minp[61] PROGMEM = {
  205, 341, 410, 683, 751, 1024, 1161, 1229, 1502, 1570, 1843, 1980, 2048, 2321, 2389, 2662, 2799, 2867, 3140, 3209, 3482, 3618, 3686, 3959, 4028
};

const static word CVIN_th_minp[62] PROGMEM = {
  0, 21, 64, 107, 150, 193, 236, 279, 322, 365, 408, 451, 494, 537, 580, 623, 666, 709, 752, 795, 838, 881, 924, 967, 1010, 1024
};


//5=chromatic
const static word DAC_LSB_chr[61] PROGMEM = {
  0, 68, 137, 205, 273, 341, 410, 478, 546, 614, 683, 751, 819, 887, 956, 1024, 1092, 1161, 1229, 1297, 1365, 1434, 1502, 1570, 1638, 1707, 1775, 1843, 1911, 1980, 2048, 2116, 2185, 2253, 2321, 2389, 2458, 2526, 2594, 2662, 2731, 2799, 2867, 2935, 3004, 3072, 3140, 3209, 3277, 3345, 3413, 3482, 3550, 3618, 3686, 3755, 3823, 3891, 3959, 4028, 4095
};

const static word CVIN_th_chr[62] PROGMEM = {
  0, 9, 26, 43, 60, 77, 94, 111, 128, 145, 162, 179, 196, 213, 230, 247, 264, 281, 298, 315, 332, 349, 366, 383, 400, 417, 434, 451, 468, 485, 502, 519, 536, 553, 570, 587, 604, 621, 638, 655, 672, 689, 706, 723, 740, 757, 774, 791, 808, 825, 842, 859, 876, 893, 910, 927, 944, 961, 978, 995, 1012, 1024
};

//6=octave
const static word DAC_LSB_oct[61] PROGMEM = {
  0, 819, 1638, 2458, 3277, 4095
};

const static word CVIN_th_oct[62] PROGMEM = {
  0, 102, 307, 512, 717, 922, 1024
};

void setup() {
  pinMode(9, OUTPUT);        //Color_LED_R
  pinMode(10, OUTPUT);       //Color_LED_G
  pinMode(11, OUTPUT);       //Color_LED_B
  pinMode(5, OUTPUT);        //gate_out
  pinMode(6, INPUT);         //slide_in
  pinMode(7, INPUT_PULLUP);  //slide_sw

  //開発用通信設定
  Serial.begin(115200);

  //DAC I2C通信
  Wire.begin();
}


void loop() {
  old_CV_IN = CV_IN;

  //-----------旋转编码器读取ロータリーエンコーダ読み取り----------------
  newPosition = myEnc.read();
  if ((newPosition - 3) / 4 > oldPosition / 4) {  //除以旋转编码器分辨率4 ロータリーエンコーダの分解能4で割る
    oldPosition = newPosition;
    scale = scale - 1;
  }

  else if ((newPosition + 3) / 4 < oldPosition / 4) {  //除以旋转编码器分辨率4 ロータリーエンコーダの分解能4で割る
    oldPosition = newPosition;
    scale = scale + 1;
  }

  if (scale < 0) {
    scale = 6;
  } else if (scale >= 7) {
    scale = 0;
  }

  //-------------------LED表示----------------------
  switch (scale) {
    case 0:  //紫
      LED(200, 0, 200);
      now_R = 200;
      now_G = 0;
      now_B = 200;
      break;

    case 1:  //青
      LED(0, 0, 255);
      now_R = 0;
      now_G = 0;
      now_B = 255;
      break;

    case 2:  //水色
      LED(0, 255, 255);
      now_R = 0;
      now_G = 255;
      now_B = 255;
      break;

    case 3:  //緑
      LED(0, 255, 0);
      now_R = 0;
      now_G = 255;
      now_B = 0;
      break;

    case 4:  //黄色
      LED(255, 120, 0);
      now_R = 255;
      now_G = 125;
      now_B = 0;
      break;

    case 5:  //赤
      LED(255, 0, 0);
      now_R = 255;
      now_G = 0;
      now_B = 0;
      break;

    case 6:  //白
      LED(255, 140, 110);
      now_R = 255;
      now_G = 140;
      now_B = 110;
      break;
  }

  //--------------slide設定--------------------------------------
  slide_sw = digitalRead(7);
  slide_CV = analogRead(0);
  slide_time = pow(slide_CV / 50, 2) + 1;  //感度調整

  if (slide_sw == 0) {  //slide_swがOFFならば、スライドは外部入力に依存 slide_如果sw为OFF，则幻灯片依赖于外部输入
    slide = digitalRead(6);
  } else if (slide_sw != 0) {  //slide_swがONならば、常時スライドON slide_如果sw为ON，则始终滑动ON
    slide = 1;
  }

  //----------------CV入出力設定 CV输入输出设置--------------------------------------
  CV_IN = analogRead(7);

  if (abs(old_CV_IN - CV_IN) > 10) {  //ノイズ対策。CVに大きな変化があったら、音程を変える。 噪音对策。如果CV有很大的变化，就改变音程。

    j = 0;  //slide reset

    for (i = 0; i <= 61; i++) {
      switch (scale) {

        case 0:  //0=major
          if (CV_IN >= (pgm_read_word(&(CVIN_th_maj[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_maj[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_maj[CV_INh])));
            goto DAC_done;
          }
          break;

        case 1:  //1=major_pentatonic
          if (CV_IN >= (pgm_read_word(&(CVIN_th_mp[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_mp[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_mp[CV_INh])));
            goto DAC_done;
          }
          break;

        case 2:  //2=major_pentatonic+7
          if (CV_IN >= (pgm_read_word(&(CVIN_th_mp7[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_mp7[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_mp7[CV_INh])));
            goto DAC_done;
          }
          break;

        case 3:  //3=harmonic_minor
          if (CV_IN >= (pgm_read_word(&(CVIN_th_hm[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_hm[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_hm[CV_INh])));
            goto DAC_done;
          }
          break;

        case 4:  //4=monor_pentatonic
          if (CV_IN >= (pgm_read_word(&(CVIN_th_minp[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_minp[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_minp[CV_INh])));
            goto DAC_done;
          }
          break;

        case 5:  //5=chromatic
          if (CV_IN >= (pgm_read_word(&(CVIN_th_chr[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_chr[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_chr[CV_INh])));
            goto DAC_done;
          }
          break;

        case 6:  //6=octave
          if (CV_IN >= (pgm_read_word(&(CVIN_th_oct[i]))) && CV_IN < (pgm_read_word(&(CVIN_th_oct[i + 1])))) {
            CV_INh = i;
            DAC(pgm_read_word(&(DAC_LSB_oct[CV_INh])));
            goto DAC_done;
          }
          break;
      }
    }
  }
DAC_done:

  //option:gaming mode (for only illumination)
  slide_IN = digitalRead(6);
  if (scale == 6 && slide_sw == 1 && slide_IN == 1) {
    k++;
    if (k >= 384) {
      k = 0;
    }

    if (k < 128) {
      illR = 0;
    } else if (k >= 128 && k < 256) {
      illR = (k - 128) * 2;
    } else if (k >= 256) {
      illR = 255 - (k - 256) * 2;
    }

    if (k < 128) {
      illG = k * 2;
    } else if (k >= 128 && k < 256) {
      illG = 255 - (k - 128) * 2;
    } else if (k >= 256) {
      illG = 0;
    }

    if (k < 128) {
      illB = 255 - k * 2;
    }

    else if (k >= 128 && k < 256) {
      illB = 0;
    } else if (k >= 256) {
      illB = (k - 256) * 2;
    }

    LED(illR, illG, illB);
    delay(slide_CV + 2);
  }

  //開発用
  Serial.print(slide_CV);
  Serial.print(",");
  Serial.print(CV_IN);
  Serial.println("");
}


void DAC(long CV_OUT) {   //CV出力_12bit_0-4095
  digitalWrite(5, HIGH);  //gate出力ON

  //-----------slide OFF時のCV出力------------------
  if (slide == 0 || slide_time <= 3) {
    Wire.beginTransmission(0x60);
    Wire.write((CV_OUT >> 8) & 0x0F);
    Wire.write(CV_OUT);
    Wire.endTransmission();
    LED(0, 0, 0);
    delay(5);
    digitalWrite(5, LOW);

  }

  //-----------slide ON時のCV出力------------------
  else if (slide == 1 && slide_time > 3) {

    if (old_CV_OUT <= CV_OUT) {  //低→高へのスライド
      while (j <= slide_time) {
        Wire.beginTransmission(0x60);
        Wire.write((old_CV_OUT + (CV_OUT - old_CV_OUT) * j / slide_time >> 8) & 0x0F);
        Wire.write(old_CV_OUT + (CV_OUT - old_CV_OUT) * j / slide_time);
        Wire.endTransmission();
        delay(5);
        j++;
        LED(min(j + 50, now_R), min(j + 50, now_G), min(j + 50, now_B));
      }
    }

    else if (old_CV_OUT > CV_OUT) {  //高→低へのスライド
      while (j <= slide_time) {
        Wire.beginTransmission(0x60);
        Wire.write((old_CV_OUT - (old_CV_OUT - CV_OUT) * j / slide_time >> 8) & 0x0F);
        Wire.write(old_CV_OUT - (old_CV_OUT - CV_OUT) * j / slide_time);
        Wire.endTransmission();
        delay(5);
        j++;
        LED(min(j + 50, now_R), min(j + 50, now_G), min(j + 50, now_B));
      }
    }

    old_CV_OUT = CV_OUT;
  }
  digitalWrite(5, LOW);  //gate出力OFF
}

void LED(byte R, byte G, byte B) {
  analogWrite(9, R);
  analogWrite(10, G);
  analogWrite(11, B);
}