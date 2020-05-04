//
// Created by karl_ on 2020-5-3.
//

#ifndef EASYPHOTOSHOP_COLOR_CONVERT_H
#define EASYPHOTOSHOP_COLOR_CONVERT_H

#include <glib-object.h>
#include <core/image.h>

gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst);

gboolean imgproc_to_binary_threshold(CoreImage *src, CoreImage **dst, gdouble threshold, gboolean inverse);

gboolean imgproc_to_binary_dither();

gboolean imgproc_to_binary_ordered_dither();

#endif //EASYPHOTOSHOP_COLOR_CONVERT_H
