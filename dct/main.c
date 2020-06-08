//
// Created by intoblue on 2020/6/7.
//

#include "imgproc/DCT.h"

int main()//测试用
{
    int imagelength=11;
    int imagewidth=11;
    int block_bit =4;

    double **image=getImage(imagelength,imagewidth);//输入图像的尺寸信息，以及分块大小，图像矩阵

    double ** dct;
    dct=DO_DCT(imagelength,imagewidth,block_bit,image);//输出DCT

    double ** idct;
    idct=DO_IDCT(imagelength,imagelength,block_bit,dct);//输出IDCT

    double **dct2;
    dct2=DODCT2(dct,imagelength,imagewidth,block_bit);//输出去除50%高频系数的IDCT

    //打印结果
    for (int i=0;i<imagelength;++i){//test DCT

        for(int j=0;j<imagewidth;++j)
        {
            printf("%f_",dct[i][j]);
        }
        printf("\n",i);

    }
    printf("dct finished\n\n");

    for (int i=0;i<imagelength;++i){//test IDCT

        for(int j=0;j<imagewidth;++j)
        {
            printf("%f_",idct[i][j]);
        }
        printf("\n",i);

    }
    printf("IDCT finished\n\n");

    for (int i=0;i<imagelength;++i){//test DCT2

        for(int j=0;j<imagewidth;++j)
        {
            printf("%f_",dct2[i][j]);
        }
        printf("\n",i);

    }
    printf("dct2 finished");
}
