//
// Created by karl_ on 2020-5-5.
//

#ifndef EASYPHOTOSHOP_PIXEL_H
#define EASYPHOTOSHOP_PIXEL_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum {
    CORE_PIXEL_U1 = 1,
    CORE_PIXEL_U3 = 3,
    CORE_PIXEL_U4 = 4,
    CORE_PIXEL_D1 = 9,
    CORE_PIXEL_D3 = 11,
    CORE_PIXEL_D4 = 12
} CorePixelType;

guint8 core_pixel_get_channel(CorePixelType pixel_type);

gdouble core_pixel_get_range(CorePixelType pixel_type);

gsize core_pixel_get_size(CorePixelType pixel_type);

gboolean core_pixel_is_uint8(CorePixelType pixel_type);

gboolean core_pixel_is_double(CorePixelType pixel_type);

G_END_DECLS

#endif //EASYPHOTOSHOP_PIXEL_H
