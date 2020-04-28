//
// Created by karl on 4/28/20.
//

#ifndef EASYPHOTOSHOP_IMAGE_H
#define EASYPHOTOSHOP_IMAGE_H

#include <glib-object.h>
#include "size.h"

G_BEGIN_DECLS

#define CORE_TYPE_IMAGE core_image_get_type()

G_DECLARE_DERIVABLE_TYPE(CoreImage, core_image, CORE, IMAGE, GObject)

struct _CoreImageClass {
    GObjectClass parent_class;

    void (*constructor) (CoreImage* self);

    // Padding to allow new virtual functions without breaking ABI.
    gpointer padding[12];
};

CoreImage* core_image_new(void);

gboolean core_image_get_size(CoreImage *self, CoreSize **size, GError **error);

gboolean core_image_set_size(CoreImage *self, CoreSize *size, GError **error);

G_END_DECLS

#endif //EASYPHOTOSHOP_IMAGE_H
