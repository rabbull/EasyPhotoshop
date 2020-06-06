//
// Created by 唐继 on 2020/6/4.
//
// version 1.0 实现了dither
// version 1.1 修复了dither中result_matrix赋值时下标计算错误的bug
// version 2.0 实现了ordered-dither
// version 2.1 修改了dither函数，合并了两种dither,减少了代码量
//

#include <stdint.h>
#include <core/image.h>
#include <imgproc/color-convert.h>

guint8 **new_matrix(guint32 x, guint32 y);
guint8 **new_dither_matrix(guint32 rank);
guint8 **grow_dither_matrix(guint8 **dither_matrix, guint32 rank);
void free_matrix(guint8 **matrix, guint32 x, guint32 y);
/*一些写的过程中以为会用上实际没用上函数
guint8 **matrix_add(guint8 **matrix1, guint8 **matrix2, guint32 rank);
guint8 **matrix_times_num(guint8 **matrix, gint8 num, guint32 rank);
guint8 **get_unit_matrix(guint32 rank);
*/

//rank should be 2^n, n>=1
//flag 0 for dither and other for ordered-dither
CoreImage *imgproc_to_binary_dither(CoreImage *coreImage, guint32 rank, guint32 flag){
    //initial
    CoreSize *new_size = NULL;
    CoreColorSpace new_color_space = CORE_COLOR_SPACE_BIN;
    CorePixelType new_pixel_type = core_image_get_pixel_type(coreImage);
    guint8 **result_matrix;
    
    //get image info
    guint32 row = core_size_get_height(core_image_get_size(coreImage));
    guint32 col = core_size_get_width(core_image_get_size(coreImage));
    guint8 **data = (guint8**)core_image_get_data(coreImage);

    //step0:get dither-matrix
    guint8 **dither_matrix = new_dither_matrix(rank);

    //step1:data map to 0-rank*rank(maybe rank*rank+1)
    guint32 T = 256/(rank * rank + 1);
    guint8 **quantization_matrix=new_matrix(row, col);
    for(guint32 i = 0; i < row; i++){
        for(guint32 j = 0; j < col; j++){
            quantization_matrix[i][j] = (gint8)(data[i][j]/T);
        }
    }

    //step2:dither
    /*dither*/
    if(flag == 0){
        result_matrix=new_matrix(row * rank, col * rank);
        new_size = core_size_new_with_value(col*rank, row*rank);
        for(guint32 i = 0; i < row; i++){
            for(guint32 j = 0; j < col; j++){
                for(guint32 x = 0; x < rank; x++){
                    for(guint32 y = 0; y < rank; y++){
                        if(quantization_matrix[i][j] > dither_matrix[x][y]){
                            result_matrix[i*rank+x][j*rank+y] = 255;
                        }else{
                            result_matrix[i*rank+x][j*rank+y] = 0;
                        }
                    }
                }
            }
        }
    }

    /*ordered dither*/
    else{
        result_matrix=new_matrix(row , col);
        new_size = core_size_new_with_value(col, row);
        for(guint32 i = 0; i < row; i++){
            for(guint32 j = 0; j < col; j++){
                if(quantization_matrix[i][j] > dither_matrix[i%rank][j%rank]){
                    result_matrix[i][j] = 255;
                }else{
                    result_matrix[i][j] = 0;
                }
            }
        }
    }

    //new CoreImage
    CoreImage *new_core_image = core_image_new_with_data((gpointer)result_matrix, new_color_space, new_pixel_type,new_size,FALSE);

    //free
    free_matrix(quantization_matrix,row,col);

    return new_core_image;
}

guint8 **new_matrix(guint32 x, guint32 y){
    guint8 **matrix;
    matrix = (guint8**)g_malloc(x);
    for(guint32 i = 0; i < x; i++){
        matrix[i] = (guint8*)g_malloc(y);
    }
    return matrix;
}

guint8 **new_dither_matrix(guint32 rank){
    if(rank == 2){
        guint8 **dither_matrix = new_matrix(2, 2);
        dither_matrix[0][0] = 0;
        dither_matrix[0][1] = 2;
        dither_matrix[1][0] = 3;
        dither_matrix[1][1] = 1;
        return dither_matrix;
    }
    return grow_dither_matrix(new_dither_matrix(rank/2), rank);
}

guint8 **grow_dither_matrix(guint8 **dither_matrix, guint32 rank){
    guint8 **new_dither_matrix = new_matrix(rank, rank);

    for(guint32 i = 0; i < rank/2; i++){
        for(guint32 j = 0; j < rank/2; j++){
            new_dither_matrix[i][j] = dither_matrix[i][j] * 4;
        }
    }
    for(guint32 i = 0; i < rank/2; i++){
        for(guint32 j = rank/2; j < rank; j++){
            new_dither_matrix[i][j] = dither_matrix[i][j-rank/2] * 4 + 2;
        }
    }
    for(guint32 i = rank/2; i < rank; i++){
        for(guint32 j = 0; j < rank/2; j++){
            new_dither_matrix[i][j] = dither_matrix[i - rank/2][j] * 4 + 3;
        }
    }
    for(guint32 i = rank/2; i < rank; i++){
        for(guint32 j = rank/2; j < rank; j++){
            new_dither_matrix[i][j] = dither_matrix[i - rank/2][j - rank/2 ] * 4 + 1;
        }
    }
    free_matrix(dither_matrix, rank/2, rank/2);
    return new_dither_matrix;
}

void free_matrix(guint8 **matrix, guint32 x, guint32 y){
    for(guint32 i = 0; i < x; i++){
        g_free(matrix[i]);
    }
    g_free(matrix);
}
/*guint8 **get_unit_matrix(guint32 rank){
    guint8 **unit_matrix = new_matrix(rank, rank);
    for(int i = 0; i < rank; i++){
        for(int j = 0; j < rank; j++){
            unit_matrix[i][j] = 0;
        }
    }
    return unit_matrix;
}

guint8 **matrix_add(guint8 **matrix1, guint8 **matrix2, guint32 rank){
    guint8 **add_matrix = new_matrix(rank, rank);
    for(guint32 i = 0; i < rank; i++){
        for(guint32 j = 0; j < rank; j=j++){
            add_matrix[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return add_matrix;
}

guint8 **matrix_times_num(guint8 **matrix, gint8 num, guint32 rank){
    guint8 **times_matrix = new_matrix(rank, rank);
    for(guint32 i = 0; i < rank; i++){
        for(guint32 j = 0; j < rank; j++){
            times_matrix[i][j] = matrix[i][j] * num;
        }
    }
    return times_matrix;
}*/