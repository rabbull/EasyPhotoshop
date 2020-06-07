#ifndef UNTITLED3_DCT_H
#define UNTITLED3_DCT_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

double **getImage(int length, int width);

double **DO_DCT(int length, int width, int block_bit, double **image);

double **DO_IDCT(int length, int width, int block_bit, double **dct);

double **DODCT2(double **image, int length, int width, int block_bit);

#endif //UNTITLED3_DCT_H
