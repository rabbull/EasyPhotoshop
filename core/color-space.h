//
// Created by karl_ on 2020-5-5.
//

#ifndef EASYPHOTOSHOP_COLOR_SPACE_H
#define EASYPHOTOSHOP_COLOR_SPACE_H

#include <glib-object.h>

typedef enum {
    CORE_COLOR_SPACE_BIN = 1,
    CORE_COLOR_SPACE_GRAY_SCALE = 9,
    CORE_COLOR_SPACE_RGB = 3,
    CORE_COLOR_SPACE_HSL = 11,
    CORE_COLOR_SPACE_RGBA = 4
} CoreColorSpace;

guint8 core_color_space_get_channel(CoreColorSpace color_space);

#endif //EASYPHOTOSHOP_COLOR_SPACE_H
