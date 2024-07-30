#ifndef sample4_H_
#define sample4_H_
 
#include <Arduino.h>
#include "mozzi_pgmspace.h"
 
#define sample4_NUM_CELLS 33000
#define sample4_SAMPLERATE 4096
 
CONSTTABLE_STORAGE(int8_t) sample4_DATA [] = {44, 0, 40, 0, 42, -1, 44, 0, 43,
0, 46, 0, 50, 0, 52, -1, 51, 0, 49, 0, 49, 0, 52, 0, 53, 0, 53, 0, 54, 0, 51,
-1, 49, 0, 52, 0, 54, -1, 54, 0, 55, -1, 59, 0, 62, -1, 60, 0, 59, -1, 61, 0,
61, 0, 59, -1, 60, 0, 61, -1, 62, 0, 64, 0, 58, 0, 53, -1, 56, 0, 57, -1, 52, 0,
48, -1, 47, 0, 46, -1, 44, 0, 42, 0, 42, -1, 37, 0, 34, -1, 38, 0, 37, -1, 35,
0, 34, -1, 33, 0, 30, -1, 26, 0, 22, 0, 19, -1, 24, 0, 23, -1, 17, 0, 18, -1,
18, 0, 17, 0, 15, -1, 16, 0, 19, -1, 16, 0, 16, -1, 20, 0, 22, -1, 18, 0, 18,
-1, 22, 0, 24, -1, 27, 0, 25, -1, 27, 0, 32, -1, 32, 0, 33, 0, 37, 0, 43, 0, 45,
-1, 43, 0, 47, -1, 50, 0, 49, -1, 51, 0, 55, -1, 55, 0, 51, -1, 50, 0, 51, 0,
52, 0, 52, 0, 51, 0, 49, 0, 46, 0, 43, 0, 43, -1, 44, 0, 44, -1, 45, 0, 43, 0,
45, 0, 45, -1, 37, 0, 31, -1, 34, 0, 34, -1, 28, 0, 22, -1, 19, 0, 19, -1, 18,
0, 16, 0, 15, -1, 11, 0, 8, -1, 7, 0, 3, 0, 3, -1, 5, 0, 7, 0, 7, -1, 3, 0, -5,
-1, -5, 0, 0, 0, -1, -1, -6, 0, -7, -1, -7, 0, -6, -1, -6, 0, -10, -1, -11, 0,
-8, -1, -6, 0, -2, -1, 0, 0, -3, 0, 0, -1, 0, 0, 3, 0, 4, -1, -1, 0, -1, 0, 1,
-1, 4, 0, 5, -1, 6, 0, 11, -1, 13, 0, 13, 0, 12, -1, 17, 0, 20, -1, 19, 0, 21,
0, 19, -1, 18, 0, 19, -1, 22, 0, 25, 0, 24, -1, 22, 0, 20, -1, 19, 0, 19, -1,
16, 0, 14, -1, 15, 0, 17, 0, 18, -1, 19, 0, 20, -1, 11, 0, 6, -1, 7, 0, 6, -1,
2, 0, -2, -1, -5, 0, -7, -1, -11, 0, -14, 0, -14, -1, -14, 0, -14, -1, -16, 0,
-20, -1, -22, 0, -18, -1, -15, 0, -17, -1, -19, 0, -23, -1, -24, 0, -22, -1,
-24, 0, -28, -1, -29, 0, -28, -1, -26, 0, -30, -1, -34, 0, -35, 0, -34, -1, -32,
0, -29, -1, -29, 0, -29, -1, -25, 0, -25, -1, -23, 0, -21, 0, -23, 0, -24, 0,
-22, 0, -24, 0, -25, -1, -21, 0, -15, 0, -12, -1, -16, 0, -14, -1, -8, 0, -3, 0,
0, -1, 0, 0, -3, -1, -5, 0, -3, -1, -1, 0, -1, -1, 2, 0, 3, 0, 0, -1, -1, 0, -3,
-1, -5, 0, -7, 0, -5, -1, -4, 0, -6, -1, -5, 0, -6, 0, -11, -1, -13, 0, -12, 0,
-11, -1, -14, 0, -18, -1, -24, 0, -25, -1, -25, 0, -29, -1, -30, 0, -34, -1,
-35, -1, -37, 0, -40, -1, -38, 0, -34, -1, -36, 0, -39, 0, -39, 0, -41, -1, -42,
0, -40, -1, -40, 0, -41, -1, -41, 0, -41, 0, -40, -1, -42, 0, -46, -1, -46, 0,
-46, -1, -45, 0, -44, 0, -46, -1, -45, 0, -43, -1, -48, 0, -46, -1, -43, 0, -45,
0, -47, 0, -48, 0, -48, 0, -46, 0, -42, 0, -42, 0, -43, 0, -43, -1, -42, 0, -39,
-1, -36, 0, -32, -1, -30, 0, -31, -1, -27, 0, -25, 0, -25, -1, -24, 0, -19, -1,
-14, 0, -17, -1, -17, 0, -17, 0, -14, -1, -13, 0, -14, -1, -9, 0, -6, 0, -6, 0,
-6, -1, -10, 0, -12, -1, -8, 0, -5, -1, -5, 0, -7, 0, -9, -1, -9, 0, -8, -1, -9,
0, -13, -1, -17, 0, -16, -1, -13, 0, -18, -1, -19, 0, -16, -1, -18, 0, -18, -1,
-19, 0, -22, -1, -23, 0, -22, -1, -24, 0, -24, -1, -23, 0, -25, 0, -25, -1, -28,
0, -30, -1, -33, 0, -34, -1, -32, 0, -27, -1, -30, 0, -31, 0, -29, -1, -31, 0,
-29, 0, -31, -1, -36, 0, -36, -1, -35, 0, -35, 0, -32, 0, -31, -1, -31, 0, -31,
-1, -32, 0, -33, 0, -35, -1, -36, 0, -32, -1, -28, 0, -28, -1, -26, 0, -22, -1,
-22, 0, -20, -1, -13, 0, -11, -1, -13, 0, -11, -1, -7, 0, -2, 0, -5, 0, -5, -1,
-2, 0, 0, -1, 5, 0, 5, -1, 1, -1, 2, 0, 8, -1, 9, 0, 6, -1, 7, 0, 5, -1, 8, 0,
11, 0, 13, -1, 13, 0, 11, -1, 15, 0, 15, 0, 9, -1, 10, 0, 14, -1, 15, 0, 12, 0,
8, -1, 4, 0, 3, -1, 5, 0, 0, -1, 0, 0, -1, 0, -7, 0, -9, -1, -11, 0, -12, -1,
-15, 0, -15, -1, -10, 0, -8, -1, -12, -1, -12, 0, -11, -1, -12, 0, -11, 0, -11,
-1, -15, 0, -14, -1, -12, 0, -12, -1, -9, 0, -8, 0, -6, -1, -5, 0, -2, -1, 0, 0,
0, -1, 1, 0, 5, -1, 8, 0, 4, -1, 4, 0, 8, 0, 10, -1, 11, 0, 15, -1, 16, 0, 17,
0, 19, -1, 19, 0, 21, -1, 22, 0, 22, -1, 24, 0, 28, -1, 34, 0, 33, -1, 28, 0,
32, -1, 35, 0, 35, -1, 34, 0, 33, 0, 31, -1, 28, 0, 28, -1, 29, 0, 30, -1, 29,
-1, 31, 0, 32, -1, 28, 0, 25, -1, 28, 0, 33, -1, 33, 0, 29, -1, 27, 0, 30, 0,
30, -1, 23, -1, 21, 0, 23, -1, 25, 0, 23, -1, 19, 0, 16, 0, 16, 0, 15, -1, 18,
0, 23, 0, 18, 0, 17, 0, 16, 0, 14, -1, 18, 0, 18, -1, 14, 0, 15, -1, 14, 0, 10,
0, 11, -1, 15, 0, 15, -1, 12, 0, 11, 0, 14, -1, 19, 0, 17, -1, 20, 0, 23, -1,
20, 0, 20, -1, 21, 0, 22, 0, 25, -1, 29, 0, 29, -1, 33, 0, 34, 0, 34, -1, 36, 0,
38, -1, 40, 0, 43, -1, 46, 0, 51, 0, 51, -1, 48, 0, 49, -1, 53, 0, 53, -1, 49,
0, 47, 0, 46, -1, 45, 0, 44, -1, 43, 0, 43, -1, 39, 0, 40, -1, 36, 0, 35, -1,
33, 0, 34, 0, 34, 0, 34, -1, 31, 0, 26, -1, 26, 0, 28, 0, 21, -1, 19, 0, 18, 0,
17, -1, 17, 0, 15, -1, 14, 0, 14, -1, 11, 0, 13, -1, 15, 0, 13, -1, 14, 0, 15,
0, 13, -1, 14, 0, 14, -1, 6, -1, 8, 0, 7, -1, 4, 0, 6, -1, 10, 0, 10, -1, 8, 0,
8, 0, 13, 0, 16, 0, 14, 0, 17, -1, 21, 0, 22, -1, 23, 0, 26, -1, 26, 0, 28, -1,
31, 0, 29, -1, 30, 0, 26, -1, 26, 0, 29, -1, 28, 0, 28, -1, 31, 0, 38, -1, 45,
0, 43, 0, 36, -1, 36, 0, 41, 0, 42, -1, 37, 0, 33, -1, 32, 0, 33, -1, 31, 0, 30,
-1, 27, 0, 25, 0, 25, -1, 20, 0, 20, -1, 20, 0, 20, 0, 22, -1, 25, 0, 22, -1,
16, 0, 19, 0, 21, -1, 15, 0, 14, -1, 16, 0, 17, 0, 14, -1, 10, 0, 9, -1, 11, 0,
8, -1, 8, 0, 11, 0, 10, -1, 12, 0, 10, -1, 10, 0, 18, -1, 17, -1, 13, 0, 13, -1,
13, 0, 10, 0, 9, -1, 11, 0, 12, -1, 12, 0, 9, -1, 9, 0, 9, 0, 2, -1, 2, 0, 3,
-1, 2, 0, 1, -1, -1, 0, -3, 0, 0, 0, 0, 0, -5, -1, -6, 0, -7, -1, -10, 0, -13,
-1, -13, -1, -12, 0, -8, -1, -6, 0, 0, -1, 0, 0, -4, 0, -3, 0, 2, 0, 4, 0, 0,
-1, -3, 0, -1, 0, 5, -1, 4, 0, 1, -1, -2, 0, 1, 0, 1, -1, -3, 0, -4, 0, -3, -1,
-3, 0, -2, -1, -2, 0, -7, -1, -12, 0, -11, -1, -10, 0, -12, -1, -12, 0, -12, -1,
-11, 0, -12, -1, -18, 0, -20, -1, -20, 0, -19, -1, -18, 0, -20, 0, -21, 0, -15,
0, -17, 0, -20, 0, -16, 0, -16, 0, -20, 0, -19, 0, -17, -1, -16, 0, -15, -1,
-12, 0, -11, -1, -12, 0, -13, 0, -12, 0, -10, 0, -10, -1, -9, 0, -8, -1, -9, 0,
-6, -1, -4, 0, -6, -1, -2, 0, -1, 0, -5, -1, -7, 0, -7, -1, -7, 0, -11, -1, -11,
-1, -10, 0, -11, -1, -11, 0, -8, 0, -10, -1, -13, 0, -12, -1, -9, 0, -8, -1, -9,
0, -11, -1, -13, 0, -12, -1, -13, 0, -12, 0, -14, 0, -15, 0, -16, 0, -20, -1,
-22, 0, -21, -1, -22, 0, -22, -1, -20, 0, -22, -1, -25, 0, -26, 0, -26, -1, -28,
0, -26, -1, -25, 0, -23, -1, -24, 0, -28, -1, -31, 0, -33, -1, -34, 0, -31, -1,
-29, 0, -29, -1, -24, 0, -30, 0, -32, 0, -23, 0, -22, 0, -28, -1, -30, 0, -31,
-1, -30, 0, -27, 0, -25, -1, -26, 0, -27, 0, -28, -1, -25, 0, -22, 0, -22, -1,
-22, 0, -20, -1, -19, 0, -17, -1, -19, 0, -19, -1, -15, 0, -9, -1, -13, 0, -14,
0, -13, -1, -11, 0, -8, -1, -11, 0, -9, -1, -6, 0, -6, -1, -3, 0, -8, 0, -11,
-1, -10, 0, -6, -1, -3, 0, -5, -1, -10, 0, -11, 0, -8, -1, -6, 0, -8, 0, -10,
-1, -7, 0, -5, -1, -8, 0, -11, 0, -5, -1, -5, 0, -7, -1, -6, 0, -9, 0, -13, 0,
-12, 0, -12, -1, -15, 0, -16, -1, -20, 0, -20, -1, -19, 0, -22, 0, -27, -1, -32,
0, -35, -1, -33, 0, -35, 0, -39, -1, -35, 0, -39, -1, -41, 0, -39, 0, -42, -1,
-44, 0, -45, 0, -45, -1, -44, 0, -43, -1, -44, 0, -45, -1, -46, 0, -44, -1, -41,
0, -40, 0, -39, -1, -35, 0, -36, -1, -37, 0, -35, 0, -34, -1, -36, 0, -34, -1,
-27, 0, -30, 0, -31, -1, -30, 0, -24, 0, -20, -1, -22, 0, -20, -1, -15, 0, -8,
-1, -4, 0, -6, -1, -8, 0, -5, -1, 1, 0, 2, 0, 4, -1, 3, 0, 4, -1, 6, 0, 7, 0, 8,
-1, 6, 0, 8, 0, 13, -1, 9, 0, 6, -1, 7, 0, 9, 0, 11, -1, 11, 0, 8, 0, 5, 0, 8,
0, 7, -1, 4, 0, 4, -1, 0, 0, -2, -1, -5, 0, -9, -1, -12, 0, -14, -1, -15, 0,
-11, 0, -12, -1, -18, 0, -18, 0, -19, -1, -21, 0, -20, -1, -23, 0, -26, -1, -25,
0, -27, -1, -26, 0, -27, 0, -27, -1, -25, 0, -27, -1, -28, 0, -27, 0, -28, -1,
-26, 0, -21, 0, -20, -1, -21, 0, -21, 0, -22, -1, -27, 0, -27, -1, -25, 0, -26,
-1, -26, 0, -26, -1, -26, 0, -28, 0, -29, 0, -27, 0, -24, 0, -20, 0, -16, -1,
-15, 0, -14, 0, -11, -1, -9, 0, -6, -1, -8, 0, -9, 0, -8, -1, -6, 0, -4, 0, -1,
-1, -1, 0, 0, -1, 3, 0, 0, 0, -2, -1, 0, 0, 2, 0, 4, -1, 6, 0, 5, -1, 6, 0, 9,
0, 8, -1, 6, 0, 7, -1, 7, 0, 4, -1, 0, 0, -2, -1, -3, 0, -5, -1, -7, 0, -1, 0,
-3, 0, -8, -1, -6, 0, -9, -1, -11, 0, -10, -1, -12, 0, -11, -1, -13, 0, -18, -1,
-17, 0, -18, -1, -17, 0, -19, 0, -22, -1, -21, 0, -18, 0, -20, 0, -22, 0, -19,
0, -21, -1, -22, 0, -21, -1, -20, -1, -19, 0, -17, -1, -14, 0, -15, -1, -19, 0,
-22, -1, -21, 0, -22, 0, -24, -1, -25, 0, -26, -1, -22, 0, -15, -1, -19, 0, -23,
-1, -20, 0, -15, -1, -14, 0, -17, 0, -17, 0, -17, -1, -16, 0, -14, -1, -9, 0,
-13, -1, -16, 0, -14, -1, -15, 0, -16, 0, -17, 0, -15, 0, -10, 0, -6, 0, -9, 0,
-11, 0, -6, 0, -5, 0, -6, -1, -6, 0, -4, -1, -2, 0, -3, 0, -5, -1, -4, 0, -7,
-1, -8, 0, -5, -1, -4, 0, -8, -1, -7, 0, -11, -1, -13, 0, -8, -1, -13, 0, -15,
-1, -14, 0, -20, -1, -21, 0, -21, 0, -16, 0, -17, 0, -22, 0, -23, 0, -21, 0,
-22, 0, -25, 0, -23, 0, -20, 0, -22, 0, -25, 0, -26, -1, -25, 0, -21, -1, -19,
-1, -18, 0, -21, -1, -24, 0, -23, -1, -25, -1, -27, 0, -27, -1, -27, 0, -23, 0,
-17, -1, -21, 0, -26, -1, -24, 0, -18, -1, -15, -1, -16, 0, -18, -1, -16, 0,
-15, -1, -13, 0, -12, 0, -17, 0, -17, 0, -16, 0, -19, -1, -20, 0, -18, -1, -17,
0, -11, -1, -8, 0, -12, 0, -13, 0, -10, 0, -4, 0, -4, -1, -6, 0, -6, -1, -1, 0,
-1, -1, -3, 0, -4, -1, -8, 0, -12, -1, -10, -1, -10, 0, -12, -1, -11, 0, -15,
-1, -15, 0, -15, -1, -21, 0, -25, 0, -25, 0, -26, -1, -28, 0, -30, 0, -30, 0,
-32, -1, -36, 0, -37, -1, -33, 0, -36, -1, -39, 0, -37, 0, -37, -1, -38, 0, -37,
-1, -33, 0, -32, -1, -33, 0, -32, -1, -34, 0, -32, -1, -32, 0, -33, -1, -35, 0,
-35, 0, -33, 0, -33, -1, -29, 0, -22, -1, -27, 0, -28, -1, -26, 0, -23, -1, -22,
0, -25, -1, -25, 0, -19, -1, -18, 0, -17, -1, -13, 0, -12, -1, -11, 0, -8, -1,
-11, 0, -10, -1, -9, 0, -10, -1, -5, 0, -4, -1, -7, 0, -9, -1, -7, 0, -4, -1,
-6, 0, -7, -1, -2, 0, 3, -1, -1, 0, -5, -1, -3, 0, 0, -1, -1, 0, 0, -1, 2, 0, 3,
-1, 3, 0, 1, 0, 1, -1, 6, 0, 3, 0, -1, -1, -3, 0, -5, -1, -5, 0, -5, -1, -3, 0,
0, -1, -5, 0, -8, -1, -6, 0, -6, -1, -10, 0, -11, -1, -11, 0, -11, -1, -13, 0,
-16, 0, -16, -1, -12, 0, -9, -1, -10, 0, -13, -1, -14, 0, -17, -1, -21, 0, -21,
-1, -18, 0, -18, 0, -18, 0, -14, -1, -15, 0, -23, -1, -26, 0, -21, -1, -17, 0,
-22, -1, -26, 0, -22, -1, -22, 0, -21, 0, -19, -1, -22, 0, -20, 0, -17, -1, -18,
0, -16, -1, -13, 0, -14, -1, -9, 0, -6, -1, -8, 0, -10, 0, -8, -1, -5, 0, -3,
-1, -5, 0, -2, -1, 4, 0, 1, -1, 0, 0, 0, -1, -2, 0, 0, -1, 5, 0, 6, -1, 8, 0,
13, -1, 11, 0, 11, -1, 18, 0, 17, -1, 16, 0, 15, -1, 14, 0, 16, -1, 14, 0, 16,
-1, 17, 0, 14, -1, 13, 0, 14, 0, 14, -1, 12, 0, 11, -1, 11, 0, 13, -1, 12, 0,
10, -1, 9, 0, 10, -1, 12, 0, 10, 0, 6, -1, 8, 0, 10, 0, 8, -1, 8, 0, 10, -1, 7,
0, 10, -1, 18, 0, 19, -1, 13, 0, 11, 0, 16, 0, 18, -1, 13, 0, 8, -1, 8, 0, 8,
-1, 7, 0, 8, 0, 3, 0, 4, 0, 7, 0, 5, 0, 8, 0, 11, 0, 9, 0, 14, 0, 16, 0, 15, 0,
12, 0, 13, 0, 18, 0, 20, 0, 18, -1, 18, 0, 21, -1, 22, 0, 21, -1, 20, 0, 19, -1,
20, 0, 24, -1, 25, 0, 27, -1, 28, 0, 24, -1, 25, 0, 30, -1, 28, -1, 25, 0, 26,
-1, 32, 0, 38, -1, 35, 0, 37, 0, 40, 0, 37, 0, 34, -1, 34, 0, 38, 0, 41, 0, 43,
0, 45, -1, 48, 0, 43, -1, 41, 0, 41, -1, 43, 0, 47, -1, 45, 0, 41, -1, 41, 0,
43, -1, 39, 0, 37, -1, 40, 0, 41, -1, 40, 0, 41, 0, 38, -1, 35, 0, 35, -1, 40,
0, 42, -1, 38, 0, 35, -1, 35, 0, 34, 0, 36, -1, 37, 0, 34, -1, 39, 0, 41, -1,
33, 0, 32, 0, 36, 0, 37, 0, 38, -1, 37, 0, 37, -1, 38, 0, 37, -1, 37, 0, 42, -1,
39, 0, 39, -1, 39, 0, 40, -1, 40, 0, 40, -1, 39, 0, 41, -1, 43, 0, 39, -1, 37,
0, 40, -1, 37, 0, 36, -1, 37, 0, 37, -1, 38, 0, 37, 0, 39, 0, 42, -1, 38, 0, 41,
-1, 42, 0, 37, -1, 38, 0, 40, -1, 39, 0, 41, 0, 41, -1, 39, 0, 40, -1, 41, 0,
40, -1, 37, 0, 35, -1, 40, 0, 39, -1, 34, 0, 36, -1, 34, 0, 33, -1, 31, 0, 28,
-1, 29, 0, 34, -1, 39, -1, 38, 0, 35, -1, 33, 0, 37, -1, 39, 0, 37, -1, 36, 0,
39, -1, 42, 0, 46, 0, 48, 0, 45, -1, 47, 0, 49, -1, 44, 0, 44, -1, 45, 0, 46,
-1, 49, 0, 52, -1, 52, 0, 52, 0, 53, -1, 56, 0, 58, 0, 59, -1, 60, 0, 61, 0, 60,
-1, 59, 0, 56, -1, 51, 0, 52, 0, 55, -1, 54, 0, 53, 0, 52, -1, 53, 0, 55, -1,
58, 0, 57, -1, 58, 0, 55, -1, 53, 0, 56, -1, 55, 0, 56, 0, 57, -1, 53, 0, 49,
-1, 48, 0, 46, -1, 49, 0, 44, -1, 41, 0, 41, -1, 40, 0, 39, 0, 36, 0, 35, 0, 39,
-1, 38, 0, 32, -1, 31, 0, 31, -1, 29, 0, 28, 0, 29, 0, 29, -1, 31, 0, 31, -1,
30, 0, 26, -1, 22, 0, 27, -1, 30, 0, 25, 0, 21, -1, 21, 0, 24, 0, 32, -1, 37, 0,
33, -1, 32, 0, 34, 0, 29, -1, 29, 0, 28, -1, 27, 0, 33, -1, 33, 0, 31, -1, 28,
0, 26, -1, 27, 0, 29, -1, 29, 0, 29, -1, 29, 0, 31, -1, 35, 0, 33, -1, 27, 0,
28, -1, 35, 0, 36, -1, 32, 0, 32, -1, 32, -1, 31, 0, 31, -1, 27, 0, 29, 0, 25,
-1, 22, 0, 23, -1, 22, 0, 23, 0, 23, 0, 22, 0, 24, 0, 26, -1, 22, 0, 22, -1, 22,
0, 22, -1, 19, 0, 18, 0, 18, -1, 19, 0, 17, -1, 22, 0, 22, 0, 15, -1, 16, 0, 16,
-1, 13, 0, 11, 0, 10, 0, 8, 0, 12, -1, 15, 0, 10, -1, 5, 0, 6, 0, 8, 0, 4, 0,
-1, -1, 4, 0, 6, 0, 3, 0, 7, 0, 7, 0, 5, 0, 8, 0, 7, 0, 5, -1, 4, 0, 1, -1, 3,
0, 7, -1, 6, -1, 2, 0, 0, -1, 0, 0, 0, -1, -4, 0, -9, -1, -8, 0, -7, -1, -8, 0,
-9, -1, -13, 0, -14, -1, -15, 0, -14, -1, -15, 0, -18, -1, -16, 0, -18, 0, -19,
0, -17, 0, -20, 0, -19, -1, -18, 0, -20, -1, -20, 0, -20, 0, -14, -1, -11, 0,
-15, -1, -15, 0, -13, 0, -10, -1, -8, 0, -8, -1, -4, 0, -1, 0, -3, -1, -1, 0, 2,
-1, 4, 0, 11, 0, 11, -1, 7, 0, 5, -1, 4, 0, 4, -1, 3, 0, 1, -1, 0, 0, 4, -1, 2,
0, -2, -1, -9, 0, -10, -1, -7, 0, -10, -1, -17, 0, -18, -1, -20, 0, -25, 0, -21,
0, -19, 0, -25, 0, -26, 0, -25, 0, -28, 0, -27, 0, -27, 0, -24, -1, -19, 0, -23,
0, -26, 0, -29, 0, -33, 0, -32, -1, -33, 0, -36, 0, -35, -1, -35, 0, -38, 0,
-37, -1, -39, 0, -41, -1, -40, 0, -38, 0, -36, -1, -37, 0, -35, -1, -33, 0, -32,
-1, -32, 0, -36, -1, -38, 0, -37, 0, -36, -1, -35, 0, -34, -1, -28, 0, -26, 0,
-30, -1, -28, 0, -26, -1, -27, 0, -30, -1, -28, 0, -23, 0, -21, 0, -22, -1, -21,
0, -24, -1, -25, 0, -19, 0, -19, 0, -18, 0, -16, 0, -18, 0, -17, -1, -16, 0,
-18, -1, -20, 0, -18, -1, -13, -1, -15, 0, -22, -1, -24, 0, -20, -1, -22, 0,
-26, 0, -24, -1, -27, 0, -33, -1, -33, 0, -34, 0, -37, -1, -38, 0, -36, -1, -36,
0, -33, 0, -38, -1, -41, 0, -36, -1, -33, 0, -35, -1, -39, 0, -39, -1, -31, 0,
-30, -1, -33, 0, -27, 0, -30, 0, -33, 0, -33, 0, -36, -1, -37, 0, -37, -1, -37,
0, -33, 0, -31, 0, -35, -1, -37, 0, -33, 0, -28, -1, -30, 0, -34, -1, -34, 0,
-30, -1, -29, 0, -28, 0, -23, -1, -22, 0, -21, -1, -22, 0, -20, 0, -18, -1, -17,
0, -16, -1, -10, 0, -9, -1, -14, 0, -14, 0, -12, -1, -9, 0, -5, -1, -8, 0, -7,
-1, -1, 0, 0, 0, 0, 0, 1, -1, 3, 0, 3, 0, 5, -1, 7, 0, 8, -1, 5, 0, 4, -1, 12,
0, 13, -1, 5, 0, 1, -1, -1, 0, -1, 0, -1, 0, -2, -1, -6, 0, -8, -1, -10, 0, -17,
0, -18, -1, -19, 0, -18, -1, -16, 0, -17, -1, -20, 0, -23, 0, -22, -1, -16, 0,
-17, 0, -22, 0, -20, 0, -19, -1, -21, 0, -24, -1, -27, 0, -26, -1, -23, 0, -22,
-1, -21, 0, -23, 0, -23, 0, -22, -1, -22, 0, -20, -1, -21, 0, -15, 0, -13, -1,
-11, 0, -10, 0, -10, -1, -6, 0, -1, 0, -2, -1, -2, 0, 0, -1, 3, 0, 3, -1, 1, 0,
6, -1, 5, 0, 3, 0, 7, 0, 7, -1, 9, 0, 14, -1, 14, 0, 15, 0, 19, 0, 18, 0, 16, 0,
18, 0, 18, -1, 16, 0, 19, -1, 26, 0, 28, -1, 22, -1, 24, 0, 33, -1, 29, 0, 26,
0, 26, -1, 26, 0, 28, -1, 29, 0, 30, -1, 27, 0, 30, 0, 30, 0, 27, 0, 28, 0, 26,
-1, 26, 0, 29, -1, 34, 0, 34, -1, 29, 0, 26, -1, 30, 0, 26, -1, 20, 0, 22, -1,
25, 0, 27, -1, 25, 0, 23, -1, 21, 0, 23, -1, 20, -1, 19, 0, 18, -1, 18, 0, 12,
0, 6, -1, 8, 0, 5, -1, -1, 0, -2, 0, 3, 0, 0, -1, -3, 0, 0, -1, 5, 0, 5, -1, 2,
0, 2, 0, 10, -1, 14, 0, 10, -1, 14, 0, 18, -1, 17, 0, 17, -1, 17, 0, 21, 0, 29,
-1, 28, 0, 27, 0, 28, 0, 24, -1, 27, 0, 31, -1, 35, 0, 39, -1, 42, 0, 46, 0, 52,
0, 49, 0, 48, -1, 55, 0, 56, -1, 52, 0, 52, 0, 52, -1, 54, 0, 59, -1, 58, 0, 57,
0, 58, 0, 58, 0, 54, -1, 52, 0, 54, -1, 54, 0, 52, 0, 52, -1, 52, 0, 49, -1, 43,
0, 47, 0, 50, -1, 47, 0, 44, 0, 42, -1, 46, 0, 48, 0, 45, -1, 46, 0, 44, -1, 42,
0, 41, 0, 36, 0, 34, -1, 29, 0, 28, -1, 30, 0, 26, 0, 19, -1, 14, 0, 15, -1, 16,
0, 13, 0, 10, -1, 10, 0, 7, -1, 4, 0, 1, -1, 2, 0, 1, -1, -1, 0, 1, -1, 1, 0, 1,
0, 0, -1, -4, 0, -3, -1, 5, 0, 3, 0, 1, -1, 2, 0, 1, 0, 1, -1, -1, 0, -1, -1, 6,
0, 14, 0, 19, -1, 24, 0, 17, 0, 17, -1, 27, 0, 25, -1, 24, 0, 29, -1, 30, 0, 33,
-1, 38, 0, 37, -1, 36, 0, 43, -1, 47, 0, 43, -1, 41, 0, 43, -1, 45, 0, 43, -1,
45, -1, 46, 0, 39, -1, 38, 0, 40, 0, 37, -1, 33, 0, 34, 0, 34, -1, 34, 0, 31,
-1, 25, 0, 26, 0, 30, -1, 32, 0, 33, -1, 31, 0, 29, -1, 28, 0, 26, -1, 27, 0,
21, -1, 14, -1, 8, 0, 4, -1, 0, 0, -5, 0, -3, -1, -1, 0, -2, 0, -4, -1, -8, 0,
-4, -1, -2, -1, -5, 0, -6, -1, -7, 0, -7, -1, -8, -1, -13, 0, -12, -1, -4, 0,
-7, -1, -8, 0, -8, -1, -7, 0, -6, -1, -4, 0, -2, -1, 4, 0, 8, -1, 10, 0, 16, 0,
16, 0, 15, 0, 16, -1, 15, 0, 14, 0, 13, 0, 11, -1, 18, 0, 25, -1, 23, 0, 21, 0,
21, -1, 22, 0, 19, 0, 18, -1, 18, 0, 20, 0, 20, -1, 15, 0, 13, -1, 9, 0, 6, 0,
8, -1, 10, 0, 7, -1, 5, 0, 0, -1, 3, 0, 1, -1, -4, 0, -7, -1, -8, 0, -6, -1, -8,
0, -11, -1, -10, 0, -9, 0, -9, -1, -11, 0, -16, -1, -15, 0, -16, 0, -20, -1,
-21, 0, -19, -1, -21, 0, -20, -1, -20, 0, -25, -1, -25, 0, -27, -1, -28, 0, -30,
-1, -31, 0, -34, -1, -34, 0, -32, -1, -36, 0, -37, -1, -33, 0, -35, -1, -37, 0,
-38, -1, -37, 0, -36, -1, -37, 0, -37, 0, -35, -1, -35, 0, -30, -1, -28, 0, -32,
0, -30, -1, -23, 0, -25, 0, -24, -1, -19, 0, -23, -1, -25, 0, -21, 0, -17, 0,
-18, 0, -16, -1, -14, 0, -15, -1, -14, 0, -14, 0, -11, -1, -10, 0, -5, -1, -4,
0, -6, -1, -8, 0, -5, -1, -4, 0, -3, -1, 0, 0, -2, -1, -2, 0, -4, -1, -10, 0,
-15, -1, -16, 0, -12, -1, -10, 0, -17, 0, -15, -1, -17, 0, -25, -1, -23, 0, -26,
0, -28, 0, -27, -1, -28, 0, -28, -1, -28, 0, -126, -1, -31, 0, -35, -1, -37, 0,
-37, 0, -38, 0, -40, 0, -40, 0, -37, 0, -37, 0, -37, 0, -43, -1, -45, 0, -42, 0,
-39, 0, -41, 0, -46, -1, -46, 0, -45, -1, -42, 0, -43, 0, -44, -1, -38, 0, -33,
-1, -31, 0, -33, -1, -37, 0, -33, 0, -25, 0, -28, -1, -32, 0, -31, 0, -38, -1,
-37, 0, -33, 0, -31, -1, -34, 0, -29, 0, -23, -1, -23, 0, -19, -1, -17, 0, -19,
0, -15, 0, -11, -1, -13, 0, -14, 0, -15, -1, -11, 0, -8, -1, -13, 0, -11, -1,
-9, 0, -9, 0, -7, -1, -8, 0, -10, -1, -6, 0, -3, -1, -6, 0, -7, 0, -2, 0, -5, 0,
-3, -1, -2, 0, -9, 0, -11, 0, -12, -1, -13, 0, -12, -1, -11, 0, -9, -1, -5, 0,
-14, -1, -19, 0, -15, 0, -14, -1, -16, 0, -14, -1, -14, 0, -18, 0, -17, 0, -20,
-1, -28, 0, -26, 0, -22, -1, -24, 0, -27, -1, -32, 0, -34, -1, -34, 0, -38, -1,
-39, 0, -37, -1, -36, 0, -33, -1, -34, 0, -40, -1, -40, 0, -38, -1, -40, 0, -38,
-1, -36, 0, -38, -1, -37, 44, -31, -1, -26, 22, -30, 0, -27, -1, -115, 0, -24, -1,
-26, 0, -24, 0, -23, 0, -23, 0, -18, 0, -17, -1, -23, 0, -24, -1, -15, 0, -13,
-1, -17, 0, -12, -1, -10, 0, -10, 0, -8, -1, -7, 0, -5, 0, -4, 0, -1, 0, 4, -1,
6, 0, 5, -1, 7, 0, 10, -1, 13, 0, 14, 0, 13, 0, 10, 0, 9, 0, 7, -1, 6, 0, 12,
-1, 20, 0, 17, 0, 14, -1, 16, 0, 16, 0, 99, -1, 12, 0, 10, -1, 8, 0, 2, -1, -2,
-1, -3, 0, -65, -78, -88, 55, -22, -99, -12, 0, -11, -1, -12, 0, -16, 0, -19, -1,
-15, 0, -13, -1, -18, 0, -19, -1, -16, 0, -16, -1, -19, 0, -21, -1, -19, 0, -16,
0, -20, -1, -19, 0, -17, -1, -18, 0, -17, 0, -19, 0, -18, 0, -17, 0, -19, -1,
-17, 0, -12, -1, -4, 0, -2, 0, -3, 0, 0, 0, 0, 0, 1, 0, 6, 0, 6, 0, 4, 0, 3, 0,
1, -1, 3, 0, -1, -1, -4, 0, -4, -1, -2, -1, 5, 0, 6, -1, 4, 0, 9, -1, 11, 0, 13,
0, 8, 0, 4, -1, 6, 0, 5, -1, 0, 0, -2, -1, 0, -1, 0, 0, -5, -1, -8, 0, -3, 0,

};

#endif /* sample4_H_ */
