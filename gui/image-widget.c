//
// Created by karl_ on 2020-4-30.
//

#include <gui/image-widget.h>

/* MACRO FUNCTIONS TO REDUCE MASSIVE REDUNDANCY. NOT METHODS. */
#define __IMAGE_WIDGET_COPY_DATA_C3(type)  \
for (i = 0; i < height; ++i) {  \
    for (j = 0; j < width; ++j) {  \
        for (k = 0; k < 3; ++k) {  \
        ((guint8 *) dst_data)[stride * i + j * 3 + k] =  \
            (guint8) (((type *) src_data)[width * 3 * i + j * 3 + k] * mag_ratio);  \
        }  \
    }  \
}

#define __IMAGE_WIDGET_COPY_DATA_C1(type)  \
for (i = 0; i < height; ++i) {  \
    for (j = 0; j < width; ++j) {  \
        for (k = 0; k < 3; ++k) {  \
        ((guint8 *) dst_data)[stride * i + j * 3 + k] =  \
            (guint8) ((type *) src_data)[width * i + j] * mag_ratio;  \
        }  \
    }  \
}

typedef struct {
    CoreImage *image;
} GuiImageWidgetPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(GuiImageWidget, gui_image_widget, GTK_TYPE_IMAGE)

/* DESTRUCTORS */
static void gui_image_widget_dispose(GObject *obj) {
    GuiImageWidgetPrivate *private;
    private = gui_image_widget_get_instance_private(GUI_IMAGE_WIDGET(obj));
    g_clear_object(&private->image);
    G_OBJECT_CLASS(gui_image_widget_parent_class)->dispose(obj);
}

static void gui_image_widget_finalize(GObject *obj) {
    G_OBJECT_CLASS(gui_image_widget_parent_class)->finalize(obj);
}

/* INIT FUNCTIONS */
static void gui_image_widget_class_init(GuiImageWidgetClass *cls) {
    GObjectClass *obj_cls = G_OBJECT_CLASS(cls);

    obj_cls->dispose = gui_image_widget_dispose;
    obj_cls->finalize = gui_image_widget_finalize;
}

static void gui_image_widget_init(GuiImageWidget *self) {}

/* CONSTRUCTORS */
GuiImageWidget *gui_image_widget_new_from_core_image(CoreImage *image) {
    GuiImageWidget *image_widget = NULL;
    g_return_val_if_fail(image != NULL, NULL);
    g_return_val_if_fail(!core_image_empty(image), NULL);
    image_widget = g_object_new(GUI_TYPE_IMAGE_WIDGET, NULL);
    gui_image_widget_update_image(image_widget, image);
    return image_widget;
}

/* PUBLIC METHODS */
gboolean gui_image_widget_update_image(GuiImageWidget *self, CoreImage *image) {
    CoreSize *size;
    GdkPixbuf *pixbuf;
    gsize i, j, k;
    gsize width, height;
    gsize stride;
    gdouble range, mag_ratio;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    gpointer src_data, dst_data;
    GuiImageWidgetPrivate *private;

    private = gui_image_widget_get_instance_private(self);
    g_clear_object(&private->image);
    private->image = g_object_ref(image);

    size = core_image_get_size(image);
    width = core_size_get_width(size);
    height = core_size_get_height(size);
    color_space = core_image_get_color_space(image);
    pixel_type = core_image_get_pixel_type(image);
    range = core_pixel_get_range(pixel_type);
    mag_ratio = 255.0 / range;

    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, core_size_get_width(size), core_size_get_height(size));

    src_data = core_image_get_data(image);
    dst_data = gdk_pixbuf_get_pixels(pixbuf);
    stride = gdk_pixbuf_get_rowstride(pixbuf);

    if (color_space == CORE_COLOR_SPACE_RGB) {
        if (core_pixel_is_double(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C3(gdouble)
        } else if (core_pixel_is_uint8(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C3(guint8)
        }
    } else if (color_space == CORE_COLOR_SPACE_GRAY_SCALE || color_space == CORE_COLOR_SPACE_BIN) {
        if (core_pixel_is_double(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C1(gdouble)
        } else if (core_pixel_is_uint8(pixel_type)) {
            __IMAGE_WIDGET_COPY_DATA_C1(guint8)
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
