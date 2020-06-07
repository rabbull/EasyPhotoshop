//
// Created by Mr.W on 2020/6/6.
//

#include "uniform-quantization.h"
#include <core/image.h>
#include <stdint.h>

guint8 **new_matrix(guint32 x, guint32 y);

//ratio为用户输入压缩比
CoreImage *uniform_quantization(CoreImage *image, guint32 ratio) {
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
    CoreSize *new_size = core_size_new_with_value(col, row);
    CoreColorSpace new_color_space = CORE_COLOR_SPACE_GRAY_SCALE;
    CorePixelType new_pixel_type = core_image_get_pixel_type(image);
    guint8 **result_data = (guint8 **) g_malloc(row);
    for (guint32 i = 0; i < row; i++) {
        result_data[i] = (guint8 *) g_malloc(col);
    }

    //均匀量化
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = data[i][j] / ratio;
        }
    }

    //反量化
    for (guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = result_data[i][j] * ratio;
        }
    }

    guint8 *result_matrix = (guint8 *) g_malloc(row * col);
    for (guint32 i = 0, z = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_matrix[z++] = result_data[i][j];
        }
        g_free(result_data[i]);
    }
    g_free(result_data);

    //获得图像
    CoreImage *new_core_image = core_image_new_with_data((gpointer) result_matrix, new_color_space, new_pixel_type,
                                                         new_size, FALSE);
    return new_core_image;
}

guint8 **new_matrix(guint32 x, guint32 y) {
    guint8 **matrix;
    matrix = (guint8 **) g_malloc(x);
    for (guint32 i = 0; i < x; i++) {
        matrix[i] = (guint8 *) g_malloc(y);
    }
    return matrix;
}
