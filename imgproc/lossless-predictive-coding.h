//
// Created by 唐继 on 2020/6/5.
//

#ifndef EASYPHOTOSHOP_LOSSLESS_PREDICTIVE_CODING_H
#define EASYPHOTOSHOP_LOSSLESS_PREDICTIVE_CODING_H

#include <stdint.h>
#include <core/image.h>

CoreImage *imgproc_lossless_predictive_coding(CoreImage *image, guint32 rank, gdouble *coefficient);

#endif //EASYPHOTOSHOP_LOSSLESS_PREDICTIVE_CODING_H
