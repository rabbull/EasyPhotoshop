#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define PI  3.14159262


double** getImage(int length,int width){

    double **image;
    image=(double **)malloc(sizeof(double *)*length);
    for(int i=0;i<width;i++){
        image[i]=(double*)malloc(sizeof(double)*width);

    }

    /*double copy[4][6]={{255,3,0,110,2},
                       {1,2,4,5,40},
                       {4,1,3,4,5},
                       {8,3,49,123,99}};*/
    /*
    double copy[9][9] = {  {52,55,61,66,70,61,64,73,1},
                        {63,59,55,90,109,85,69,72,2},
                        {62,59,68,113,144,104,66,73,3},
                        {63,58,71,122,154,106,70,69,4},
                        {67,61,68,104,126,88,68,70,5},
                        {79,65,60,70,77,68,58,75,6},
                        {85,71,64,59,55,61,65,83,7},
                        {87,79,69,68,65,76,78,94,10},
                        {1,2,3,45,6,7,8,98,7}};*/

    /*double copy[8][8] = {  {52,55,61,66,70,61,64,73},
                        {63,59,55,90,109,85,69,72},
                        {62,59,68,113,144,104,66,73},
                        {63,58,71,122,154,106,70,69},
                        {67,61,68,104,126,88,68,70},
                        {79,65,60,70,77,68,58,75},
                        {85,71,64,59,55,61,65,83},
                        {87,79,69,68,65,76,78,94} };*/
    /*double copy[4][8] = {  {52,55,61,66,70,61,64,73},
                           {63,59,55,90,109,85,69,72},
                           {62,59,68,113,144,104,66,73},
                           {63,58,71,122,154,106,70,69},};*/
    double copy[11][11] = {{52,55,61,66,70,61,64,73,77,89,3},
        {63,59,55,90,109,85,69,72,13,10,46},
        {62,59,68,113,144,104,66,73,20,40,56},
        {63,58,71,122,154,106,70,69,10,30,78},
        {67,61,68,104,126,88,68,70,25,37,45},
        {79,65,60,118,155,255,0,75,25,46,14},
        {85,71,64,59,55,61,65,83,16,63,13},
        {87,79,69,68,64,73,77,94,28,199,46},
        {47,59,64,67,66,74,78,95,27,18,48},
        {67,109,69,68,65,76,78,94,26,209,11},
        {67,109,69,68,65,76,77,74,66,9,10}};


    for(int i=0;i<length;i++){
        for(int j=0;j<width;j++){
            image[i][j]=copy[i][j];

        }
    }

    return image;
}//获取测试的图像矩阵，最终不需要


double ** DO_DCT(int length, int width, int block_bit,double** image) {

    //double **image=getImage(length,width);//获取图像信息

    int blockSize=block_bit;
    int blockNumberSize1=length/block_bit;
    int blockNumberSize2=width/block_bit;
    double ** DCTimage=(double**)malloc(sizeof(double *)*length);//DCT
    for(int i=0;i<length;i++){
        DCTimage[i]=(double *)malloc(sizeof(double *)*width);
    }
    for (int  i= 0; i <length ; ++i) {
        for (int j = 0; j < width; ++j) {
            DCTimage[i][j]=image[i][j];
        }
    }

    double ** m=(double**)malloc(sizeof(double *)*block_bit);
    for(int i=0;i<block_bit;i++){
        m[i]=(double *)malloc(sizeof(double *)*block_bit);
    }

    int** a=(int**)malloc(sizeof(int*)*block_bit);
    for(int i=0;i<block_bit;i++){
        a[i]=(int*)malloc(sizeof(int*)*block_bit);
    }


    for(int i=0;i<blockNumberSize1;++i)
    {                                //图像分块
        for(int j=0;j<blockNumberSize2;++j)
        {
            for(int k=0;k<blockSize;k++)
            {
                for(int l=0;l<blockSize;l++)
                {
                    a[k][l]=image[i*blockSize+k][j*blockSize+l];
                }
            }
            //DCT
            double alpha,beta;
            for(int p=0; p < blockSize; p++)
            {
                for(int q=0; q < blockSize; q++)
                {
                    if(p==0) {alpha=sqrt(1.0 / blockSize);}
                    else {alpha=sqrt(2.0 / blockSize);}
                    if(q==0){ beta=sqrt(1.0 / blockSize);}
                    else {beta=sqrt(2.0 / blockSize);}
                    double ans=0;
                    for(int i=0; i < blockSize; i++)
                        for(int j=0; j < blockSize; j++)
                            ans+= a[i][j] * cos((2 * i + 1) * PI * p / (2 * blockSize)) * cos((2 * j + 1) * PI * q / (2 * blockSize));
                    m[p][q]= alpha * beta * ans;
                }
            }


            for(int k=0;k<blockSize;k++){
                for(int l=0;l<blockSize;l++){
                    DCTimage[i*blockSize+k][j*blockSize+l]=m[k][l];
                    //SCHimage[i*blockSize+k][j*blockSize+k]=n[k][l];
                }
            }
        }
    }
    //printf("success in DCT\n");
    return DCTimage;
}//输入图像的长、宽、要求分块的大小、原始图像矩阵，返回DCT矩阵

double ** DO_IDCT(int length,int width,int block_bit,double ** dct){

    //int **image=getImage(length,width);

    int blockSize=block_bit;
    int blockNumberSize1=length/block_bit;
    int blockNumberSize2=width/block_bit;

    double ** IDCTimage=(double**)malloc(sizeof(double *)*length);//IDCT
    for(int i=0;i<length;i++){
        IDCTimage[i]=(double *)malloc(sizeof(double *)*width);
    }

    double ** m=(double **)malloc(sizeof(double *)*block_bit);
    for(int i=0;i<block_bit;i++){
        m[i]=(double *)malloc(sizeof(double *)*block_bit);
    }

    double ** a=(double **)malloc(sizeof(double *)*block_bit);
    for(int i=0;i<block_bit;i++){
        a[i]=(double *)malloc(sizeof(double *)*block_bit);
    }

    for (int i = 0; i <length ; ++i) {
        for (int j = 0; j <width ; ++j) {
                IDCTimage[i][j]=dct[i][j];
        }

    }
    //dct=DO_DCT(length,width,block_bit);



    for(int i=0;i<blockNumberSize1;++i)
    {       //图像分块
        for(int j=0;j<blockNumberSize2;++j)
        {
            for(int k=0;k<blockSize;k++)
            {
                for(int l=0;l<blockSize;l++)
                {
                    m[k][l]=dct[i*blockSize+k][j*blockSize+l];

                }
            }
            //IDCT
            double alpha,beta;
            for(int i=0; i < blockSize; i++)
            {
                for(int j=0; j < blockSize; j++)
                {
                    double tmp=0.0;
                    for(int p=0; p < blockSize; p++)
                        for(int q=0; q < blockSize; q++)
                        {
                            if(p==0) alpha=sqrt(1.0 / blockSize);
                            else alpha=sqrt(2.0 / blockSize);
                            if(q==0) beta=sqrt(1.0 / blockSize);
                            else beta=sqrt(2.0 / blockSize);
                            tmp+= alpha * beta * m[p][q] * cos((2 * i + 1) * PI * p / (2 * blockSize)) * cos((2 * j + 1) * PI * q / (2 * blockSize));
                        }
                    a[i][j]=tmp;
                }
            }


            for(int k=0;k<blockSize;k++)
            {
                for(int l=0;l<blockSize;l++)
                {
                    // _SCHimage[i*8+k][j*8+l]=m[k][l];
                    IDCTimage[i*blockSize+k][j*blockSize+l]=a[k][l];
                }
            }
        }
    }
    //printf("success in IDCT!\n");
    return IDCTimage;
}//输入图像的长、宽、要求分块的大小、DCT处理后图像矩阵，返回IDCT矩阵

double** DODCT2(double **image,int length,int width,int block_bit){


    double ** DCT2image=(double**)malloc(sizeof(double *)*length);//DCT2分配空间
    for(int i=0;i<length;i++){
        DCT2image[i]=(double *)malloc(sizeof(double)*width);
    }
    for (int i = 0; i <length ; ++i) {
        for (int j = 0; j <width ; ++j) {
            DCT2image[i][j]=image[i][j];
        }
    }
    int blockSize=block_bit;
    int blockNumberSize1=length/block_bit;
    int blockNumberSize2=width/block_bit;
    for(int i=0;i<blockNumberSize1;++i)
    {                                //图像分块
        for(int j=0;j<blockNumberSize2;++j)
        {
            for (int k = 0; k <blockSize ; ++k) {
                for (int l = 0; l < blockSize; ++l) {

                    /*if((k==0)&(l==0)){
                        //DCT2image[i*blockSize+k][j*blockSize+l]=0;
                    }*/
                    if((k+l+1)>blockSize){
                        DCT2image[i*blockSize+k][j*blockSize+l]=0;
                    }
                    if(((k+l+1)==blockSize)&&((k+1)>(blockSize/4))&&((k+1)<=(blockSize/4*3))){
                        DCT2image[i*blockSize+k][j*blockSize+l]=0;
                    }

                }
            }

        }
    }

/*
    for (int i = 0; i < length; ++i) {
                                        //half dct test
        for (int j = 0; j <width ; ++j) {
            printf("%f_",DCT2image[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    double **IDCT2image;
    IDCT2image=DO_IDCT(length,width,block_bit,DCT2image);

    return IDCT2image;

}//输入DCT处理后图像、图像的长、宽、要求分块的大小，返回IDCT2矩阵



