//
// Created by karl on 7/6/2020.
//

#ifndef EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H
#define EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H

#include <core/image.h>

CoreImage *imgproc_dct(CoreImage *image, gint block_bit);

CoreImage *imgproc_dct_drop(CoreImage *image, gdouble drop_rate);

CoreImage *imgproc_idct(CoreImage *dct_image, gint block_bit, gdouble drop_rate);

#endif //EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H
