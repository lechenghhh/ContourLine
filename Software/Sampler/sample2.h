#ifndef sample2_H_
#define sample2_H_
 
#include <Arduino.h>
#include "mozzi_pgmspace.h"
 
#define sample2_NUM_CELLS 24000
#define sample2_SAMPLERATE 4096
 
CONSTTABLE_STORAGE(int8_t) sample2_DATA [] = {1, 0, 9, 0, 16, 0, 24, 0, 31, 0,
39, 0, 46, 0, 52, 0, 58, -1, 64, 0, 70, 0, 75, 0, 79, 0, 84, 0, 87, -1, 90, 0,
92, -1, 93, 0, 93, 0, 92, 0, 89, -1, 86, 0, 83, -1, 79, 0, 74, 0, 69, -1, 64, 0,
59, -1, 54, 0, 49, -1, 45, 0, 40, -1, 35, 0, 30, -1, 24, 0, 19, 0, 13, -1, 6, 0,
-1, -1, -8, 0, -15, -1, -21, 0, -26, 0, -31, 0, -35, 0, -40, 0, -43, -1, -47, 0,
-51, -1, -54, 0, -57, 0, -60, 0, -62, -1, -64, 0, -65, -1, -67, 0, -68, 0, -69,
-1, -70, 0, -70, 0, -70, -1, -69, 0, -69, 0, -69, -1, -69, 0, -68, -1, -67, 0,
-66, -1, -64, 0, -62, 0, -59, -1, -57, 0, -55, -1, -53, 0, -51, -1, -49, 0, -48,
-1, -46, 0, -45, -1, -44, 0, -43, -1, -41, 0, -40, -1, -39, 0, -37, -1, -35, 0,
-33, -1, -29, 0, -26, 0, -21, -1, -16, 0, -10, -1, -3, 0, 4, -1, 12, 0, 19, -1,
27, 0, 34, -1, 40, 0, 46, -1, 50, 0, 54, -1, 58, 0, 61, 0, 63, -1, 65, 0, 67,
-1, 69, 0, 70, -1, 70, 0, 70, 0, 68, -1, 66, 0, 63, 0, 59, -1, 54, 0, 49, -1,
43, 0, 36, -1, 29, 0, 22, -1, 14, 0, 7, -1, 1, 0, -5, -1, -10, 0, -15, -1, -19,
0, -23, 0, -27, -1, -31, 0, -34, -1, -37, 0, -40, -1, -43, 0, -44, -1, -45, 0,
-46, 0, -45, 0, -45, 0, -44, -1, -43, 0, -42, -1, -41, 0, -40, 0, -40, -1, -39,
0, -39, -1, -38, 0, -38, -1, -39, 0, -39, 0, -39, -1, -39, 0, -39, -1, -39, 0,
-40, 0, -40, 0, -41, -1, -41, 0, -43, -1, -44, 0, -46, 0, -48, 0, -51, 0, -53,
-1, -55, 0, -56, -1, -58, 0, -59, -1, -60, -1, -60, 0, -61, -1, -62, 0, -63, -1,
-65, 0, -66, -1, -67, 0, -67, -1, -66, 0, -65, 0, -62, 0, -60, 0, -57, 0, -54,
-1, -51, 0, -46, -1, -41, 0, -35, -1, -28, 0, -21, 0, -13, 0, -5, -1, 3, 0, 10,
0, 17, -1, 24, 0, 31, -1, 37, 0, 42, -1, 47, 0, 51, 0, 55, -1, 58, 0, 61, -1,
62, -1, 63, 0, 64, -1, 63, 0, 62, -1, 60, 0, 57, -1, 54, 0, 49, 0, 44, -1, 39,
0, 34, -1, 30, 0, 27, 0, 25, 0, 23, 0, 22, 0, 21, -1, 21, 0, 21, 0, 21, 0, 21,
0, 20, -1, 19, 0, 17, -1, 15, 0, 12, 0, 9, -1, 7, 0, 6, -1, 5, 0, 6, -1, 7, 0,
8, 0, 10, -1, 11, 0, 11, -1, 12, 0, 12, 0, 11, -1, 10, 0, 9, -1, 7, 0, 5, -1, 3,
0, 1, -1, -1, 0, -3, -1, -5, 0, -7, 0, -10, 0, -13, 0, -17, 0, -20, 0, -23, -1,
-26, 0, -29, -1, -31, 0, -32, -1, -33, 0, -34, 0, -34, -1, -34, 0, -34, -1, -34,
0, -34, -1, -34, 0, -34, 0, -33, -1, -33, 0, -32, -1, -31, 0, -30, -1, -28, 0,
-26, 0, -24, 0, -22, 0, -19, -1, -16, 0, -13, -1, -10, 0, -6, -1, -3, 0, 0, 0,
3, -1, 7, 0, 10, -1, 13, 0, 16, 0, 19, -1, 22, 0, 25, 0, 28, -1, 31, 0, 34, 0,
38, -1, 41, 0, 43, -1, 45, 0, 45, 0, 45, -1, 44, 0, 43, -1, 40, 0, 38, -1, 35,
0, 32, 0, 30, -1, 27, 0, 25, -1, 24, 0, 23, -1, 23, 0, 23, 0, 23, 0, 24, -1, 25,
0, 26, -1, 27, 0, 27, 0, 28, -1, 27, 0, 26, -1, 25, 0, 24, 0, 23, -1, 21, 0, 19,
0, 18, -1, 16, 0, 15, 0, 13, -1, 12, 0, 10, 0, 9, -1, 7, 0, 5, -1, 3, 0, 0, -1,
-2, 0, -4, 0, -6, -1, -8, 0, -10, 0, -11, -1, -12, 0, -13, 0, -14, -1, -16, 0,
-18, -1, -20, 0, -23, 0, -27, 0, -31, 0, -34, -1, -38, 0, -40, -1, -42, 0, -42,
-1, -42, 0, -42, -1, -41, 0, -40, -1, -39, 0, -38, -1, -36, 0, -34, -1, -31, 0,
-28, 0, -24, -1, -20, 0, -16, -1, -12, 0, -7, -1, -2, 0, 2, -1, 7, 0, 12, -1,
16, 0, 19, -1, 22, 0, 25, 0, 28, 0, 31, -1, 35, 0, 39, -1, 43, 0, 48, -1, 53, 0,
57, 0, 61, -1, 64, 0, 65, -1, 66, 0, 65, -1, 64, 0, 62, -1, 60, 0, 57, -1, 54,
0, 50, -1, 45, 0, 39, 0, 34, -1, 28, 0, 22, -1, 17, 0, 11, -1, 6, 0, 1, -1, -3,
0, -7, -1, -11, 0, -16, -1, -22, 0, -29, -1, -36, 0, -43, 0, -50, 0, -56, -1,
-62, 0, -66, -1, -68, 0, -70, 0, -70, -1, -69, 0, -68, -1, -67, 0, -66, 0, -65,
-1, -65, 0, -64, -1, -63, 0, -62, -1, -61, -1, -60, 0, -58, -1, -55, 0, -53, 0,
-49, 0, -46, 0, -42, -1, -38, 0, -34, -1, -30, 0, -26, -1, -22, -1, -18, 0, -15,
-1, -11, 0, -8, 0, -6, -1, -4, 0, -3, -1, -2, 0, -1, 0, -1, 0, -1, 0, 0, -1, 0,
0, 1, 0, 2, 0, 4, 0, 5, -1, 7, 0, 8, -1, 9, 0, 10, -1, 11, 0, 14, 0, 16, -1, 20,
0, 25, -1, 29, 0, 34, -1, 39, 0, 43, -1, 47, 0, 50, -1, 53, 0, 55, -1, 56, 0,
57, 0, 57, 0, 57, 0, 57, -1, 57, -1, 56, 0, 55, -1, 55, 0, 54, -1, 52, 0, 50,
-5, -13, -13, -22, -22, -31, -31, -39, -39, -45, -45, -50, -50, -53, -53, -54,
13, 0, 9, -1, 6, 0, 3, -1, 1, 0, -2, 0, -4, -1, -6, 0, -7, -1, -8, 0, -9, -1,
-8, 0, -8, -1, -6, 0, -5, 0, -4, -1, -2, 0, -1, 0, 0, 0, 1, -1, 1, 0, 2, 0, 2,
-1, 1, 0, 1, -1, 0, 0, -1, 0, -2, -1, -3, 0, -4, -1, -6, 0, -7, -1, -9, 0, -11,
-1, -13, 0, -16, 0, -19, -1, -23, 0, -27, -1, -32, 0, -38, 0, -44, -1, -51, 0,
-59, 0, -66, 0, -73, 0, -78, 0, -83, 0, -87, -1, -91, 0, -94, -1, -96, 0, -98,
0, -99, -1, -100, 0, -100, -1, -100, 0, -98, -1, -97, 0, -94, -1, -90, 0, -86,
-1, -81, 0, -75, -1, -69, 0, -62, -1, -54, 0, -46, -1, -37, 0, -27, 0, -16, -1,
-5, 0, 7, -1, 17, 0, 28, 0, 37, 0, 46, -1, 53, 0, 60, -1, 65, 0, 69, -1, 72, -1,
75, 0, 77, -1, 78, 0, 79, -1, 80, 0, 80, -1, 80, 0, 79, 0, 77, 0, 75, 0, 72, 0,
69, -1, 65, 0, 62, -1, 59, 0, 57, -1, 55, 0, 54, -1, 53, 0, 52, 0, 51, 0, 49,
-1, 46, 0, 42, -1, 38, 0, 34, 0, 29, -1, 24, 0, 19, -1, 15, 0, 10, -1, 6, 0, 3,
-1, -1, 0, -4, -1, -7, 0, -10, 0, -13, 0, -16, 0, -19, -1, -22, 0, -25, -1, -27,
0, -29, -1, -32, 0, -34, -1, -36, 0, -37, 0, -39, -1, -41, 0, -44, -1, -46, 0,
-49, 0, -52, -1, -55, 0, -58, -1, -59, -1, -60, 0, -60, -1, -59, 0, -56, -1,
-53, 0, -49, -1, -45, 0, -42, -1, -40, 0, -38, -1, -37, 0, -35, -1, -33, 0, -31,
-1, -28, 0, -25, -1, -21, 0, -18, -1, -14, 0, -9, -1, -5, 0, 1, 0, 6, -1, 12, 0,
18, 0, 24, -1, 30, 0, 34, 0, 38, -1, 40, 0, 42, -1, 43, 0, 43, -1, 43, 0, 43,
-2, -2, -3, -3, -5, -5, -8, -8, -12, -12, -18, -18, -24, -24, -31, -31, -37,
-37, -41, -41, -44, -44, -45, -45, -44, -44, -42, -42, -40, -40, -38, -38, -36,
-36, -35, -35, -33, -33, -30, -30, -27, -27, -24, -24, -20, -20, -16, -16, -11,
-11, -7, -7, -3, -3, 0, 0, 1, 1, 0, 0, -3, -3, -8, -8, -13, -13, -18, -18, -23,
-23, -26, -26, -28, -28, -28, -29, -28, -28, -26, -26, -24, -24, -21, -21, -19,
-19, -17, -17, -14, -14, -12, -12, -10, -10, -6, -6, -3, -3, 2, 2, 7, 7, 13, 13,
19, 19, 24, 24, 28, 29, 31, 31, 33, 33, 34, 34, 33, 33, 31, 31, 27, 27, 23, 23,
19, 19, 15, 15, 12, 12, 10, 10, 10, 10, 11, 11, 14, 14, 17, 17, 21, 21, 24, 24,
25, 25, 25, 25, 24, 24, 22, 22, 20, 20, 18, 18, 17, 17, 15, 15, 15, 15, 15, 15,
17, 17, 19, 19, 23, 23, 27, 27, 31, 31, 34, 34, 35, 35, 35, 35, 34, 34, 32, 32,
30, 30, 29, 29, 28, 28, 28, 28, 30, 30, 32, 32, 34, 34, 35, 35, 35, 35, 33, 33,
30, 30, 25, 25, 21, 21, 17, 17, 13, 13, 11, 11, 9, 9, 8, 8, 7, 7, 5, 5, 1, 1,
-4, -4, -10, -10, -18, -18, -26, -26, -34, -34, -40, -40, -45, -45, -47, -47,
-47, -47, -46, -46, -43, -43, -41, -41, -39, -39, -37, -37, -36, -36, -35, -35,
-35, -35, -35, -35, -35, -35, -35, -35, -34, -34, -34, -34, -33, -33, -32, -32,
-33, -33, -34, -34, -35, -35, -36, -36, -36, -36, -36, -36, -36, -36, -35, -35,
-35, -35, -35, -35, -35, -35, -36, -36, -36, -36, -36, -36, -36, -36, -36, -36,
-34, -34, -30, -30, -25, -25, -17, -17, -7, -7, 4, 4, 15, 15, 27, 27, 37, 37,
46, 46, 52, 52, 55, 55, 54, 54, 49, 49, 41, 41, 32, 32, 23, 23, 15, 15, 10, 10,
8, 8, 8, 8, 10, 10, 13, 13, 16, 16, 19, 19, 21, 21, 22, 22, 22, 22, 20, 20, 19,
19, 18, 18, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 17, 17, 17, 17, 15, 15, 13,
13, 11, 11, 10, 10, 10, 10, 11, 11, 12, 12, 14, 14, 16, 16, 17, 17, 18, 18, 19,
19, 19, 19, 18, 18, 17, 17, 15, 15, 14, 14, 13, 13, 11, 11, 10, 10, 10, 10, 10,
10, 11, 11, 12, 12, 14, 14, 15, 15, 16, 16, 16, 16, 14, 14, 11, 11, 5, 5, -1,
-42, 0, -44, 0, -46, -1, -49, 0, -52, -1, -55, 0, -59, -1, -62, 0, -66, -1, -70,
0, -73, 0, -77, -1, -79, 0, -82, -1, -83, 0, -84, -1, -84, 0, -83, -1, -81, 0,
-79, 0, -75, -1, -71, 0, -66, -1, -60, 0, -54, -1, -48, 0, -41, 0, -34, 0, -27,
0, -20, 0, -11, -1, -3, 0, 6, -1, 15, 0, 25, 0, 35, 0, 45, 0, 55, 0, 64, -1, 72,
0, 80, -1, 86, 0, 92, -1, 96, 0, 98, -1, 100, 0, 100, -1, 99, 0, 97, 0, 94, 0,
90, -1, 86, 0, 80, -1, 75, 0, 69, -1, 64, 0, 58, -1, 53, 0, 48, -1, 43, 0, 39,
-1, 35, 0, 32, 0, 29, 0, 25, 0, 22, 0, 18, -1, 14, 0, 10, -1, 5, -1, 0, 0, -4,
-1, -9, 0, -13, -1, -17, -1, -20, 0, -23, -1, -25, 0, -26, 0, -27, -1, -28, 0,
-28, 0, -28, -1, -27, 0, -27, -1, -27, 0, -28, 0, -28, -1, -29, 0, -29, -1, -29,
0, -28, -1, -27, 0, -25, -1, -23, 0, -21, -1, -18, 0, -16, -1, -13, 0, -11, -1,
-9, 0, -8, -1, -6, 0, -5, 0, -4, -1, -2, 0, -1, -1, 1, 0, 2, -1, 4, 0, 5, -1, 6,
0, 7, -1, 9, 0, 9, -1, 10, 0, 11, -1, 12, 0, 13, -1, 14, 0, 14, 0, 15, 0, 16,
-1, 17, 0, 18, -1, 20, 0, 22, 0, 25, 0, 28, 0, 30, 0, 33, -1, 35, 0, 37, -1, 38,
0, 39, 0, 39, -1, 38, 0, 38, -1, 37, 0, 36, 0, 34, -1, 33, 0, 31, -1, 29, 0, 27,
-1, 24, 0, 22, -1, 19, 0, 16, 0, 12, -1, 9, 0, 6, -1, 4, 0, 1, -1, 0, 0, -2, -1,
-2, 0, -3, 0, -2, 0, -1, -1, 1, 0, 3, 0, 5, 0, 8, 0, 10, 0, 13, -1, 16, 0, 18,
0, 20, -1, 22, 0, 24, -1, 25, 0, 26, 0, 26, 0, 27, 0, 27, -1, 27, 0, 26, -1, 26,
0, 24, -1, 23, 0, 21, 0, 20, -1, 18, 0, 16, -1, 15, 0, 14, -1, 13, 0, 13, -1,
13, -1, 13, 0, 13, -1, 12, 0, 12, -1, 10, 0, 8, -1, 6, 0, 2, 0, -1, -1, -5, 0,
-9, -1, -13, 0, -16, 0, -20, 0, -23, -1, -26, 0, -28, -1, -30, 0, -32, -1, -34,
0, -36, -1, -37, 0, -39, 0, -40, 0, -40, -1, -41, 0, -41, -1, -41, 0, -40, -1,
-38, 0, -36, -1, -32, 0, -28, -1, -23, 0, -17, 0, -10, 0, -3, 0, 5, 0, 13, 0,
20, 0, 28, 0, 35, 0, 41, 0, 47, -1, 52, 0, 57, -1, 62, 0, 66, -1, 70, 0, 73, -1,
75, 0, 77, -1, 77, 0, 77, -1, 76, 0, 74, -1, 71, 0, 68, -1, 64, 0, 59, 0, 55,
-1, 50, 0, 45, -1, 40, 0, 35, 0, 30, 0, 25, -1, 20, 0, 15, -1, 10, 0, 4, 0, -3,
-1, -9, 0, -16, -1, -22, 0, -28, 0, -34, -1, -39, 0, -44, -1, -48, 0, -52, -1,
-56, 0, -60, -1, -63, 0, -66, -1, -69, 0, -71, 0, -73, -1, -75, 0, -76, -1, -77,
0, -78, 0, -78, -1, -78, 0, -78, -1, -77, 0, -76, -1, -75, 0, -74, 0, -73, -1,
-71, 0, -69, 0, -67, -1, -65, 0, -63, -1, -60, 0, -58, -1, -55, 0, -53, -1, -51,
0, -50, 0, -48, -1, -47, 0, -45, -1, -44, 0, -42, 0, -41, 0, -39, -1, -37, 0,
-36, -1, -34, 0, -31, -1, -28, 0, -25, -1, -21, 0, -16, -1, -10, 0, -4, -1, 3,
0, 10, -1, 17, 0, 24, -1, 31, 0, 37, -1, 43, 0, 47, -1, 51, 0, 55, 0, 57, -1,
60, 0, 61, -1, 63, 0, 64, 0, 64, 0, 64, -1, 63, 0, 61, -1, 59, 0, 56, 0, 52, -1,
48, 0, 43, -1, 37, 0, 30, -1, 24, 0, 17, 0, 10, 0, 3, 0, -3, 0, -9, -1, -14, 0,
-18, 0, -23, 0, -27, 0, -30, 0, -34, -1, -37, 0, -39, 0, -42, -1, -44, 0, -45,
-1, -46, 0, -47, 0, -47, 0, -46, 0, -46, -1, -45, 0, -44, -1, -43, 0, -42, -1,
-41, 0, -40, -1, -39, 0, -39, -1, -38, 0, -38, -1, -37, 0, -37, 0, -36, 0, -36,
0, -36, 0, -35, -1, -35, 0, -35, -1, -36, 0, -36, -1, -37, 0, -38, -1, -40, 0,
-41, -1, -43, 0, -44, -1, -45, 0, -47, 0, -48, -1, -49, 0, -50, -1, -51, 0, -52,
0, -53, -1, -54, 0, -55, -1, -55, 0, -55, -1, -54, 0, -53, -1, -51, 0, -49, -1,
-47, 0, -44, 0, -40, -1, -36, 0, -31, -1, -25, 0, -18, -1, -11, 0, -4, -1, 4, 0,
12, 0, 19, 0, 26, -1, 33, 0, 40, -1, 45, 0, 50, 0, 55, 0, 59, -1, 62, 0, 65, -1,
67, 0, 68, -1, 69, 0, 69, 0, 69, -1, 68, 0, 66, 0, 63, -1, 60, 0, 55, -1, 51, 0,
46, -1, 42, 0, 39, -1, 35, 0, 33, -1, 30, 0, 29, 0, 28, -1, 27, 0, 27, -1, 27,
0, 26, -1, 25, 0, 24, -1, 22, 0, 20, -1, 17, 0, 15, 0, 13, 0, 11, -1, 10, 0, 10,
-1, 10, 0, 11, 0, 11, -1, 12, 0, 12, -1, 12, 0, 11, -1, 11, 0, 10, -1, 8, 0, 7,
-1, 5, 0, 3, 0, 2, -1, 0, 0, -2, -1, -5, 0, -7, 0, -9, 0, -12, 0, -14, 0, -17,
0, -19, -1, -21, 0, -23, -1, -24, 0, -26, 0, -27, -1, -28, 0, -28, -1, -29, 0,
-29, -1, -30, 0, -30, -1, -30, 0, -30, 0, -29, 0, -29, -1, -28, 0, -27, 0, -26,
0, -24, 0, -23, 0, -21, -1, -18, 0, -16, 0, -13, 0, -10, 0, -7, 0, -3, -1, 0, 0,
4, -1, 7, 0, 11, 0, 14, 0, 18, -1, 21, 0, 24, -1, 27, 0, 30, -1, 33, 0, 36, -1,
39, 0, 42, 0, 44, -1, 46, 0, 47, 0, 47, -1, 47, 0, 46, -1, 45, 0, 42, -1, 40, 0,
37, -1, 35, 0, 32, 0, 30, 0, 28, 0, 27, 0, 26, -1, 25, 0, 25, -1, 26, 0, 27, -1,
27, 0, 28, -1, 28, 0, 28, 0, 28, -1, 28, 0, 27, -1, 25, 0, 24, -1, 22, 0, 20,
-1, 18, 0, 17, -1, 15, 0, 13, -1, 12, 0, 10, -1, 9, 0, 7, -1, 5, 0, 4, -1, 2, 0,
-1, -1, -3, 0, -5, -1, -6, 0, -8, 0, -9, 0, -10, 0, -11, 0, -12, -1, -13, 0,
-14, -1, -16, 0, -18, -1, -20, 0, -23, 0, -25, -1, -28, 0, -31, -1, -33, 0, -34,
0, -35, -1, -36, 0, -36, -1, -36, 0, -36, 0, -35, -1, -34, 0, -32, 0, -30, 0,
-28, -1, -25, 0, -22, -1, -18, 0, -14, -1, -11, 0, -6, -1, -2, 0, 2, 0, 6, 0,
10, -1, 14, 0, 18, -1, 21, 0, 25, 0, 28, -1, 32, 0, 36, -1, 40, 0, 44, -1, 48,
0, 52, -1, 56, 0, 58, 0, 60, -1, 62, 0, 62, -1, 62, 0, 61, -1, 60, 0, 58, -1,
55, 0, 51, 0, 47, -1, 42, 0, 37, -1, 32, 0, 26, -1, 21, 0, 15, -1, 10, 0, 6, -1,
1, 0, -3, -1, -7, 0, -12, 0, -17, 0, -23, 0, -29, 0, -35, -1, -41, 0, -47, -1,
-53, 0, -57, -1, -61, 0, -63, -1, -64, 0, -65, -1, -64, -1, -64, 0, -63, -1,
-62, 0, -61, -1, -60, 0, -59, -1, -58, 0, -57, 0, -55, 0, -54, 0, -51, -1, -48,
0, -45, -1, -42, 0, -38, 0, -34, -1, -30, 0, -25, 111, -21, 0, -16, -1, -12, 0,
-8, -1, -4, -1, -1, 0, 1, -1, 3, 0, 5, -1, 6, 0, 6, 111, 7, -1, 7, 0, 8, -1, 8, 0,
9, -1, 10, 0, 10, -1, 11, 0, 12, 0, 13, 0, 14, 0, 15, -1, 16, 0, 17, 0, 19, 0,
22, 0, 25, 0, 29, 0, 33, -1, 37, 0, 41, -1, 45, -1, 49, 0, 52, -1, 54, 0, 56,
-1, 57, -1, 58, 100, 59, -1, 59, 120, 58, -1, 58, 0, 57, 0, 57, -1, 55, 0, 54, -1,
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
-1, -23, 0, -23, -1, -23, 0, -23, -1, -23, 0, -24, -1, -24, 0, -25, -1, -25, 0,
-25, 0, -25, -1, -24, 0, -23, -1, -22, 0, -20, -1, -18, 0, -16, -1, -14, 0, -11,
0, -9, 0, -6, 0, -4, 0, -1, -1, 1, 0, 4, -1, 6, 0, 9, -1, 11, 0, 12, 0, 13, 0,
14, 0, 14, -1, 13, 0, 12, -1, 11, 0, 9, 0, 7, -1, 6, 0, 4, -1, 3, 0, 1, -1, 0,
0, -2, -1, -3, 0, -4, -1, -6, 0, -7, 0, -8, 0, -9, 0, -10, 0, -10, -1, -9, 0,
-8, -1, -6, 0, -4, 0, -1, 0, 1, -1, 4, 0, 6, -1, 9, 0, 12, -1, 15, -1, 18, 0,
21, -1, 24, 0, 27, 0, 30, -1, 33, 0, 35, 0, 38, -1, 40, 0, 41, -1, 43, 0, 44,
-1, 44, 0, 43, -1, 42, 0, 40, -1, 37, 0, 33, -1, 29, 0, 25, -1, 20, 0, 15, 0,
11, -1, 8, 0, 5, -1, 2, 0, 1, -1, -1, 0, -2, 0, -3, -1, -5, 0, -7, -1, -9, 0,
-12, -1, -15, 0, -19, -1, -22, 0, -26, -1, -29, 0, -32, -1, -34, 0, -36, -1,
-38, 0, -39, -1, -39, 0, -39, 0, -39, -1, -38, 0, -38, -1, -37, 0, -36, 0, -35,
-1, -35, 0, -34, 0, -33, -1, -32, 0, -31, -1, -30, 0, -29, -1, -28, 0, -27, -1,
-25, 0, -23, -1, -21, 0, -19, -1, -17, 0, -16, 0, -15, 0, -15, -1, -15, 0, -16,
0, -18, 0, -20, 0, -22, 0, -25, -1, -28, 0, -31, 0, -33, 0, -36, 0, -37, 0, -38,
-1, -38, 0, -37, 0, -35, 0, -32, 0, -28, 0, -24, -1, -19, 0, -14, -1, -8, 0, -3,
0, 3, 0, 10, -1, 16, 0, 23, -1, 29, 0, 35, -1, 41, 0, 46, 0, 51, -1, 55, 0, 59,
0, 63, 0, 66, -1, 69, 0, 71, -1, 72, 0, 73, 0, 74, -1, 73, 0, 72, -1, 71, 0, 68,
-1, 66, 0, 63, -1, 59, 0, 56, -1, 53, 0, 49, -1, 46, 0, 43, 0, 40, -1, 37, 0,
34, 0, 31, -1, 27, 0, 24, -1, 20, 0, 17, 0, 13, 0, 9, 0, 6, 0, 2, 0, 0, -1, -3,
0, -5, -1, -6, 0, -7, -1, -7, 0, -8, -1, -8, 0, -9, -1, -10, 0, -11, 0, -13, -1,
-15, 0, -18, 0, -20, 0, -23, -1, -26, 0, -28, -1, -31, 0, -33, 0, -35, -1, -36,
0, -38, -1, -39, 0, -41, -1, -42, 0, -44, 0, -46, -1, -49, 0, -51, -1, -54, 0,
-57, -1, -61, 0, -64, 0, -67, 0, -70, -1, -73, 0, -75, -1, -76, 0, -76, -1, -76,
0, -75, 0, -72, -1, -69, 0, -66, 0, -61, 0, -56, 0, -51, 0, -45, 0, -39, -1,
-33, 0, -26, -1, -19, 0, -11, 0, -2, -1, 6, 0, 16, 0, 25, -1, 35, 0, 44, -1, 53,
0, 62, -1, 70, 0, 77, -1, 83, 0, 88, -1, 92, 0, 95, 0, 97, 0, 98, -1, 97, 0, 96,
-1, 94, 0, 91, 0, 87, -1, 82, 0, 77, 0, 71, -1, 66, 0, 60, -1, 55, -1, 50, 0,
45, -1, 40, 0, 36, -1, 32, 0, 28, 0, 25, -1, 21, 0, 17, -1, 13, 0, 9, -1, 5, 0,
0, -1, -4, 0, -8, -1, -13, 0, -16, -1, -19, 0, -22, -1, -24, 0, -26, -1, -28, 0,
-28, -1, -29, 0, -29, -1, -30, 0, -30, -1, -30, 0, -30, -1, -31, 0, -31, -1,
-31, 0, -31, 0, -30, -1, -29, 0, -27, 0, -25, -1, -23, 0, -20, 0, -18, -1, -15,
0, -13, -1, -11, 0, -9, -1, -7, 0, -6, 0, -5, -1, -4, 0, -3, -1, -1, 0, 0, -1,
1, 0, 2, -1, 3, 0, 4, -1, 5, 0, 6, -1, 7, 0, 8, 0, 8, -1, 9, 0, 9, -1, 10, 0,
10, -1, 11, 0, 12, -1, 13, 0, 15, -1, 17, 0, 19, 0, 22, -1, 24, 0, 27, -1, 29,

0, 39, 0, };

#endif /* sample2_H_ */
