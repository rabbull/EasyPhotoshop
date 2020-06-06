//
// Created by 唐继 on 2020/6/5.
//

#include <imgproc/lossless-predictive-coding.h>
#include <core/image.h>
#include <stdint.h>

//rank for predictor's rank
//coefficient for a  coefficient list of predictor
CoreImage *imgproc_lossless_predictive_coding(CoreImage *image, guint32 rank, guint32 *coefficient){
    //get image info
    guint32 row = core_size_get_height(core_image_get_size(image));
    guint32 col = core_size_get_width(core_image_get_size(image));
    guint8 **data = (guint8**)core_image_get_data(image);

    //initial
    CoreSize *new_size = core_size_new_with_value(col, row);
    CoreColorSpace new_color_space = CORE_COLOR_SPACE_GRAY_SCALE;
    CorePixelType new_pixel_type = core_image_get_pixel_type(image);
    guint8 **result_data = (guint8 **)g_malloc(row);
    for(guint32 i = 0; i < row; i++){
        result_data[i] = (guint8 *)g_malloc(col);
    }

    //预测编码
    gdouble data_pre = 0.0;
    guint32 x = 0, y = 0;
    for(guint32 i = 0; i < row; i++){
        for(guint32 j = 0; j < col; j++){
            if(j == 0){
                result_data[i][j] = data[i][j];
            }else if(j < rank){
                result_data[i][j] = data[i][j] - data[i][0];
            }else{
                for(x = rank, y = 0; x >= 1 && y < rank; x--, y++){
                    data_pre += data[i][j - x] * coefficient[y];
                }
                result_data = data[i][j] - (guint8)data_pre;
            }
        }
    }

    //处理负值，映射到0-255
    for(guint32 i = 0; i < row; i++) {
        for (guint32 j = 0; j < col; j++) {
            result_data[i][j] = (result_data[i][j] + 255) / 2;
        }
    }

    //将预测值生成为图片
    CoreImage *new_core_image = core_image_new_with_data((gpointer)result_data, new_color_space, new_pixel_type,new_size,FALSE);

    return new_core_image;
}