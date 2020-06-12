//
// Created by Mr.W on 2020/6/6.
//

#ifndef EASYPHOTOSHOP_UNIFORM_QUANTIZATION_H
#define EASYPHOTOSHOP_UNIFORM_QUANTIZATION_H
#include <stdint.h>
#include <core/image.h>

CoreImage *imgproc_uniform_quantization(CoreImage* image, guint32 ratio);

#endif //EASYPHOTOSHOP_UNIFORM_QUANTIZATION_H
