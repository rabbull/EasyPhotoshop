//
// Created by karl_ on 2020-5-3.
//

#ifndef EASYPHOTOSHOP_COLOR_CONVERT_H
#define EASYPHOTOSHOP_COLOR_CONVERT_H

#include <glib-object.h>
#include <core/image.h>

gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst);

#endif //EASYPHOTOSHOP_COLOR_CONVERT_H
