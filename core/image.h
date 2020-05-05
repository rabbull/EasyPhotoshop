//
// Created by karl on 4/28/20.
//

#ifndef EASYPHOTOSHOP_IMAGE_H
#define EASYPHOTOSHOP_IMAGE_H

#include <glib-object.h>
#include <core/color-space.h>
#include <core/pixel.h>
#include <core/size.h>

G_BEGIN_DECLS

#define CORE_TYPE_IMAGE core_image_get_type()

G_DECLARE_DERIVABLE_TYPE(CoreImage, core_image, CORE, IMAGE, GObject)

struct _CoreImageClass {
    GObjectClass parent_class;

    /* Padding to allow new virtual functions without breaking ABI. */
    gpointer padding[12];
};

/* CONSTRUCTORS */
CoreImage *core_image_new(void);

CoreImage *core_image_new_empty_with_size(CoreSize *size, CoreColorSpace color_space, CorePixelType pixel_type);

CoreImage *core_image_new_with_data(gpointer data, CoreColorSpace color_space, CorePixelType pixel_type, CoreSize *size,
                                    gboolean copy_data);

CoreImage *core_image_new_fill_with_color(CoreSize *size, CoreColorSpace color_space, CorePixelType pixel_type,
                                          gpointer pixel_data);

CoreImage *core_image_new_clone(CoreImage *old);


/* PUBLIC METHODS */
gboolean core_image_empty(CoreImage *self);

/* CLONE */
gboolean core_image_copy(CoreImage *self, CoreImage *another);

/* GETTERS */
CoreSize *core_image_get_size(CoreImage *self);

guint8 core_image_get_channel(CoreImage *self);

guint8 core_image_get_byte_per_pixel(CoreImage *self);

gpointer core_image_get_data(CoreImage *self);

gpointer core_image_get_pixel(CoreImage *self, guint32 x, guint32 y);

CoreColorSpace core_image_get_color_space(CoreImage* self);

CorePixelType core_image_get_pixel_type(CoreImage* self);

/* OTHERS */
gboolean core_image_reshape(CoreImage *self, CoreSize *size, GError **error);

gboolean core_image_assign_data(CoreImage *self, gpointer data, CoreColorSpace color_space, CorePixelType pixel_type,
                                CoreSize *size, gboolean copy_data);

G_END_DECLS

#endif //EASYPHOTOSHOP_IMAGE_H
