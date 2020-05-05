//
// Created by karl_ on 2020-5-5.
//

#include <core/color-space.h>

guint8 core_color_space_get_channel(CoreColorSpace const color_space) {
    return color_space & 7u;
}
