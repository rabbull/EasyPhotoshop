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

G_DEFINE_TYPE(GuiImageWidget, gui_image_widget, GTK_TYPE_IMAGE)

static void gui_image_widget_class_init(GuiImageWidgetClass *cls) {}

static void gui_image_widget_init(GuiImageWidget *self) {}

GuiImageWidget *gui_image_widget_new_from_core_image(CoreImage *image) {
    GuiImageWidget *image_widget = NULL;
    CoreSize *size = 0;
    GdkPixbuf *pixbuf;
    gsize i, j, area;
    guint8 channel;
    gdouble range, mag_ratio;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    gpointer src_data, dst_data;
    g_return_val_if_fail(image != NULL, NULL);
    g_return_val_if_fail(!core_image_empty(image), NULL);

    image = g_object_ref(image);

    size = core_image_get_size(image);
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, core_size_get_width(size), core_size_get_height(size));
    area = core_size_get_area(size);
    channel = core_image_get_channel(image);
    color_space = core_image_get_color_space(image);
    pixel_type = core_image_get_pixel_type(image);
    range = core_pixel_get_range(pixel_type);
    mag_ratio = 255.0 / range;
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

    image_widget = g_object_new(GUI_TYPE_IMAGE_WIDGET, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image_widget), pixbuf);

    // release resources
    g_object_unref(image);
    g_object_unref(pixbuf);
    g_object_unref(size);
    return image_widget;
}

static void gui_image_widget_dispose(GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->dispose(obj);
}

static void gui_image_widget_finalize(GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->finalize(obj);
}
