//
// Created by karl on 4/28/20.
//

#include "image.h"

typedef struct {
    gdouble *data;
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
    private->channel = 3;
    private->size = g_object_new(CORE_TYPE_SIZE, NULL);
}

CoreImage *core_image_new(void) {
    return g_object_new(CORE_TYPE_IMAGE, NULL);
}

CoreImage *core_image_new_empty_with_size(CoreSize* size, guint8 channel) {
    CoreImage *img;
    CoreImagePrivate *private;
    size = g_object_ref(size);
    img = core_image_new();
    private = core_image_get_instance_private(img);
    core_size_copy(size, &private->size);
    private->channel = channel;
    private->data = g_malloc(core_size_get_area(size) * core_image_get_byte_per_pixel(img));
    g_object_unref(size);
    return img;
}

CoreImage *core_image_new_with_data(gdouble *data, gsize data_len, guint8 channel, CoreSize *size, gboolean copy_data) {
    CoreImage *img;
    img = core_image_new();
    core_image_assign_data(img, data, data_len, channel, size, copy_data);
    return img;
}

CoreImage *core_image_new_fill_with_color(CoreSize *size, guint8 channel, guint8 const *pix) {
    CoreImage *img;
    CoreImagePrivate *private;
    gsize byte_per_pix, n_pix;
    gsize i, j;
    g_return_val_if_fail(size != NULL, NULL);
    g_return_val_if_fail(pix != NULL, NULL);
    size = g_object_ref(size);
    img = core_image_new();
    private = core_image_get_instance_private(img);
    core_size_copy(size, &private->size);
    g_object_unref(size);
    byte_per_pix = channel;
    n_pix = core_size_get_area(private->size);
    private->data = g_malloc(sizeof(guint8) * byte_per_pix * n_pix);
    for (i = 0; i < n_pix; ++i) {
        for (j = 0; j < byte_per_pix; ++j) {
            private->data[i * byte_per_pix + j] = pix[j];
        }
    }
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
CoreSize* core_image_get_size(CoreImage *self) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    g_return_val_if_fail(self != NULL, NULL);

    return g_object_ref(private->size);
}

guint8 core_image_get_channel(CoreImage *self) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, 0);
    private = core_image_get_instance_private(self);
    return private->channel;
}

guint8 core_image_get_byte_per_pixel(CoreImage *self) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, 0);
    private = core_image_get_instance_private(self);
    return private->channel * sizeof(gdouble);
}

gboolean core_image_reshape(CoreImage *self, CoreSize *size, GError **error) {
    CoreImagePrivate *private = NULL;
    GError *temp_error = NULL;
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(size != NULL, FALSE);

    size = g_object_ref(size);
    private = core_image_get_instance_private(self);
    if (core_size_get_area(private->size) == 0) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "Empty image shall not be reshaped");
        g_object_unref(size);
        return FALSE;
    }
    if (core_size_get_area(private->size) != core_size_get_area(size)) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "Reshape operation does not modify area.");
        g_object_unref(size);
        return FALSE;
    }

    if (!core_size_copy(size, &private->size)) {
        g_propagate_error(error, temp_error);
        g_object_unref(size);
        return FALSE;
    }

    g_object_unref(size);
    return TRUE;
}

gdouble *core_image_get_data(CoreImage *self) {
    CoreImagePrivate *private = NULL;
    g_return_val_if_fail(self != NULL, NULL);
    private = core_image_get_instance_private(self);
    return private->data;
}

gboolean
core_image_assign_data(CoreImage *self, gdouble *data, gsize data_len, guint8 channel, CoreSize *size, gboolean copy_data) {
    CoreImagePrivate *private = NULL;
    gdouble *_data = NULL;
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(data != NULL, FALSE);
    private = core_image_get_instance_private(self);
    size = g_object_ref(size);
    core_size_copy(size, &private->size);
    g_object_unref(size);
    if (!copy_data) {
        _data = data;
    } else {
        _data = g_malloc(sizeof(gdouble) * data_len);
        memcpy(_data, data, sizeof(gdouble) * data_len);
    }
    private->data = _data;
    private->channel = channel;
    return TRUE;
}

gdouble *core_image_get_pixel(CoreImage *self, guint32 x, guint32 y) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, NULL);

    private = core_image_get_instance_private(self);
    g_return_val_if_fail(x < core_size_get_width(private->size), NULL);
    g_return_val_if_fail(y < core_size_get_height(private->size), NULL);

    return private->data + (y * core_size_get_width(private->size) + x) * core_image_get_byte_per_pixel(self);
}
