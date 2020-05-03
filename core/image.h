//
// Created by karl on 4/28/20.
//

#ifndef EASYPHOTOSHOP_IMAGE_H
#define EASYPHOTOSHOP_IMAGE_H

#include <glib-object.h>
#include <core/size.h>

G_BEGIN_DECLS

#define CORE_TYPE_IMAGE core_image_get_type()

G_DECLARE_DERIVABLE_TYPE(CoreImage, core_image, CORE, IMAGE, GObject)

struct _CoreImageClass {
    GObjectClass parent_class;

    // Padding to allow new virtual functions without breaking ABI.
    gpointer padding[12];
};

// constructors
CoreImage *core_image_new(void);

CoreImage *core_image_new_empty_with_size(CoreSize* size, guint8 channel);

CoreImage *core_image_new_with_data(gdouble *data, gsize data_len, guint8 channel, CoreSize *size, gboolean copy_data);

CoreImage *core_image_new_fill_with_color(CoreSize *size, guint8 channel, gdouble const *pix);

// clone
gboolean core_image_copy(CoreImage *self, CoreImage **another);

// other public methods
CoreSize *core_image_get_size(CoreImage *self);

guint8 core_image_get_channel(CoreImage *self);

guint8 core_image_get_byte_per_pixel(CoreImage *self);

gboolean core_image_reshape(CoreImage *self, CoreSize *size, GError **error);

gdouble *core_image_get_data(CoreImage *self);

gdouble *core_image_get_pixel(CoreImage *self, guint32 x, guint32 y);

gboolean
core_image_assign_data(CoreImage *self, gdouble *data, gsize data_len, guint8 channel, CoreSize *size, gboolean copy_data);

G_END_DECLS

#endif //EASYPHOTOSHOP_IMAGE_H
