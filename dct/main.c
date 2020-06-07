//
// Created by intoblue on 2020/6/7.
//

#include "DCT.h"

int main()
{


    int imagelength=4;
    int imagewidth=4;
    int block_bit =4;

    double **image=getImage(imagelength,imagewidth);//输入图像的尺寸信息，以及分块大小，图像矩阵

    double ** dct;
    dct=DO_DCT(imagelength,imagewidth,block_bit,image);

    double ** idct;
    idct=DO_IDCT(imagelength,imagelength,block_bit,dct);

    double **dct2;
    dct2=DODCT2(dct,imagelength,imagewidth,block_bit);

    //test
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
