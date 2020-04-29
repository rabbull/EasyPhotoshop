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

CoreImage *core_image_new(void) {
    return g_object_new(CORE_TYPE_IMAGE, NULL);
}

CoreImage *core_image_new_with_data(guint8 *data, gsize data_len, guint8 bpp, CoreSize *size, gboolean copy_data) {
    CoreImage *img;
    img = core_image_new();
    core_image_assign_data(img, data, data_len, bpp, size, copy_data);
    return img;
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
gboolean core_image_get_size(CoreImage *self, CoreSize **size) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(size != NULL, FALSE);
    g_return_val_if_fail((*size) == NULL, FALSE);

    *size = private->size;
    return TRUE;
}

gboolean core_image_reshape(CoreImage *self, CoreSize *size, GError **error) {
    CoreImagePrivate *private = NULL;
    GError *temp_error = NULL;
    guint32 w, h, new_w, new_h;
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(size != NULL, FALSE);

    private = core_image_get_instance_private(self);

    w = core_size_get_width(private->size);
    h = core_size_get_height(private->size);
    if (w == 0 && h == 0) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "empty image shall not be reshaped.");
        return FALSE;
    }

    new_w = core_size_get_width(size);
    new_h = core_size_get_height(size);
    if (new_w * new_h != w * h) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "reshape do not modify area.");
        return FALSE;
    }

    if (!core_size_copy(size, &private->size)) {
        g_propagate_error(error, temp_error);
        return FALSE;
    }

    return TRUE;
}

guint8 *core_image_get_data(CoreImage *self) {
    CoreImagePrivate *private = NULL;
    g_return_val_if_fail(self != NULL, NULL);
    private = core_image_get_instance_private(self);
    return private->data;
}

gboolean
core_image_assign_data(CoreImage *self, guint8 *data, gsize data_len, guint8 bpp, CoreSize *size, gboolean copy_data) {
    CoreImagePrivate *private = NULL;
    guint8 *_data = NULL;
    gsize i;

    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(data != NULL, FALSE);
    g_return_val_if_fail(bpp == 8 || bpp == 24 || bpp == 32, FALSE);

    private = core_image_get_instance_private(self);
    if (!copy_data) {
        _data = data;
    } else {
        _data = g_malloc(sizeof(guint8) * data_len);
        memcpy(_data, data, data_len);
    }

    private->data = _data;
    private->bpp = bpp;
    core_size_copy(size, &private->size);

    return TRUE;
}