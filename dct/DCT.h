#ifndef UNTITLED3_DCT_H
#define UNTITLED3_DCT_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define PI  3.14159262

double** getImage(int length,int width);//获取测试的图像矩阵，最终不使用
double ** DO_DCT(int length, int width, int block_bit,double** image);//输入图像的长、宽、要求分块的大小、原始图像矩阵，返回DCT矩阵
double ** DO_IDCT(int length,int width,int block_bit,double ** dct);//输入图像的长、宽、要求分块的大小、DCT处理后图像矩阵，返回IDCT矩阵
double** DODCT2(double **image,int length,int width,int block_bit);//输入DCT处理后图像、图像的长、宽、要求分块的大小，返回IDCT2矩阵

#endif //UNTITLED3_DCT_H
