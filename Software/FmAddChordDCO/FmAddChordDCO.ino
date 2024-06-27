//https://github.com/Testbild-synth/HAGIWO_arduino_oscillator //项目地址
#include <Mozzi.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <tables/saw2048_int8.h>
#include <tables/triangle_hermes_2048_int8.h>
#include <tables/square_no_alias_2048_int8.h>
#include <tables/triangle_dist_cubed_2048_int8.h>
#include <FixMath.h>
#include "Module_Ctrl.h"
#include "Module_LEDDisplay.h"

#define CONTROL_RATE 128

#define V_OCT_PIN A0

// #define FREQ_PIN A0
// #define P1_POT_PIN A1
// #define P2_POT_PIN A3

#define P1_CV_PIN A1
#define P2_CV_PIN A2
// #define GAIN_CV_PIN A6
#define MODE_CV_PIN A3

#define SW_PIN_1 4
#define SW_PIN_2 5
#define LED_1_PIN 6
#define LED_2_PIN 7
#define LED_3_PIN 8
#define FUNC_LENGTH 4  // menu length

Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc1(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc2(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc3(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc4(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc5(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc6(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc7(SIN2048_DATA);
Oscil<SIN2048_NUM_CELLS, AUDIO_RATE> osc8(SIN2048_DATA);

int btnHover3 = 0;  //按下事件判断
int btnTime3 = 0;   //长按事件判断

uint16_t freq_pot_val = 0;  //频率旋钮
uint16_t oct_cv_val = 0;    //voct cv
uint16_t p1_pot_val = 0;
uint16_t p1_cv_val = 0;
uint16_t p2_pot_val = 0;
uint16_t p2_cv_val = 0;
int gain_cv_val = 0;
uint8_t mode = 0;
uint8_t mode_val = 0;

///additive variables
int ADD_freq1 = 110;   //base freq of OSC1
int ADD_freqv1 = 440;  // freq1 apply voct
byte ADD_gain = 127;
uint8_t ADD_harm_knob = 0;
////////

////chord variables
int freq1 = 110;  //base freq

int freqv1 = 440;  //apply voct
int freqv2 = 440;
int freqv3 = 440;
int freqv4 = 440;
int freqv5 = 440;

byte note1 = 0;  //Root
byte note2 = 0;  //2nd
byte note3 = 0;  //3rd
byte note4 = 0;  //4th
byte note5 = 0;  //Root

byte inv_aply1 = 0;  //0 = no inv , 1 = inv , Root
byte inv_aply2 = 0;  //2nd
byte inv_aply3 = 0;  //3rd
byte inv_aply4 = 0;  //4th
bool inv_aply5 = 0;  //0 = no output root sound , 1 = output root sound

int inv = 0;
int inv_knob = 0;
int chord = 0;
byte wave = 0;
////////

////FM variables
Q16n16 fm_deviation;  //偏离
Q16n16 mod_freq, knob_freq;
///////

const static byte chord_table[8][4] PROGMEM = {
  {
    0,
    68,
    119,
    205,
  },  //Maj
  {
    0,
    68,
    119,
    187,
  },  //Maj7
  {
    0,
    68,
    119,
    239,
  },  //Majadd9
  {
    0,
    34,
    119,
    205,
  },  //sus2
  {
    0,
    51,
    119,
    239,
  },  //minadd9
  {
    0,
    51,
    119,
    170,
  },  //min7
  {
    0,
    51,
    119,
    205,
  },  //min
  {
    0,
    0,
    0,
    0,
  }  //root
};

const static float voctpow[1024] PROGMEM = {
  0, 0.004882, 0.009765, 0.014648, 0.019531, 0.024414, 0.029296, 0.034179, 0.039062, 0.043945, 0.048828, 0.05371, 0.058593, 0.063476, 0.068359, 0.073242, 0.078125, 0.083007, 0.08789, 0.092773, 0.097656, 0.102539, 0.107421, 0.112304, 0.117187, 0.12207, 0.126953, 0.131835, 0.136718, 0.141601, 0.146484, 0.151367, 0.15625,
  0.161132, 0.166015, 0.170898, 0.175781, 0.180664, 0.185546, 0.190429, 0.195312, 0.200195, 0.205078, 0.20996, 0.214843, 0.219726, 0.224609, 0.229492, 0.234375, 0.239257, 0.24414, 0.249023, 0.253906, 0.258789, 0.263671, 0.268554, 0.273437, 0.27832, 0.283203, 0.288085, 0.292968, 0.297851, 0.302734, 0.307617, 0.3125, 0.317382,
  0.322265, 0.327148, 0.332031, 0.336914, 0.341796, 0.346679, 0.351562, 0.356445, 0.361328, 0.36621, 0.371093, 0.375976, 0.380859, 0.385742, 0.390625, 0.395507, 0.40039, 0.405273, 0.410156, 0.415039, 0.419921, 0.424804, 0.429687, 0.43457, 0.439453, 0.444335, 0.449218, 0.454101, 0.458984, 0.463867, 0.46875, 0.473632, 0.478515,
  0.483398, 0.488281, 0.493164, 0.498046, 0.502929, 0.507812, 0.512695, 0.517578, 0.52246, 0.527343, 0.532226, 0.537109, 0.541992, 0.546875, 0.551757, 0.55664, 0.561523, 0.566406, 0.571289, 0.576171, 0.581054, 0.585937, 0.59082, 0.595703, 0.600585, 0.605468, 0.610351, 0.615234, 0.620117, 0.625, 0.629882, 0.634765, 0.639648,
  0.644531, 0.649414, 0.654296, 0.659179, 0.664062, 0.668945, 0.673828, 0.67871, 0.683593, 0.688476, 0.693359, 0.698242, 0.703125, 0.708007, 0.71289, 0.717773, 0.722656, 0.727539, 0.732421, 0.737304, 0.742187, 0.74707, 0.751953, 0.756835, 0.761718, 0.766601, 0.771484, 0.776367, 0.78125, 0.786132, 0.791015, 0.795898, 0.800781, 0.805664, 0.810546,
  0.815429, 0.820312, 0.825195, 0.830078, 0.83496, 0.839843, 0.844726, 0.849609, 0.854492, 0.859375, 0.864257, 0.86914, 0.874023, 0.878906, 0.883789, 0.888671, 0.893554, 0.898437, 0.90332, 0.908203, 0.913085, 0.917968, 0.922851, 0.927734, 0.932617, 0.9375, 0.942382, 0.947265, 0.952148, 0.957031, 0.961914, 0.966796, 0.971679, 0.976562, 0.981445, 0.986328, 0.99121, 0.996093, 1.000976, 1.005859, 1.010742, 1.015625, 1.020507, 1.02539, 1.030273, 1.035156, 1.040039, 1.044921, 1.049804, 1.054687, 1.05957, 1.064453, 1.069335, 1.074218, 1.079101, 1.083984,
  1.088867, 1.09375, 1.098632, 1.103515, 1.108398, 1.113281, 1.118164, 1.123046, 1.127929, 1.132812, 1.137695, 1.142578, 1.14746, 1.152343, 1.157226, 1.162109, 1.166992, 1.171875, 1.176757, 1.18164, 1.186523, 1.191406, 1.196289, 1.201171, 1.206054, 1.210937, 1.21582, 1.220703, 1.225585, 1.230468, 1.235351, 1.240234, 1.245117, 1.25, 1.254882, 1.259765, 1.264648, 1.269531, 1.274414, 1.279296, 1.284179, 1.289062, 1.293945, 1.298828, 1.30371, 1.308593, 1.313476, 1.318359, 1.323242, 1.328125, 1.333007, 1.33789, 1.342773, 1.347656, 1.352539, 1.357421, 1.362304, 1.367187, 1.37207, 1.376953, 1.381835, 1.386718, 1.391601, 1.396484, 1.401367, 1.40625, 1.411132, 1.416015, 1.420898, 1.425781, 1.430664, 1.435546, 1.440429, 1.445312, 1.450195, 1.455078, 1.45996, 1.464843, 1.469726, 1.474609, 1.479492, 1.484375, 1.489257, 1.49414, 1.499023, 1.503906, 1.508789, 1.513671, 1.518554, 1.523437, 1.52832, 1.533203, 1.538085, 1.542968, 1.547851, 1.552734, 1.557617, 1.5625, 1.567382, 1.572265, 1.577148, 1.582031, 1.586914, 1.591796, 1.596679, 1.601562, 1.606445, 1.611328, 1.61621, 1.621093, 1.625976, 1.630859, 1.635742, 1.640625, 1.645507, 1.65039, 1.655273, 1.660156, 1.665039, 1.669921, 1.674804, 1.679687, 1.68457, 1.689453, 1.694335, 1.699218, 1.704101, 1.708984, 1.713867, 1.71875, 1.723632, 1.728515, 1.733398, 1.738281, 1.743164, 1.748046, 1.752929, 1.757812, 1.762695, 1.767578, 1.77246, 1.777343, 1.782226, 1.787109, 1.791992, 1.796875, 1.801757, 1.80664, 1.811523, 1.816406, 1.821289, 1.826171, 1.831054, 1.835937, 1.84082, 1.845703, 1.850585, 1.855468, 1.860351, 1.865234, 1.870117, 1.875, 1.879882, 1.884765, 1.889648, 1.894531, 1.899414, 1.904296, 1.909179, 1.914062, 1.918945, 1.923828, 1.92871, 1.933593, 1.938476, 1.943359, 1.948242, 1.953125, 1.958007, 1.96289, 1.967773, 1.972656, 1.977539, 1.982421, 1.987304, 1.992187, 1.99707, 2.001953, 2.006835, 2.011718, 2.016601, 2.021484, 2.026367, 2.03125, 2.036132, 2.041015, 2.045898, 2.050781, 2.055664, 2.060546, 2.065429, 2.070312, 2.075195, 2.080078, 2.08496, 2.089843, 2.094726, 2.099609, 2.104492, 2.109375, 2.114257, 2.11914, 2.124023, 2.128906, 2.133789, 2.138671, 2.143554, 2.148437, 2.15332, 2.158203, 2.163085, 2.167968, 2.172851, 2.177734, 2.182617, 2.1875, 2.192382, 2.197265, 2.202148, 2.207031, 2.211914, 2.216796, 2.221679, 2.226562, 2.231445, 2.236328, 2.24121, 2.246093, 2.250976, 2.255859, 2.260742, 2.265625, 2.270507, 2.27539, 2.280273, 2.285156, 2.290039, 2.294921, 2.299804, 2.304687, 2.30957, 2.314453, 2.319335, 2.324218, 2.329101, 2.333984, 2.338867, 2.34375, 2.348632, 2.353515, 2.358398, 2.363281, 2.368164, 2.373046, 2.377929, 2.382812, 2.387695, 2.392578, 2.39746, 2.402343, 2.407226, 2.412109, 2.416992, 2.421875, 2.426757, 2.43164, 2.436523, 2.441406, 2.446289, 2.451171, 2.456054, 2.460937, 2.46582, 2.470703, 2.475585, 2.480468, 2.485351, 2.490234, 2.495117, 2.5, 2.504882, 2.509765, 2.514648, 2.519531, 2.524414, 2.529296, 2.534179, 2.539062, 2.543945, 2.548828, 2.55371, 2.558593, 2.563476, 2.568359, 2.573242, 2.578125, 2.583007, 2.58789, 2.592773, 2.597656, 2.602539, 2.607421, 2.612304, 2.617187, 2.62207, 2.626953, 2.631835, 2.636718, 2.641601, 2.646484, 2.651367, 2.65625, 2.661132, 2.666015, 2.670898, 2.675781, 2.680664, 2.685546, 2.690429, 2.695312, 2.700195, 2.705078, 2.70996, 2.714843, 2.719726, 2.724609, 2.729492, 2.734375, 2.739257, 2.74414, 2.749023, 2.753906, 2.758789, 2.763671, 2.768554, 2.773437, 2.77832, 2.783203, 2.788085, 2.792968, 2.797851, 2.802734, 2.807617, 2.8125, 2.817382, 2.822265, 2.827148, 2.832031, 2.836914, 2.841796, 2.846679, 2.851562, 2.856445, 2.861328, 2.86621, 2.871093, 2.875976, 2.880859, 2.885742, 2.890625, 2.895507, 2.90039, 2.905273, 2.910156, 2.915039, 2.919921, 2.924804, 2.929687, 2.93457, 2.939453, 2.944335, 2.949218, 2.954101, 2.958984, 2.963867, 2.96875, 2.973632, 2.978515, 2.983398, 2.988281, 2.993164, 2.998046, 3.002929, 3.007812, 3.012695, 3.017578, 3.02246, 3.027343, 3.032226, 3.037109, 3.041992, 3.046875, 3.051757, 3.05664, 3.061523, 3.066406, 3.071289, 3.076171, 3.081054, 3.085937, 3.09082, 3.095703, 3.100585, 3.105468, 3.110351, 3.115234, 3.120117, 3.125, 3.129882, 3.134765, 3.139648, 3.144531, 3.149414, 3.154296, 3.159179, 3.164062, 3.168945, 3.173828, 3.17871, 3.183593, 3.188476, 3.193359, 3.198242, 3.203125, 3.208007, 3.21289, 3.217773, 3.222656, 3.227539, 3.232421, 3.237304, 3.242187, 3.24707, 3.251953, 3.256835, 3.261718, 3.266601, 3.271484, 3.276367, 3.28125, 3.286132, 3.291015, 3.295898, 3.300781, 3.305664, 3.310546, 3.315429, 3.320312, 3.325195, 3.330078, 3.33496, 3.339843, 3.344726, 3.349609, 3.354492, 3.359375, 3.364257, 3.36914, 3.374023, 3.378906, 3.383789, 3.388671, 3.393554, 3.398437, 3.40332, 3.408203, 3.413085, 3.417968, 3.422851, 3.427734, 3.432617, 3.4375, 3.442382, 3.447265, 3.452148, 3.457031, 3.461914, 3.466796, 3.471679, 3.476562, 3.481445, 3.486328, 3.49121, 3.496093, 3.500976, 3.505859, 3.510742, 3.515625, 3.520507, 3.52539, 3.530273, 3.535156, 3.540039, 3.544921, 3.549804, 3.554687, 3.55957, 3.564453, 3.569335, 3.574218, 3.579101, 3.583984, 3.588867, 3.59375, 3.598632, 3.603515, 3.608398, 3.613281, 3.618164, 3.623046, 3.627929, 3.632812, 3.637695, 3.642578, 3.64746, 3.652343, 3.657226, 3.662109, 3.666992, 3.671875, 3.676757, 3.68164, 3.686523, 3.691406, 3.696289, 3.701171, 3.706054, 3.710937, 3.71582, 3.720703, 3.725585, 3.730468, 3.735351, 3.740234, 3.745117, 3.75, 3.754882, 3.759765, 3.764648, 3.769531, 3.774414, 3.779296, 3.784179, 3.789062, 3.793945, 3.798828, 3.80371, 3.808593, 3.813476, 3.818359, 3.823242, 3.828125, 3.833007, 3.83789, 3.842773, 3.847656, 3.852539, 3.857421, 3.862304, 3.867187, 3.87207, 3.876953, 3.881835, 3.886718, 3.891601, 3.896484, 3.901367, 3.90625, 3.911132, 3.916015, 3.920898, 3.925781, 3.930664, 3.935546, 3.940429, 3.945312, 3.950195, 3.955078, 3.95996, 3.964843, 3.969726, 3.974609, 3.979492, 3.984375, 3.989257, 3.99414, 3.999023, 4.003906, 4.008789, 4.013671, 4.018554, 4.023437, 4.02832, 4.033203, 4.038085, 4.042968, 4.047851, 4.052734, 4.057617, 4.0625, 4.067382, 4.072265, 4.077148, 4.082031, 4.086914, 4.091796, 4.096679, 4.101562, 4.106445, 4.111328, 4.11621, 4.121093, 4.125976, 4.130859, 4.135742, 4.140625, 4.145507, 4.15039, 4.155273, 4.160156, 4.165039, 4.169921, 4.174804, 4.179687, 4.18457, 4.189453, 4.194335, 4.199218, 4.204101, 4.208984, 4.213867, 4.21875, 4.223632, 4.228515, 4.233398, 4.238281, 4.243164, 4.248046, 4.252929, 4.257812, 4.262695, 4.267578, 4.27246, 4.277343, 4.282226, 4.287109, 4.291992, 4.296875, 4.301757, 4.30664, 4.311523, 4.316406, 4.321289, 4.326171, 4.331054, 4.335937, 4.34082, 4.345703, 4.350585, 4.355468, 4.360351, 4.365234, 4.370117, 4.375, 4.379882, 4.384765, 4.389648, 4.394531, 4.399414, 4.404296, 4.409179, 4.414062, 4.418945, 4.423828, 4.42871, 4.433593, 4.438476, 4.443359, 4.448242, 4.453125, 4.458007, 4.46289, 4.467773, 4.472656, 4.477539, 4.482421, 4.487304, 4.492187, 4.49707, 4.501953, 4.506835, 4.511718, 4.516601, 4.521484, 4.526367, 4.53125, 4.536132, 4.541015, 4.545898, 4.550781, 4.555664, 4.560546, 4.565429, 4.570312, 4.575195, 4.580078, 4.58496, 4.589843, 4.594726, 4.599609, 4.604492, 4.609375, 4.614257, 4.61914, 4.624023, 4.628906, 4.633789, 4.638671, 4.643554, 4.648437, 4.65332, 4.658203, 4.663085, 4.667968, 4.672851, 4.677734, 4.682617, 4.6875, 4.692382, 4.697265, 4.702148, 4.707031, 4.711914, 4.716796, 4.721679, 4.726562, 4.731445, 4.736328, 4.74121, 4.746093, 4.750976, 4.755859, 4.760742, 4.765625, 4.770507, 4.77539, 4.780273, 4.785156, 4.790039, 4.794921, 4.799804, 4.804687, 4.80957, 4.814453, 4.819335, 4.824218, 4.829101, 4.833984, 4.838867, 4.84375, 4.848632, 4.853515, 4.858398, 4.863281, 4.868164, 4.873046, 4.877929, 4.882812, 4.887695, 4.892578, 4.89746, 4.902343, 4.907226, 4.912109, 4.916992, 4.921875, 4.926757, 4.93164, 4.936523, 4.941406, 4.946289, 4.951171, 4.956054, 4.960937, 4.96582, 4.970703, 4.975585, 4.980468, 4.985351, 4.990234, 4.995117
};

const static byte ADD_gain_table[8][256] PROGMEM = {
  { 255, 255, 254, 253, 251, 249, 247, 244, 242, 238, 235, 231, 227, 223, 218, 213, 208, 203, 197, 192, 186, 180, 173, 167, 161, 154, 148, 141, 134, 128, 121, 114, 108, 101, 94, 88, 82, 76, 70, 64, 58, 52, 47, 42, 37, 32, 28, 24, 20, 17, 14, 11, 8, 6, 4, 2, 1, 0, 0, 0, 0, 0, 1, 2, 4, 6, 8, 10, 13, 17, 20, 24, 28, 32, 37, 42, 47, 52, 58, 63, 69, 75, 81, 88, 94, 101, 107, 114, 120, 127, 134, 141, 147, 154, 160, 167, 173, 179, 185, 191, 197, 202, 208, 213, 218, 222, 227, 231, 235, 238, 241, 244, 247, 249, 251, 253, 254, 255, 255, 255, 255, 255, 254, 253, 251, 249, 247, 245, 242, 239, 235, 231, 227, 223, 218, 213, 208, 203, 198, 192, 186, 180, 174, 168, 161, 155, 148, 141, 135, 128, 121, 115, 108, 101, 95, 88, 82, 76, 70, 64, 58, 53, 47, 42, 37, 33, 28, 24, 20, 17, 14, 11, 8, 6, 4, 2, 1, 0, 0, 0, 0, 0, 1, 2, 4, 6, 8, 10, 13, 16, 20, 24, 28, 32, 37, 41, 46, 52, 57, 63, 69, 75, 81, 87, 94, 100, 107, 113, 120, 127, 133, 140, 147, 153, 160, 166, 173, 179, 185, 191, 197, 202, 207, 213, 217, 222, 226, 231, 234, 238, 241, 244, 247, 249, 251, 253, 254, 255, 255, 255, 255, 255, 254, 253, 251, 249, 247, 245, 242, 239, 235, 232, 228, 223, 219, 214 },
  { 221, 214, 207, 199, 191, 182, 173, 163, 153, 142, 132, 121, 111, 100, 90, 80, 70, 60, 51, 43, 35, 28, 22, 16, 11, 7, 4, 1, 0, 0, 0, 1, 4, 7, 11, 16, 21, 28, 35, 43, 51, 60, 69, 79, 89, 100, 110, 121, 131, 142, 152, 162, 172, 182, 191, 199, 207, 214, 221, 226, 231, 235, 238, 241, 242, 243, 242, 241, 239, 236, 231, 227, 221, 215, 207, 200, 191, 182, 173, 163, 153, 143, 132, 122, 111, 100, 90, 80, 70, 61, 52, 43, 35, 28, 22, 16, 11, 7, 4, 1, 0, 0, 0, 1, 3, 7, 11, 15, 21, 28, 35, 42, 51, 60, 69, 79, 89, 99, 110, 120, 131, 142, 152, 162, 172, 181, 190, 199, 207, 214, 220, 226, 231, 235, 238, 241, 242, 243, 242, 241, 239, 236, 232, 227, 221, 215, 208, 200, 192, 183, 173, 163, 153, 143, 133, 122, 111, 101, 90, 80, 71, 61, 52, 44, 36, 29, 22, 16, 11, 7, 4, 2, 0, 0, 0, 1, 3, 6, 10, 15, 21, 27, 34, 42, 51, 59, 69, 79, 89, 99, 109, 120, 131, 141, 152, 162, 171, 181, 190, 198, 206, 214, 220, 226, 231, 235, 238, 241, 242, 243, 242, 241, 239, 236, 232, 227, 221, 215, 208, 200, 192, 183, 174, 164, 154, 143, 133, 122, 112, 101, 91, 81, 71, 61, 52, 44, 36, 29, 22, 16, 11, 7, 4, 2, 0, 0, 0, 1, 3, 6, 10, 15, 21, 27, 34, 42 },
  { 165, 158, 150, 143, 135, 127, 119, 111, 103, 95, 87, 79, 72, 64, 57, 50, 44, 38, 32, 27, 22, 17, 13, 10, 7, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 13, 17, 21, 26, 32, 37, 44, 50, 57, 64, 71, 79, 87, 94, 102, 110, 118, 126, 134, 142, 150, 158, 165, 172, 179, 185, 192, 197, 203, 208, 212, 216, 220, 223, 225, 227, 229, 230, 230, 230, 229, 227, 226, 223, 220, 217, 213, 208, 203, 198, 192, 186, 179, 173, 166, 158, 151, 143, 135, 127, 119, 111, 103, 95, 87, 80, 72, 65, 58, 51, 44, 38, 32, 27, 22, 17, 13, 10, 7, 4, 2, 1, 0, 0, 0, 1, 2, 4, 6, 9, 13, 17, 21, 26, 31, 37, 43, 50, 57, 64, 71, 79, 86, 94, 102, 110, 118, 126, 134, 142, 150, 157, 165, 172, 179, 185, 191, 197, 203, 207, 212, 216, 220, 223, 225, 227, 229, 230, 230, 230, 229, 228, 226, 223, 220, 217, 213, 208, 203, 198, 192, 186, 180, 173, 166, 159, 151, 143, 136, 128, 120, 112, 103, 96, 88, 80, 72, 65, 58, 51, 44, 38, 32, 27, 22, 17, 13, 10, 7, 4, 2, 1, 0, 0, 0, 0, 2, 4, 6, 9, 13, 17, 21, 26, 31, 37, 43, 49, 56, 63, 71, 78, 86, 94, 102, 110, 118, 126, 134, 142, 149, 157, 164, 171, 178, 185, 191, 197, 202, 207, 212, 216, 219, 223, 225, 227, 229, 230, 230, 230 },
  { 97, 86, 75, 64, 54, 44, 36, 28, 20, 14, 9, 5, 2, 0, 0, 0, 2, 5, 9, 14, 20, 27, 35, 44, 54, 64, 74, 85, 97, 108, 119, 131, 142, 152, 162, 172, 181, 189, 196, 202, 208, 212, 215, 216, 217, 217, 215, 212, 208, 203, 197, 189, 181, 173, 163, 153, 142, 131, 120, 109, 97, 86, 75, 64, 54, 45, 36, 28, 21, 14, 9, 5, 2, 0, 0, 0, 2, 5, 9, 14, 20, 27, 35, 44, 53, 64, 74, 85, 96, 108, 119, 130, 141, 152, 162, 172, 181, 189, 196, 202, 207, 212, 215, 216, 217, 217, 215, 212, 208, 203, 197, 190, 182, 173, 163, 153, 143, 132, 120, 109, 98, 86, 75, 65, 55, 45, 36, 28, 21, 14, 9, 5, 2, 0, 0, 0, 2, 5, 9, 14, 20, 27, 35, 44, 53, 63, 74, 85, 96, 107, 119, 130, 141, 152, 162, 171, 180, 188, 196, 202, 207, 211, 215, 216, 217, 217, 215, 212, 208, 203, 197, 190, 182, 173, 164, 154, 143, 132, 121, 109, 98, 87, 76, 65, 55, 45, 36, 28, 21, 15, 9, 5, 2, 0, 0, 0, 2, 4, 8, 13, 20, 27, 35, 43, 53, 63, 73, 84, 96, 107, 118, 130, 141, 151, 162, 171, 180, 188, 196, 202, 207, 211, 214, 216, 217, 217, 215, 212, 208, 203, 197, 190, 182, 173, 164, 154, 143, 132, 121, 110, 98, 87, 76, 65, 55, 46, 37, 28, 21, 15, 10, 5, 2, 0, 0, 0 },
  { 46, 42, 38, 33, 30, 26, 22, 19, 16, 13, 11, 8, 6, 5, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 8, 11, 13, 16, 19, 22, 26, 29, 33, 37, 42, 46, 51, 55, 60, 65, 70, 75, 80, 86, 91, 96, 102, 107, 112, 118, 123, 128, 133, 138, 143, 148, 153, 157, 162, 166, 170, 174, 178, 181, 185, 188, 190, 193, 195, 197, 199, 201, 202, 203, 204, 204, 204, 204, 204, 203, 202, 201, 199, 198, 196, 193, 191, 188, 185, 182, 178, 175, 171, 167, 162, 158, 153, 149, 144, 139, 134, 129, 124, 118, 113, 108, 102, 97, 92, 86, 81, 76, 71, 66, 61, 56, 51, 46, 42, 38, 34, 30, 26, 23, 19, 16, 13, 11, 9, 6, 5, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 8, 10, 13, 16, 19, 22, 25, 29, 33, 37, 41, 46, 50, 55, 60, 65, 70, 75, 80, 85, 91, 96, 101, 107, 112, 117, 123, 128, 133, 138, 143, 148, 153, 157, 162, 166, 170, 174, 178, 181, 184, 187, 190, 193, 195, 197, 199, 201, 202, 203, 204, 204, 204, 204, 204, 203, 202, 201, 199, 198, 196, 193, 191, 188, 185, 182, 178, 175, 171, 167, 163, 158, 154, 149, 144, 139, 134, 129, 124, 119, 113, 108, 103, 97, 92, 87, 81, 76, 71, 66, 61, 56, 51, 47, 42, 38, 34, 30, 26, 23, 20, 16, 14, 11, 9, 7, 5, 3, 2 },
  { 9, 5, 3, 1, 0, 0, 0, 1, 3, 5, 8, 12, 17, 22, 27, 34, 40, 47, 55, 62, 70, 79, 87, 95, 104, 112, 120, 128, 136, 143, 150, 157, 163, 169, 174, 178, 182, 186, 188, 190, 191, 191, 191, 190, 188, 186, 183, 179, 174, 169, 164, 157, 151, 144, 136, 129, 121, 113, 104, 96, 88, 79, 71, 63, 55, 48, 41, 34, 28, 22, 17, 13, 9, 5, 3, 1, 0, 0, 0, 1, 3, 5, 8, 12, 17, 22, 27, 33, 40, 47, 54, 62, 70, 78, 87, 95, 103, 112, 120, 128, 136, 143, 150, 157, 163, 169, 174, 178, 182, 186, 188, 190, 191, 191, 191, 190, 188, 186, 183, 179, 175, 169, 164, 158, 151, 144, 137, 129, 121, 113, 105, 96, 88, 80, 71, 63, 56, 48, 41, 34, 28, 22, 17, 13, 9, 6, 3, 1, 0, 0, 0, 1, 3, 5, 8, 12, 16, 21, 27, 33, 40, 47, 54, 62, 70, 78, 86, 95, 103, 111, 120, 128, 135, 143, 150, 157, 163, 168, 174, 178, 182, 185, 188, 190, 191, 191, 191, 190, 188, 186, 183, 179, 175, 170, 164, 158, 151, 144, 137, 129, 121, 113, 105, 96, 88, 80, 72, 64, 56, 48, 41, 35, 28, 23, 17, 13, 9, 6, 3, 1, 0, 0, 0, 1, 2, 5, 8, 12, 16, 21, 27, 33, 40, 47, 54, 62, 70, 78, 86, 94, 103, 111, 119, 127, 135, 142, 150, 156, 163, 168, 173, 178, 182, 185, 188, 190 },
  { 0, 0, 1, 3, 5, 7, 10, 13, 17, 20, 25, 29, 34, 39, 44, 50, 55, 61, 67, 73, 80, 86, 92, 98, 104, 111, 117, 122, 128, 134, 139, 144, 149, 153, 158, 161, 165, 168, 171, 173, 175, 177, 178, 178, 179, 179, 178, 177, 175, 173, 171, 168, 165, 162, 158, 154, 149, 145, 139, 134, 129, 123, 117, 111, 105, 99, 93, 86, 80, 74, 68, 62, 56, 50, 45, 39, 34, 29, 25, 21, 17, 13, 10, 7, 5, 3, 2, 0, 0, 0, 0, 0, 1, 3, 5, 7, 10, 13, 16, 20, 24, 29, 34, 39, 44, 49, 55, 61, 67, 73, 79, 85, 92, 98, 104, 110, 116, 122, 128, 133, 139, 144, 149, 153, 157, 161, 165, 168, 171, 173, 175, 177, 178, 178, 179, 179, 178, 177, 175, 173, 171, 168, 165, 162, 158, 154, 150, 145, 140, 134, 129, 123, 117, 111, 105, 99, 93, 87, 80, 74, 68, 62, 56, 50, 45, 40, 34, 30, 25, 21, 17, 13, 10, 8, 5, 3, 2, 0, 0, 0, 0, 0, 1, 3, 5, 7, 10, 13, 16, 20, 24, 29, 33, 38, 44, 49, 55, 61, 67, 73, 79, 85, 91, 98, 104, 110, 116, 122, 128, 133, 139, 144, 148, 153, 157, 161, 165, 168, 171, 173, 175, 177, 178, 178, 179, 179, 178, 177, 175, 174, 171, 169, 166, 162, 158, 154, 150, 145, 140, 135, 129, 124, 118, 112, 106, 99, 93, 87, 81, 74, 68, 62, 56, 51, 45, 40 },
  { 15, 21, 27, 34, 41, 49, 57, 65, 74, 83, 91, 100, 108, 116, 124, 131, 138, 144, 150, 155, 159, 162, 164, 165, 166, 165, 164, 162, 159, 155, 150, 145, 139, 132, 125, 117, 109, 100, 92, 83, 74, 66, 57, 49, 41, 34, 27, 21, 16, 11, 7, 4, 1, 0, 0, 0, 1, 3, 7, 10, 15, 21, 27, 33, 41, 48, 57, 65, 74, 82, 91, 100, 108, 116, 124, 131, 138, 144, 150, 154, 158, 162, 164, 165, 166, 165, 164, 162, 159, 155, 150, 145, 139, 132, 125, 117, 109, 101, 92, 83, 75, 66, 58, 49, 42, 34, 27, 21, 16, 11, 7, 4, 1, 0, 0, 0, 1, 3, 6, 10, 15, 20, 27, 33, 40, 48, 56, 65, 73, 82, 91, 99, 108, 116, 124, 131, 138, 144, 150, 154, 158, 162, 164, 165, 166, 166, 164, 162, 159, 155, 150, 145, 139, 132, 125, 117, 109, 101, 92, 84, 75, 66, 58, 50, 42, 35, 28, 21, 16, 11, 7, 4, 2, 0, 0, 0, 1, 3, 6, 10, 15, 20, 26, 33, 40, 48, 56, 64, 73, 82, 90, 99, 107, 116, 123, 131, 138, 144, 149, 154, 158, 161, 164, 165, 166, 166, 164, 162, 159, 155, 151, 145, 139, 132, 125, 118, 109, 101, 93, 84, 75, 67, 58, 50, 42, 35, 28, 22, 16, 11, 7, 4, 2, 0, 0, 0, 1, 3, 6, 10, 15, 20, 26, 33, 40, 48, 56, 64, 73, 81, 90, 99, 107, 115, 123, 131 }
};

const static byte ADD_harm_table[8][256] PROGMEM = {
  { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4 },
  { 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
  { 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
  { 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 5, 5, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 7, 6, 6, 5, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 5, 6, 6, 7, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 7, 7, 6, 5, 5, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 5, 5, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 7, 6, 6, 5, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 5, 6, 6, 7, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 7, 7, 6, 5, 5, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 5, 5, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 10, 10, 10, 10, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 4, 4, 4 },
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 12, 12, 12, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 12, 12, 12, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 12, 12, 12, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 12, 12, 12, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 12, 12, 12, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 12, 12, 12, 12, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 12, 12, 12, 12, 14, 14, 14 },
  { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 14, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 15, 15, 15, 15, 15, 15, 14, 14, 14, 12, 12, 10, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 14, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 15, 15, 15, 15, 15, 14, 14, 14, 12, 12, 12, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 14, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 15, 15, 15, 15, 15, 14, 14, 14, 12, 12, 12, 10, 10, 8, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 4, 3 },
  { 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 15, 15, 15, 14, 14, 14, 12, 12, 12, 10, 10, 8, 8, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 5, 5, 6, 6, 6, 7, 7, 8, 8, 8, 8, 8, 10, 10, 10, 12, 12, 14, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 15, 15, 15, 14, 14, 14, 12, 12, 12, 10, 10, 8, 8, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 6 }
};

byte POSITION = 0;
char function[FUNC_LENGTH][5] = { "Freq", "P1", "P2", "Mode" };
short param[FUNC_LENGTH] = { 0, 0, 0 };
bool* ledGroup[FUNC_LENGTH] = { Led_F, Led_1, Led_2, Led_M };

void setup() {
  Serial.begin(115200);  //使用Serial.begin()函数来初始化串口波特率,参数为要设置的波特率
  startMozzi(CONTROL_RATE);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(SW_PIN_1, INPUT_PULLUP);
  pinMode(SW_PIN_2, INPUT_PULLUP);
  digitalWrite(SW_PIN_1, HIGH);
  digitalWrite(SW_PIN_2, HIGH);

  initCtrl(4, 50, 12, 13, HIGH);  //初始化控制参数// 旋钮 旋钮修改启动范围 按钮1 按钮2
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, INPUT);
  digitalWrite(11, LOW);

  osc1.setTable(SIN2048_DATA);
  osc2.setTable(SIN2048_DATA);
  osc3.setTable(SIN2048_DATA);
  osc4.setTable(SIN2048_DATA);
  osc5.setTable(SIN2048_DATA);
  osc6.setTable(SIN2048_DATA);
  osc7.setTable(SIN2048_DATA);
  osc8.setTable(SIN2048_DATA);
}

void updateControl() {
  POSITION = getPostition(POSITION, FUNC_LENGTH);  //获取菜单下标
  param[POSITION] = getParam(param[POSITION]);     //用以注册按钮旋钮控制引脚 并获取修改成功的旋钮值
  displayLED(ledGroup[POSITION]);                  //display  //用字母展示控制
  if (getKnobEnable() == 0) displayLED(Led_NULL);  //如果处在非编辑状态 led将半灭显示

  Serial.print(" mode=");           //mode 0: FM mode 1: add, mode 2: chord
  Serial.print(mode);               //mode
  Serial.print(" func");            //func param
  Serial.print(POSITION);           //func param
  Serial.print("=");                //func param
  Serial.println(param[POSITION]);  //func param

  read_inputs();
  mode = param[3] >> 8;  // mode 0: FM mode 1: add, mode 2: chord

  if (p1_pot_val >= 1020) {  //inv knob max
    set_waves();
  } else {
    if (mode == 0) {
      knob_freq = freq_pot_val;
      FM_setFreqs(knob_freq);
    } else if (mode == 1) {
      CHORD_setFreqs();
    } else {
      ADD_setFreqs();
    }
  }
}

void read_inputs() {
  // freq_pot_val = mozziAnalogRead(FREQ_PIN);
  freq_pot_val = param[0];
  oct_cv_val = mozziAnalogRead(V_OCT_PIN);
  // p1_pot_val = mozziAnalogRead(P1_POT_PIN);
  p1_pot_val = param[1];
  p1_cv_val = mozziAnalogRead(P1_CV_PIN);
  // p2_pot_val = mozziAnalogRead(P2_POT_PIN);
  p2_pot_val = param[2];
  p2_cv_val = mozziAnalogRead(P2_CV_PIN);
  // gain_cv_val = constrain(param[3] / 2, 0, 255);
  gain_cv_val = 255;
  mode_val = mozziAnalogRead(MODE_CV_PIN) / 100;
}

//
void set_waves() {
  switch (round(p2_pot_val / 200) - 1) {
    case 0:  //sin
      osc1.setTable(SIN2048_DATA);
      osc2.setTable(SIN2048_DATA);
      osc3.setTable(SIN2048_DATA);
      osc4.setTable(SIN2048_DATA);
      osc5.setTable(SIN2048_DATA);
      osc6.setTable(SIN2048_DATA);
      osc7.setTable(SIN2048_DATA);
      osc8.setTable(SIN2048_DATA);
      break;

    case 1:  //tri hermes
      osc1.setTable(TRIANGLE_HERMES_2048_DATA);
      osc2.setTable(TRIANGLE_HERMES_2048_DATA);
      osc3.setTable(TRIANGLE_HERMES_2048_DATA);
      osc4.setTable(TRIANGLE_HERMES_2048_DATA);
      osc5.setTable(TRIANGLE_HERMES_2048_DATA);
      osc6.setTable(TRIANGLE_HERMES_2048_DATA);
      osc7.setTable(TRIANGLE_HERMES_2048_DATA);
      osc8.setTable(TRIANGLE_HERMES_2048_DATA);
      break;

    case 2:  //tri dist
      osc1.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc2.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc3.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc4.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc5.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc6.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc7.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      osc8.setTable(TRIANGLE_DIST_CUBED_2048_DATA);
      break;

    case 3:  //saw
      osc1.setTable(SAW2048_DATA);
      osc2.setTable(SAW2048_DATA);
      osc3.setTable(SAW2048_DATA);
      osc4.setTable(SAW2048_DATA);
      osc5.setTable(SAW2048_DATA);
      osc6.setTable(SAW2048_DATA);
      osc7.setTable(SAW2048_DATA);
      osc8.setTable(SAW2048_DATA);
      break;

    case 4:  //square
      osc1.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc2.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc3.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc4.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc5.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc6.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc7.setTable(SQUARE_NO_ALIAS_2048_DATA);
      osc8.setTable(SQUARE_NO_ALIAS_2048_DATA);
      break;
      break;
  }
}

void FM_setFreqs(Q16n16 freq2) {  //freq2=knob_freq
  // Serial.print("freq2 ");
  // Serial.println(freq2);

  Q16n16 toneFreq = (2270658 + freq2 * 5000) * pow(2, (pgm_read_float(&(voctpow[oct_cv_val]))));
  // mod_freq = ((toneFreq >> 8) * ((p1_pot_val + p1_cv_val) / 2));
  // fm_deviation = (p2_pot_val + p2_cv_val) << 6;
  // osc1.setFreq_Q16n16(toneFreq);
  // osc2.setFreq_Q16n16(mod_freq);
  mod_freq = (toneFreq >> 1) * ((p1_pot_val + p1_cv_val) >> 5);      //新版倍频算法 0.5-16倍频
  fm_deviation = ((mod_freq >> 16) * (1 + p2_pot_val + p2_cv_val));  //op2amount
  osc1.setFreq_Q16n16(toneFreq);                                     //给主波形设置频率
  osc2.setFreq_Q16n16(mod_freq);
}

void ADD_setFreqs() {
  //harmonics
  ADD_harm_knob = constrain(((p2_pot_val + p2_cv_val) / 4), 0, 255);
  //harmonics_gain
  ADD_gain = constrain(((p1_cv_val + p1_pot_val) / 4), 0, 255);
  //OSC frequency knob
  ADD_freq1 = freq_pot_val / 8;
  ADD_freqv1 = ADD_freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val]))));  // V/oct apply

  osc1.setFreq(ADD_freqv1);  // set the frequency
  osc2.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[0][ADD_harm_knob]))));
  osc3.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[1][ADD_harm_knob]))));
  osc4.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[2][ADD_harm_knob]))));
  osc5.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[3][ADD_harm_knob]))));
  osc6.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[4][ADD_harm_knob]))));
  osc7.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[5][ADD_harm_knob]))));
  osc8.setFreq(ADD_freqv1 * (pgm_read_byte(&(ADD_harm_table[6][ADD_harm_knob]))));
}

void CHORD_setFreqs() {
  chord = constrain((p2_pot_val / 128) + (p2_cv_val / 128), 0, 7);

  //inversion setting
  inv = constrain((p1_pot_val / 64) + (p1_cv_val / 64), 0, 15);
  switch (inv) {
    case 0:
      inv_aply1 = 0;
      inv_aply2 = 0;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 0;
      break;

    case 1:
      inv_aply1 = 1;
      inv_aply2 = 0;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 0;
      break;

    case 2:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 0;
      break;

    case 3:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 0;
      inv_aply5 = 0;
      break;

    case 4:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 0;
      break;

    case 5:
      inv_aply1 = 2;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 0;
      break;

    case 6:
      inv_aply1 = 2;
      inv_aply2 = 2;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 0;
      break;

    case 7:
      inv_aply1 = 2;
      inv_aply2 = 2;
      inv_aply3 = 2;
      inv_aply4 = 1;
      inv_aply5 = 0;
      break;

    case 8:
      inv_aply1 = 2;
      inv_aply2 = 2;
      inv_aply3 = 2;
      inv_aply4 = 1;
      inv_aply5 = 1;
      break;

    case 9:
      inv_aply1 = 2;
      inv_aply2 = 2;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 1;
      break;

    case 10:
      inv_aply1 = 2;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 1;
      break;

    case 11:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 1;
      inv_aply5 = 1;
      break;

    case 12:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 1;
      inv_aply4 = 0;
      inv_aply5 = 1;
      break;

    case 13:
      inv_aply1 = 1;
      inv_aply2 = 1;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 1;
      break;

    case 14:
      inv_aply1 = 1;
      inv_aply2 = 0;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 1;
      break;

    case 15:
      inv_aply1 = 0;
      inv_aply2 = 0;
      inv_aply3 = 0;
      inv_aply4 = 0;
      inv_aply5 = 1;
      break;
  }
  //setting chord note
  note1 = (pgm_read_byte(&(chord_table[chord][0])));
  note2 = (pgm_read_byte(&(chord_table[chord][1])));
  note3 = (pgm_read_byte(&(chord_table[chord][2])));
  note4 = (pgm_read_byte(&(chord_table[chord][3])));
  note5 = (pgm_read_byte(&(chord_table[chord][0])));
  //OSC frequency knob
  freq1 = freq_pot_val / 4 + 64;
  //set wave
  if (p1_pot_val >= 1020) {  //inv knob max
    wave = (p2_pot_val / 128);
  }
  freqv1 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + 205 * inv_aply1 + note1]))));  //ROOT
  freqv2 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + 205 * inv_aply2 + note2]))));  //2nd
  freqv3 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + 205 * inv_aply3 + note3]))));  //3rd
  freqv4 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + 205 * inv_aply4 + note4]))));  //4th
  freqv5 = freq1 * pow(2, (pgm_read_float(&(voctpow[oct_cv_val + note5]))));                    //ROOT
  osc1.setFreq(freqv1);                                                                         // set the frequency
  osc2.setFreq(freqv2);
  osc3.setFreq(freqv3);
  osc4.setFreq(freqv4);
  osc5.setFreq(freqv5);
}


AudioOutput_t updateAudio() {
  if (mode == 0) {  //fm
    // return MonoOutput::fromNBit(16, (osc1.phMod(Q15n16(fm_deviation * osc2.next() >> 8)) / 2) * (gain_cv_val / 2));//old
    return MonoOutput::fromNBit(16, osc1.phMod(fm_deviation * osc2.next() >> 8) << 8);//new
  } else if (mode == 2) {  //add
    return MonoOutput::fromNBit(16, (osc1.next() * (pgm_read_byte(&(ADD_gain_table[0][ADD_gain]))) / 512 + osc2.next() * (pgm_read_byte(&(ADD_gain_table[1][ADD_gain]))) / 512 + osc3.next() * (pgm_read_byte(&(ADD_gain_table[2][ADD_gain]))) / 512 + osc4.next() * (pgm_read_byte(&(ADD_gain_table[3][ADD_gain]))) / 512 + osc5.next() * (pgm_read_byte(&(ADD_gain_table[4][ADD_gain]))) / 512 + osc6.next() * (pgm_read_byte(&(ADD_gain_table[5][ADD_gain]))) / 512 + osc7.next() * (pgm_read_byte(&(ADD_gain_table[6][ADD_gain]))) / 512 + osc8.next() * (pgm_read_byte(&(ADD_gain_table[7][ADD_gain]))) / 512) * (gain_cv_val / 4));
  } else {  //chord
    return MonoOutput::fromNBit(16, ((osc1.next() / 8 + osc2.next() / 8 + osc3.next() / 8 + osc4.next() / 8 + osc5.next() / 8 * inv_aply5) * gain_cv_val));
  }
}

void loop() {
  audioHook();
}
