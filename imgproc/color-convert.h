//
// Created by karl_ on 2020-5-3.
//

#ifndef EASYPHOTOSHOP_COLOR_CONVERT_H
#define EASYPHOTOSHOP_COLOR_CONVERT_H

#include <glib-object.h>
#include <core/image.h>

gboolean imgproc_to_HSL(CoreImage *src, CoreImage **dst);

gboolean imgproc_to_RGB(CoreImage *src, CoreImage **dst);

gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst);

gboolean imgproc_to_binary_threshold(CoreImage *src, CoreImage **dst, gdouble threshold, gboolean inverse);

CoreImage *imgproc_to_binary_dither(CoreImage *coreImage, guint32 rank, guint32 flag);

#endif //EASYPHOTOSHOP_COLOR_CONVERT_H
