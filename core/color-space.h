//
// Created by karl_ on 2020-5-5.
//

#ifndef EASYPHOTOSHOP_COLOR_SPACE_H
#define EASYPHOTOSHOP_COLOR_SPACE_H

#include <glib-object.h>

typedef enum {
    CORE_COLOR_SPACE_BIN = 01,          /* binary image */
    CORE_COLOR_SPACE_GRAY_SCALE = 011,  /* gray scale image */
    CORE_COLOR_SPACE_RGB = 03,          /* rgb image */
    CORE_COLOR_SPACE_HSL = 013,         /* hsl image */
    CORE_COLOR_SPACE_RGBA = 04,         /* rgba image */
    CORE_COLOR_SPACE_PLAIN = 021,       /* image that stores plain data */
} CoreColorSpace;

guint8 core_color_space_get_channel(CoreColorSpace color_space);

#endif //EASYPHOTOSHOP_COLOR_SPACE_H
