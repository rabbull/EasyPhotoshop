//
// Created by karl on 7/6/2020.
//

#include "discrete-cosine-transform.h"
#include <math.h>

static double **do_dct(int length, int width, int block_bit, double **image);

static double **do_idct(int length, int width, int block_bit, double **dct);

static double **do_dct_drop_half(double **dct, int length, int width, int block_bit);

CoreImage *imgproc_discrete_cosine_transform(CoreImage *image, gint block_bit) {
    CoreSize *size = NULL;
    gpointer img_data;
    int i, j;
    int length, width;
    double **img_data_cast = NULL;
    double **dct;
    CoreImage *dct_image;
    gdouble *dct_data;
    CorePixelType pixel_type;
    g_return_val_if_fail(CORE_COLOR_SPACE_GRAY_SCALE == core_image_get_color_space(image), NULL);

    pixel_type = core_image_get_pixel_type(image);
    size = core_image_get_size(image);
    length = core_size_get_height(size);
    width = core_size_get_width(size);
    img_data = core_image_get_data(image);

    img_data_cast = malloc(sizeof(double *) * length);
    for (i = 0; i < length; ++i) {
        img_data_cast[i] = malloc(sizeof(double) * width);
    }

    if (pixel_type == CORE_PIXEL_D1) {
        for (i = 0; i < length; ++i) {
            for (j = 0; j < width; ++j) {
                img_data_cast[i][j] = ((gdouble *) img_data)[i * width + j] * 255.0;
            }
        }
    } else if (pixel_type == CORE_PIXEL_U1) {
        for (i = 0; i < length; ++i) {
            for (j = 0; j < width; ++j) {
                img_data_cast[i][j] = ((guint8 *) img_data)[i * width + j];
            }
        }
    } else {
        goto fail;
    }

    dct = do_dct(length, width, block_bit, img_data_cast);
    dct_data = g_malloc(sizeof(gdouble) * core_size_get_area(size));
    for (i = 0; i < length; ++i) {
        memcpy(dct_data + i * width, dct[i], width * sizeof(gdouble));
        free(dct[i]);
    }
    free(dct);
    dct_image = core_image_new_with_data(dct_data, CORE_COLOR_SPACE_MATRIX, CORE_PIXEL_D1, size, FALSE);
    g_object_unref(size);
    return dct_image;

    fail:
    if (size) g_object_unref(size);
    if (img_data_cast) {
        for (i = 0; i < length; ++i) {
            free(img_data_cast[i]);
        }
        free(img_data_cast);
    }
    return NULL;
}

CoreImage *imgproc_discrete_cosine_transform_drop_half(CoreImage *dct_image, gint block_bit) {
    CoreImage *dropped;
    CoreSize *size;
    int length, width;
    double **dct_data, **dropped_data;
    gdouble *dropped_data_cast;
    int i;

    g_return_val_if_fail(CORE_COLOR_SPACE_MATRIX == core_image_get_color_space(dct_image), NULL);
    g_return_val_if_fail(CORE_PIXEL_D1 == core_image_get_pixel_type(dct_image), NULL);

    size = core_image_get_size(dct_image);
    length = core_size_get_height(size);
    width = core_size_get_height(size);
    dct_data = malloc(sizeof(double *) * length);
    for (i = 0; i < length; ++i) {
        dct_data[i] = malloc(sizeof(double) * width);
        memcpy(dct_data[i], (gdouble *) core_image_get_data(dct_image) + i * width, width * sizeof(gdouble));
    }
    dropped_data = do_dct_drop_half(dct_data, length, width, block_bit);
    dropped_data_cast = g_malloc(sizeof(gdouble) * core_size_get_area(size));
    for (i = 0; i < length; ++i) {
        memcpy(dropped_data_cast + i * width, dropped_data[i], sizeof(gdouble) * width);
        free(dropped_data[i]);
    }
    free(dropped_data);
    dropped = core_image_new_with_data(dropped_data_cast, CORE_COLOR_SPACE_MATRIX, CORE_PIXEL_D1, size, FALSE);
    g_object_unref(size);
    return dropped;
}

CoreImage *imgproc_inverse_discrete_cosine_transform(CoreImage *dct_image, gint block_bit) {
    CoreImage *recovered;
    double **recovered_data;
    gdouble *recovered_data_cast;
    double **dct_data;
    CoreSize *size;
    int length, width;
    int i;

    g_return_val_if_fail(CORE_COLOR_SPACE_MATRIX == core_image_get_color_space(dct_image), NULL);
    g_return_val_if_fail(CORE_PIXEL_D1 == core_image_get_pixel_type(dct_image), NULL);

    size = core_image_get_size(dct_image);
    length = core_size_get_height(size);
    width = core_size_get_width(size);

    dct_data = malloc(sizeof(double *) * length);
    for (i = 0; i < length; ++i) {
        dct_data[i] = malloc(sizeof(double) * width);
        memcpy(dct_data[i], (gdouble *) core_image_get_data(dct_image) + i * width, sizeof(gdouble) * width);
    }
    recovered_data = do_idct(length, width, block_bit, dct_data);
    recovered_data_cast = g_malloc(sizeof(double) * core_size_get_area(size));
    for (i = 0; i < length; ++i) {
        memcpy(recovered_data_cast + i * width, recovered_data[i], sizeof(double) * width);
        free(recovered_data[i]);
    }
    free(recovered_data);
    recovered = core_image_new_with_data(recovered_data_cast, CORE_COLOR_SPACE_GRAY_SCALE, CORE_PIXEL_D1, size, FALSE);
    g_object_unref(size);
    return recovered;
}

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

static double **do_dct_drop_half(double **dct, int length, int width, int block_bit) {
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
    return DCT2image;
}//输入DCT处理后图像、图像的长、宽、要求分块的大小，返回IDCT2矩阵
