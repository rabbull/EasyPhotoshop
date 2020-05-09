//
// Created by karl_ on 2020-4-30.
//

#include <gui/image-widget.h>

/* MACRO FUNCTIONS TO REDUCE REDUNDANCY, NOT A METHOD */
#define __IMAGE_WIDGET_COPY_DATA_C1(i, j, type, src, dst, ratio)  \
for (i = 0; i < area; ++i) {  \
    for (j = 0; j < 3; ++j) {  \
        ((guint8 *) dst)[i * 3 + j] = ((type *) src)[i] * ratio;  \
    }  \
}

#define __IMAGE_WIDGET_COPY_DATA_C3(i, j, type, src, dst, ratio)  \
for (i = 0; i < area; ++i) {  \
    for (j = 0; j < 3; ++j) {  \
        ((guint8 *) dst)[i * 3 + j] = ((type *) src)[i * 3 + j] * ratio;  \
    }  \
}

typedef struct {
    CoreImage* image;
} GuiImageWidgetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GuiImageWidget, gui_image_widget, GTK_TYPE_IMAGE)

static void gui_image_widget_class_init(GuiImageWidgetClass *cls) {}

static void gui_image_widget_init(GuiImageWidget *self) {}

static void gui_image_widget_dispose(GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->dispose(obj);
}

static void gui_image_widget_finalize(GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->finalize(obj);
}

GuiImageWidget *gui_image_widget_new_from_core_image(CoreImage *image) {
    GuiImageWidget *image_widget = NULL;
    g_return_val_if_fail(image != NULL, NULL);
    g_return_val_if_fail(!core_image_empty(image), NULL);
    image_widget = g_object_new(GUI_TYPE_IMAGE_WIDGET, NULL);
    gui_image_widget_update_image(image_widget, image);
    return image_widget;
}

gboolean gui_image_widget_update_image(GuiImageWidget *self, CoreImage *image) {
    CoreSize *size;
    GdkPixbuf *pixbuf;
    gsize i, j, area;
    gdouble range, mag_ratio;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    gpointer src_data, dst_data;
    GuiImageWidgetPrivate *private;

    private = gui_image_widget_get_instance_private(self);
    g_object_unref(private->image);
    private->image = g_object_ref(image);

    size = core_image_get_size(image);
    area = core_size_get_area(size);
    color_space = core_image_get_color_space(image);
    pixel_type = core_image_get_pixel_type(image);
    range = core_pixel_get_range(pixel_type);
    mag_ratio = 255.0 / range;

    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, core_size_get_width(size), core_size_get_height(size));

    src_data = core_image_get_data(image);
    dst_data = gdk_pixbuf_get_pixels(pixbuf);

    if (color_space == CORE_COLOR_SPACE_RGB) {
        if (core_pixel_is_uint8(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C3(i, j, guint8, src_data, dst_data, mag_ratio)
        } else if (core_pixel_is_double(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C3(i, j, gdouble, src_data, dst_data, mag_ratio)
        }
    } else if (color_space == CORE_COLOR_SPACE_GRAY_SCALE || color_space == CORE_COLOR_SPACE_BIN) {
        if (core_pixel_is_uint8(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C1(i, j, guint8, src_data, dst_data, mag_ratio)
        } else if (core_pixel_is_double(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C1(i, j, gdouble, src_data, dst_data, mag_ratio)
        }
    }

    gtk_image_set_from_pixbuf(GTK_IMAGE(self), pixbuf);

    g_object_unref(pixbuf);
    g_object_unref(size);
    return TRUE;
}

CoreImage *gui_image_widget_get_image(GuiImageWidget *self) {
    GuiImageWidgetPrivate *private;
    private = gui_image_widget_get_instance_private(self);
    return g_object_ref(private->image);
}
