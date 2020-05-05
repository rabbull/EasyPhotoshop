//
// Created by karl_ on 2020-5-5.
//

#include <core/pixel.h>


guint8 core_pixel_get_channel(CorePixelType const pixel_type) {
    return pixel_type & 7u;
}

gdouble core_pixel_get_range(CorePixelType const pixel_type) {
    if (core_pixel_is_uint8(pixel_type)) {
        return 255.0;
    } else if (core_pixel_is_double(pixel_type)) {
        return 1.0;
    } else {
        return 0.0;
    }
}

gsize core_pixel_get_size(CorePixelType pixel_type) {
    gsize channel_size = 0;
    if (core_pixel_is_uint8(pixel_type)) {
        channel_size = sizeof(guint8);
    } else if (core_pixel_is_double(pixel_type)) {
        channel_size = sizeof(gdouble);
    }
    return channel_size * core_pixel_get_channel(pixel_type);
}

gboolean core_pixel_is_uint8(CorePixelType pixel_type) {
    return pixel_type >> 3u == 0;
}

gboolean core_pixel_is_double(CorePixelType pixel_type) {
    return pixel_type >> 3u == 1;
}
