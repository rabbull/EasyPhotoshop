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

    double copy[4][4]={{5,3,0,2},{1,7,8,3},{4,2,2,2},{8,5,2,1}};

    for(int i=0;i<length;i++){
        for(int j=0;j<width;j++){
            image[i][j]=copy[i][j];

        }
    }

    return image;
}


double ** DO_DCT(int length, int width, int block_bit,double** image) {

    //double **image=getImage(length,width);//获取图像信息

    int blockSize=block_bit;
    int blockNumberSize=length/block_bit;

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


    for(int i=0;i<blockNumberSize;++i)
    {                                //图像分块
        for(int j=0;j<blockNumberSize;++j)
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
    printf("success in DCT\n");
    return DCTimage;
}

double ** DO_IDCT(int length,int width,int block_bit,double ** dct){

    //int **image=getImage(length,width);

    int blockSize=block_bit;
    int blockNumberSize=length/block_bit;

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


    //dct=DO_DCT(length,width,block_bit);



    for(int i=0;i<blockNumberSize;++i)
    {       //图像分块
        for(int j=0;j<blockNumberSize;++j)
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
                    double ans=0;
                    for(int p=0; p < blockSize; p++)
                        for(int q=0; q < blockSize; q++)
                        {
                            if(p==0) {
                                alpha = sqrt(1.0 / blockSize);
                            }
                            else {
                                alpha=sqrt(2.0 / blockSize);
                            }
                            if(q==0) {
                                beta=sqrt(1.0 / blockSize);
                            }
                            else {
                                beta=sqrt(2.0 / blockSize);
                            }

                            ans+= alpha * beta * m[p][q] * cos((2 * i + 1) * PI * p / (2 * blockSize)) * cos((2 * j + 1) * PI * q / (2 * blockSize));
                        }
                    a[i][j]=ans;
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
    printf("success in IDCT!\n");
    return IDCTimage;
}
double** DODCT2(double **image,int length,int width,int block_bit){
    printf("\nthis is half dct\n");

    double ** DCT2image=(double**)malloc(sizeof(double *)*length);//DCT2分配空间
    for(int i=0;i<length;i++){
        DCT2image[i]=(double *)malloc(sizeof(double)*width);
    }
    for (int i = 0; i <length ; ++i) {
        for (int j = 0; j <width ; ++j) {
            DCT2image[i][j]=image[i][j];
        }
    }
    for (int i = 0; i <length ; ++i) {
        for (int j = 0; j < width; ++j) {
            if((i+j+1)>length){
                DCT2image[i][j]=0;
            }
            if(((i+j+1)==length)&&((i+1)>(length/2))){
                DCT2image[i][j]=0;
            }

        }}

    /*test
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j <width ; ++j) {
            printf("%f_",DCT2image[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

    double **IDCT2image;
    IDCT2image=DO_IDCT(length,width,block_bit,DCT2image);

    return IDCT2image;

}



