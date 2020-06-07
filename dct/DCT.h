#ifndef UNTITLED3_DCT_H
#define UNTITLED3_DCT_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define PI  3.14159262

double** getImage(int length,int width);
double ** DO_DCT(int length, int width, int block_bit,double** image);//DCT操作
double ** DO_IDCT(int length,int width,int block_bit,double ** dct);//IDCT操作
double** DODCT2(double **image,int length,int width,int block_bit);

#endif //UNTITLED3_DCT_H
