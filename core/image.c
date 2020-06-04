//
// Created by karl on 4/28/20.
//

#include <stdint.h>
#include <core/image.h>
#include <fileio/input-format-table.h>
#include <fileio/output-format-table.h>

typedef struct {
    gpointer data;
    CoreColorSpace color_space;
    CoreSize *size;
    CorePixelType pixel_type;
    guint8 channel;
} CoreImagePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(CoreImage, core_image, G_TYPE_OBJECT)

/* CONSTRUCTORS */
CoreImage *core_image_new(void) {
    return g_object_new(CORE_TYPE_IMAGE, NULL);
}

CoreImage *core_image_new_empty_with_size(CoreSize *size, CoreColorSpace color_space, CorePixelType pixel_type) {
    CoreImage *img;
    CoreImagePrivate *private;
    g_return_val_if_fail(size != NULL, NULL);
    g_return_val_if_fail(core_pixel_get_channel(pixel_type) == core_color_space_get_channel(color_space), NULL);

    size = g_object_ref(size);
    img = core_image_new();
    private = core_image_get_instance_private(img);
    core_size_copy(size, private->size);
    private->channel = core_color_space_get_channel(color_space);
    private->data = g_malloc(core_size_get_area(size) * core_pixel_get_size(pixel_type));
    g_object_unref(size);
    return img;
}

CoreImage *core_image_new_with_data(gpointer data, CoreColorSpace color_space, CorePixelType pixel_type, CoreSize *size,
                                    gboolean copy_data) {
    CoreImage *img;
    img = core_image_new();
    core_image_assign_data(img, data, color_space, pixel_type, size, copy_data);
    return img;
}

CoreImage *core_image_new_fill_with_color(CoreSize *size, CoreColorSpace color_space, CorePixelType pixel_type,
                                          gpointer pixel_data) {
    CoreImage *img;
    CoreImagePrivate *private;
    gpointer new_data, curr;
    gsize area, pixel_size, new_data_size;
    g_return_val_if_fail(size != NULL, NULL);
    g_return_val_if_fail(pixel_data != NULL, NULL);
    size = g_object_ref(size);
    img = core_image_new();
    private = core_image_get_instance_private(img);
    core_size_copy(size, private->size);
    area = core_size_get_area(size);
    g_object_unref(size);

    pixel_size = core_pixel_get_size(pixel_type);
    new_data_size = area * pixel_size;
    new_data = g_malloc(new_data_size);
    curr = new_data;
    while (curr < new_data + new_data_size) {
        memcpy(curr, pixel_data, pixel_size);
        curr = (gpointer) ((guint8 *) curr + pixel_size);
    }
    core_image_assign_data(img, new_data, color_space, pixel_type, size, FALSE);
    return img;
}

CoreImage *core_image_new_clone(CoreImage *old) {
    CoreImage *new;
    new = core_image_new();
    core_image_copy(old, new);
    return new;
}

CoreImage *core_image_new_open(GString *path) {
    CoreImage *image;
    FileIOInputFormatTable *table;
    gsize n;
    gsize i;
    input_method_t input_method;

    table = fileio_input_format_table_get_instance();
    n = fileio_input_format_table_get_length(table);
    for (i = 0; i < n; ++i) {
        input_method = fileio_input_format_table_get_input_method(table, i);
        image = input_method(path);
        if (image) {
            return image;
        }
    }

    g_object_unref(table);
    return NULL;
}

/* DESTRUCTORS */
static void core_image_dispose(GObject *obj) {
    CoreImagePrivate *private = core_image_get_instance_private(CORE_IMAGE(obj));
    g_clear_object(&private->size);
    G_OBJECT_CLASS(core_image_parent_class)->dispose(obj);
}

static void core_image_finalize(GObject *obj) {
    CoreImagePrivate *private = core_image_get_instance_private(CORE_IMAGE(obj));
    if (private->data) {
        g_free(private->data);
        private->data = NULL;
    }
    G_OBJECT_CLASS(core_image_parent_class)->finalize(obj);
}

/* INIT FUNCTIONS */

/* CONSTRUCTORS */
static void core_image_class_init(CoreImageClass *cls) {
    GObjectClass *obj_cls = G_OBJECT_CLASS(cls);

    obj_cls->dispose = core_image_dispose;
    obj_cls->finalize = core_image_finalize;
}

static void core_image_init(CoreImage *self) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    private->data = NULL;
    private->color_space = CORE_COLOR_SPACE_RGB;
    private->channel = 3;
    private->pixel_type = CORE_PIXEL_U3;
    private->size = core_size_new();
}

/* PUBLIC METHODS */
/* CLONE */
gboolean core_image_copy(CoreImage *self, CoreImage *another) {
    CoreImagePrivate *self_private, *another_private;
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(another != NULL, FALSE);
    self_private = core_image_get_instance_private(self);
    core_image_assign_data(another, self_private->data, self_private->color_space, self_private->pixel_type,
                           self_private->size, TRUE);
    return TRUE;
}

/* GETTERS */
CoreSize *core_image_get_size(CoreImage *self) {
    CoreImagePrivate *private = core_image_get_instance_private(self);
    g_return_val_if_fail(self != NULL, NULL);
    return g_object_ref(private->size);
}

gboolean core_image_empty(CoreImage *self) {
    CoreImagePrivate *private = NULL;
    g_return_val_if_fail(self != NULL, TRUE);
    private = core_image_get_instance_private(self);
    return private->data == NULL;
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
    return core_pixel_get_size(private->pixel_type);
}

gpointer core_image_get_data(CoreImage *self) {
    CoreImagePrivate *private = NULL;
    g_return_val_if_fail(self != NULL, NULL);
    private = core_image_get_instance_private(self);
    return private->data;
}

gpointer core_image_get_pixel(CoreImage *self, guint32 x, guint32 y) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, NULL);

    private = core_image_get_instance_private(self);
    g_return_val_if_fail(x < core_size_get_width(private->size), NULL);
    g_return_val_if_fail(y < core_size_get_height(private->size), NULL);

    return (gpointer) ((guint8 *) private->data +
                       (y * core_size_get_width(private->size) + x) * core_image_get_byte_per_pixel(self));
}

CoreColorSpace core_image_get_color_space(CoreImage *self) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, -1);
    private = core_image_get_instance_private(self);
    return private->color_space;
}

CorePixelType core_image_get_pixel_type(CoreImage *self) {
    CoreImagePrivate *private;
    g_return_val_if_fail(self != NULL, -1);
    private = core_image_get_instance_private(self);
    return private->pixel_type;
}

/* OTHERS */
gboolean core_image_reshape(CoreImage *self, CoreSize *size, GError **error) {
    CoreImagePrivate *private = NULL;
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
    core_size_copy(size, private->size);
    g_object_unref(size);
    return TRUE;
}

gboolean core_image_assign_data(CoreImage *self, gpointer data, CoreColorSpace color_space, CorePixelType pixel_type,
                                CoreSize *size, gboolean copy_data) {
    CoreImagePrivate *private = NULL;
    gpointer _data = NULL;
    gsize area;
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(data != NULL, FALSE);
    g_return_val_if_fail(core_color_space_get_channel(color_space) == core_pixel_get_channel(pixel_type), FALSE);

    private = core_image_get_instance_private(self);
    size = g_object_ref(size);
    area = core_size_get_area(size);
    core_size_copy(size, private->size);
    g_object_unref(size);
    if (!copy_data) {
        _data = data;
    } else {
        _data = g_malloc(core_pixel_get_size(pixel_type) * area);
        memcpy(_data, data, core_pixel_get_size(pixel_type) * area);
    }
    if (!core_image_empty(self)) {
        g_free(private->data);
    }
    private->data = _data;
    private->channel = core_color_space_get_channel(color_space);
    private->color_space = color_space;
    private->pixel_type = pixel_type;
    return TRUE;
}

char const *CORE_IMAGE_SAVE_METHOD_BMP = "BMP";

gboolean core_image_save(CoreImage *self, GString *path, char const *method) {
    FileIOOutputFormatTable *table = fileio_output_format_table_get_instance();
    output_method_t output_method = fileio_output_format_table_get_output_method(table, method);
    if (output_method == NULL) {
        return FALSE;
    }
    return output_method(self, path);
}