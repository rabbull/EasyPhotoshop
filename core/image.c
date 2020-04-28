//
// Created by karl on 4/28/20.
//

#include "image.h"

typedef struct {
    guint8 *data;
    guint8 bpp;     // bits per pixel
    guint8 channel;
    CoreSize *size;
} CoreImagePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(CoreImage, core_image, G_TYPE_OBJECT)

// constructor
static void core_image_class_init(CoreImageClass *klass) {
}

static void core_image_init(CoreImage *self) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    private->data = NULL;
    private->bpp = 24;
    private->channel = 3;
    private->size = g_object_new(CORE_TYPE_SIZE, NULL);
}

// destructor
static void core_image_dispose(GObject *obj) {
    CoreImagePrivate *private = core_image_get_instance_private(CORE_IMAGE(obj));
    g_clear_object(&private->size);
    G_OBJECT_CLASS(core_image_parent_class)->dispose(obj);
}

static void core_image_finalize(GObject *obj) {
    CoreImagePrivate *private = core_image_get_instance_private(CORE_IMAGE(obj));
    if (private->data) g_free(private->data);
    G_OBJECT_CLASS(core_image_parent_class)->finalize(obj);
}

// public methods
gboolean core_image_get_size(CoreImage *self, CoreSize **size, GError **error) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(size != NULL, FALSE);
    g_return_val_if_fail((*size) == NULL, FALSE);

    *size = private->size;
    return TRUE;
}

gboolean core_image_set_size(CoreImage *self, CoreSize *size, GError **error) {
    CoreImagePrivate *private = NULL;
    GError *temp_error;
    g_return_val_if_fail(self != NULL && size != NULL, FALSE);

    private = core_image_get_instance_private(self);
    if (!core_size_copy(size, &private->size, &temp_error)) {
        g_propagate_error(error, temp_error);
        return FALSE;
    }
    return TRUE;
}