//
// Created by karl on 7/6/2020.
//

#include "discrete-cosine-transform.h"
#include <math.h>

static double **do_dct(int length, int width, int block_bit, double **image);

static double **do_idct(int length, int width, int block_bit, double **dct);

static double **do_dct_2(double **dct, int length, int width, int block_bit);

CoreImage *imgproc_dct(CoreImage *image, gint block_bit) {
    CoreSize *size;
    gpointer img_data;
    int i, j;
    int length, width;
    double **img_data_cast = NULL;
    double **dct;
    CoreColorSpace color_space;
    CorePixelType pixel_type;

    color_space = core_image_get_color_space(image);
    g_return_val_if_fail(color_space == CORE_COLOR_SPACE_RGB, NULL);

    pixel_type = core_image_get_pixel_type(image);
    size = core_image_get_size(image);
    length = core_size_get_height(size);
    width = core_size_get_width(size);
    img_data = core_image_get_data(image);

    img_data_cast = malloc(sizeof(double *) * length);
    for (i = 0; i < length; ++i) {
        img_data_cast[i] = malloc(sizeof(double) * width);
    }

    if (pixel_type == CORE_PIXEL_D3) {
        for (i = 0; i < length; ++i) {
            for (j = 0; j < width; ++j) {
                img_data_cast[i][j] = ((gdouble *) img_data)[i * width + j] * 255.0;
            }
        }
    } else if (pixel_type == CORE_PIXEL_U3) {
        for (i = 0; i < length; ++i) {
            for (j = 0; j < width; ++j) {
                img_data_cast[i][j] = ((gdouble *) img_data)[i * width + j] * 255.0;
            }
        }
    } else {
        goto fail;
    }

    dct = do_dct(length, width, block_bit, img_data_cast);

    /* TODO: convert dct to CoreImage; free dct; */

    fail:
    if (img_data_cast) {
        for (i = 0; i < length; ++i) {
            free(img_data_cast[i]);
        }
        free(img_data_cast);
    }
    return NULL;
}

CoreImage *imgproc_dct_drop(CoreImage *image, gdouble drop_rate) {}

CoreImage *imgproc_idct(CoreImage *dct_image, gint block_bit, gdouble drop_rate) {}

static double **do_dct(int length, int width, int block_bit, double **image) {
    int blockSize = block_bit;
    int blockNumberSize1 = length / block_bit;
    int blockNumberSize2 = width / block_bit;
    double **DCTimage = (double **) malloc(sizeof(double *) * length);//DCT
    for (int i = 0; i < length; i++) {
        DCTimage[i] = (double *) malloc(sizeof(double *) * width);
    }
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            DCTimage[i][j] = image[i][j];
        }
    }

    double **m = (double **) malloc(sizeof(double *) * block_bit);
    for (int i = 0; i < block_bit; i++) {
        m[i] = (double *) malloc(sizeof(double *) * block_bit);
    }

    int **a = (int **) malloc(sizeof(int *) * block_bit);
    for (int i = 0; i < block_bit; i++) {
        a[i] = (int *) malloc(sizeof(int *) * block_bit);
    }

    for (int i = 0; i < blockNumberSize1; ++i) {                                //图像分块
        for (int j = 0; j < blockNumberSize2; ++j) {
            for (int k = 0; k < blockSize; k++) {
                for (int l = 0; l < blockSize; l++) {
                    a[k][l] = image[i * blockSize + k][j * blockSize + l];
                }
            }
            //DCT
            double alpha, beta;
            for (int p = 0; p < blockSize; p++) {
                for (int q = 0; q < blockSize; q++) {
                    if (p == 0) { alpha = sqrt(1.0 / blockSize); }
                    else { alpha = sqrt(2.0 / blockSize); }
                    if (q == 0) { beta = sqrt(1.0 / blockSize); }
                    else { beta = sqrt(2.0 / blockSize); }
                    double ans = 0;
                    for (int i = 0; i < blockSize; i++)
                        for (int j = 0; j < blockSize; j++)
                            ans += a[i][j] * cos((2 * i + 1) * M_PI * p / (2 * blockSize)) *
                                   cos((2 * j + 1) * M_PI * q / (2 * blockSize));
                    m[p][q] = alpha * beta * ans;
                }
            }


            for (int k = 0; k < blockSize; k++) {
                for (int l = 0; l < blockSize; l++) {
                    DCTimage[i * blockSize + k][j * blockSize + l] = m[k][l];
                }
            }
        }
    }
    return DCTimage;
}//输入图像的长、宽、要求分块的大小、原始图像矩阵，返回DCT矩阵

static double **do_idct(int length, int width, int block_bit, double **dct) {
    int blockSize = block_bit;
    int blockNumberSize1 = length / block_bit;
    int blockNumberSize2 = width / block_bit;

    double **IDCTimage = (double **) malloc(sizeof(double *) * length);//IDCT
    for (int i = 0; i < length; i++) {
        IDCTimage[i] = (double *) malloc(sizeof(double *) * width);
    }

    double **m = (double **) malloc(sizeof(double *) * block_bit);
    for (int i = 0; i < block_bit; i++) {
        m[i] = (double *) malloc(sizeof(double *) * block_bit);
    }

    double **a = (double **) malloc(sizeof(double *) * block_bit);
    for (int i = 0; i < block_bit; i++) {
        a[i] = (double *) malloc(sizeof(double *) * block_bit);
    }

    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            IDCTimage[i][j] = dct[i][j];
        }

    }

    for (int i = 0; i < blockNumberSize1; ++i) {       //图像分块
        for (int j = 0; j < blockNumberSize2; ++j) {
            for (int k = 0; k < blockSize; k++) {
                for (int l = 0; l < blockSize; l++) {
                    m[k][l] = dct[i * blockSize + k][j * blockSize + l];

                }
            }
            //IDCT
            double alpha, beta;
            for (int i = 0; i < blockSize; i++) {
                for (int j = 0; j < blockSize; j++) {
                    double tmp = 0.0;
                    for (int p = 0; p < blockSize; p++)
                        for (int q = 0; q < blockSize; q++) {
                            if (p == 0) alpha = sqrt(1.0 / blockSize);
                            else alpha = sqrt(2.0 / blockSize);
                            if (q == 0) beta = sqrt(1.0 / blockSize);
                            else beta = sqrt(2.0 / blockSize);
                            tmp += alpha * beta * m[p][q] * cos((2 * i + 1) * M_PI * p / (2 * blockSize)) *
                                   cos((2 * j + 1) * M_PI * q / (2 * blockSize));
                        }
                    a[i][j] = tmp;
                }
            }


            for (int k = 0; k < blockSize; k++) {
                for (int l = 0; l < blockSize; l++) {
                    IDCTimage[i * blockSize + k][j * blockSize + l] = a[k][l];
                }
            }
        }
    }
    return IDCTimage;
}//输入图像的长、宽、要求分块的大小、DCT处理后图像矩阵，返回IDCT矩阵


static double **do_dct_2(double **dct, int length, int width, int block_bit) {
    double **DCT2image = (double **) malloc(sizeof(double *) * length);//DCT2分配空间
    for (int i = 0; i < length; i++) {
        DCT2image[i] = (double *) malloc(sizeof(double) * width);
    }
    for (int i = 0; i < length; ++i) {
        for (int j = 0; j < width; ++j) {
            DCT2image[i][j] = dct[i][j];
        }
    }
    int blockSize = block_bit;
    int blockNumberSize1 = length / block_bit;
    int blockNumberSize2 = width / block_bit;
    for (int i = 0; i < blockNumberSize1; ++i) {                                //图像分块
        for (int j = 0; j < blockNumberSize2; ++j) {
            for (int k = 0; k < blockSize; ++k) {
                for (int l = 0; l < blockSize; ++l) {

                    /*if((k==0)&(l==0)){
                        //DCT2image[i*blockSize+k][j*blockSize+l]=0;
                    }*/
                    if ((k + l + 1) > blockSize) {
                        DCT2image[i * blockSize + k][j * blockSize + l] = 0;
                    }
                    if (((k + l + 1) == blockSize) && ((k + 1) > (blockSize / 4)) && ((k + 1) <= (blockSize / 4 * 3))) {
                        DCT2image[i * blockSize + k][j * blockSize + l] = 0;
                    }

                }
            }

        }
    }

    double **IDCT2image;
    IDCT2image = do_idct(length, width, block_bit, DCT2image);

    return IDCT2image;
}//输入DCT处理后图像、图像的长、宽、要求分块的大小，返回IDCT2矩阵
