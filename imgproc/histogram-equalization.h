//
// Created by karl_ on 2020-5-3.
//

#ifndef EASYPHOTOSHOP_HISTOGRAM_EQUALIZATION_H
#define EASYPHOTOSHOP_HISTOGRAM_EQUALIZATION_H

#include <glib-object.h>
#include <core/image.h>

gboolean imgproc_histogram_equalization(CoreImage *src, CoreImage *dst, GError *error);

#endif //EASYPHOTOSHOP_HISTOGRAM_EQUALIZATION_H
