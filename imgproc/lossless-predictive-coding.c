//
// Created by 唐继 on 2020/6/5.
//

#include "math.h"
#include <core/image.h>
#include "lossless-predictive-coding.h"

static guint8 **new_matrix(guint32 x, guint32 y);

//rank for predictor's rank
//coefficient for a coefficient list of predictor
CoreImage *imgproc_predictive_coding(CoreImage *image, guint32 rank, gdouble const *const coefficient) {
    //get image info
    CoreSize *size = core_image_get_size(image);
    guint32 row = core_size_get_height(size);
    guint32 col = core_size_get_width(size);
    g_object_unref(size);
    guint8 *image_data = (guint8 *) core_image_get_data(image);
    guint8 **data = new_matrix(row, col);
    for (guint32 i = 0, z = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            data[i][j] = image_data[z++];
        }
    }
    //initial
    CoreSize *new_size = core_size_new_with_value(row, col);
    CoreColorSpace new_color_space = CORE_COLOR_SPACE_GRAY_SCALE;
    CorePixelType new_pixel_type = CORE_PIXEL_U1;
    gdouble **result_data = (gdouble **) g_malloc(sizeof(gdouble *) * row);
    for (guint32 i = 0; i < row; i++) {
        result_data[i] = (gdouble *) g_malloc(sizeof(gdouble) * col);
    }

    //预测编码
    gdouble data_pre = 0.0;
    guint32 x = 0, y = 0;
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            if (j == 0) {
                result_data[i][j] = (gdouble) data[i][j];
            } else if (j < rank) {
                result_data[i][j] = data[i][j] - (gdouble) data[i][0];
            } else {
                for (x = rank, y = 0; x >= 1 && y < rank; x--, y++) {
                    data_pre = data_pre + ((gdouble) data[i][j - x] * coefficient[y]);
                }
                result_data[i][j] = data[i][j] - (gdouble) data_pre;
                data_pre = 0.0;
            }
        }
    }

    //映射到0-255
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = (result_data[i][j] + 255) / 2;
            result_data[i][j] = (guint8) result_data[i][j];
        }
    }

    //反映射
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = result_data[i][j] * 2 - 255;
        }
    }

    //逆向
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            if (j == 0) {
                result_data[i][j] = result_data[i][j];
            } else if (j < rank) {
                result_data[i][j] = result_data[i][j] + result_data[i][0];
            } else {
                for (x = rank, y = 0; x >= 1 && y < rank; x--, y++) {
                    data_pre = data_pre + (gdouble) data[i][j - x] * coefficient[y];
                }
                result_data[i][j] = result_data[i][j] + data_pre;
                data_pre = 0.0;
            }
        }
    }

    guint8 *result_matrix = (guint8 *) g_malloc(row * col * sizeof(guint8));
    for (guint32 i = 0, z = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_matrix[z++] = (guint8) result_data[i][j];
        }
        g_free(result_data[i]);
    }
    g_free(result_data);

    //将预测值生成为图片
    CoreImage *new_core_image = core_image_new_with_data((gpointer) result_matrix, new_color_space, new_pixel_type,
                                                         new_size, FALSE);

    return new_core_image;
}

CoreImage *imgproc_predictive_decoding(CoreImage *image, guint32 rank, gdouble const *const coefficient) {
    //get image info
    guint32 row = core_size_get_height(core_image_get_size(image));
    guint32 col = core_size_get_width(core_image_get_size(image));
    guint8 *image_data = (guint8 *) core_image_get_data(image);
    guint8 **data = new_matrix(row, col);
    for (guint32 i = 0, z = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            data[i][j] = image_data[z++];
        }
    }

    //initial
    CoreSize *new_size = core_size_new_with_value(row, col);
    CoreColorSpace new_color_space = CORE_COLOR_SPACE_GRAY_SCALE;
    CorePixelType new_pixel_type = CORE_PIXEL_U1;
    gdouble **result_data = (gdouble **) g_malloc(sizeof(gdouble *) * row);
    for (guint32 i = 0; i < row; i++) {
        result_data[i] = (gdouble *) g_malloc(sizeof(gdouble) * col);
        for (gint32 j = 0; j < col; j++) {
            result_data[i][j] = (gdouble) data[i][j];
        }
    }

    //反映射
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = result_data[i][j] * 2 - 255;
        }
    }

    g_print("result_data after inverse mapping: ");
    for (int i = 0; i < 10; ++i) {
        g_print("%lf, ", result_data[i][i]);
    }
    g_print("\n");

    //逆向
    gdouble data_pre = 0.0;
    guint32 x = 0, y = 0;
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            if (j == 0) {
                result_data[i][j] = result_data[i][j];
            } else if (j < rank) {
                result_data[i][j] = result_data[i][j] + result_data[i][0];
            } else {
                for (x = rank, y = 0; x >= 1 && y < rank; x--, y++) {
                    data_pre = data_pre + (gdouble) data[i][j - x] * coefficient[y];
                }
                result_data[i][j] = result_data[i][j] + data_pre;
                data_pre = 0.0;
            }
        }
    }

    g_print("result_data after decoding: ");
    for (int i = 0; i < 10; ++i) {
        g_print("%lf, ", result_data[i][i]);
    }
    g_print("\n");

    guint8 *result_matrix = (guint8 *) g_malloc(row * col * sizeof(guint8));
    for (guint32 i = 0, z = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_matrix[z++] = (guint8) result_data[i][j];
        }
        g_free(result_data[i]);
    }
    g_free(result_data);

    CoreImage *new_core_image = core_image_new_with_data((gpointer) result_matrix, new_color_space, new_pixel_type,
                                                         new_size, FALSE);

    return new_core_image;
}

static guint8 **new_matrix(guint32 x, guint32 y) {
    guint8 **matrix;
    matrix = (guint8 **) g_malloc(x * sizeof(guint8 *));
    for (guint32 i = 0; i < x; i++) {
        matrix[i] = (guint8 *) g_malloc(sizeof(guint8) * y);
    }
    return matrix;
}
