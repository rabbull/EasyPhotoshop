//
// Created by karl on 7/6/2020.
//

#ifndef EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H
#define EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H

#include <core/image.h>

CoreImage *imgproc_discrete_cosine_transform(CoreImage *image, gint block_bit);

CoreImage *imgproc_discrete_cosine_transform_drop_half(CoreImage *dct_image, gint block_bit);

CoreImage *imgproc_inverse_discrete_cosine_transform(CoreImage *dct_image, gint block_bit);

#endif //EASYPHOTOSHOP_DISCRETE_COSINE_TRANSFORM_H
