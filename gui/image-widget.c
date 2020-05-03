//
// Created by karl_ on 2020-4-30.
//

#include <gui/image-widget.h>

G_DEFINE_TYPE(GuiImageWidget, gui_image_widget, GTK_TYPE_IMAGE)

static void gui_image_widget_class_init (GuiImageWidgetClass *cls) {}

static void gui_image_widget_init (GuiImageWidget *self) {}

GuiImageWidget* gui_image_widget_new_from_core_image(CoreImage* image) {
    GuiImageWidget* image_widget = NULL;
    CoreSize *size = 0;
    GdkPixbuf *pixbuf;
    gsize i, j, area;
    guint8 channel;

    // claim resources
    image = g_object_ref(image);

    size = core_image_get_size(image);
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, core_size_get_width(size), core_size_get_height(size));
    area = core_size_get_area(size);
    channel = core_image_get_channel(image);
    if (channel == 3) {  // rgb
        for (i = 0; i < area; ++i) {
            for (j = 0; j < channel; ++j) {
                // note: No information about how pixel data are stored in pixbuf instance provided.
                // I guess it'll work and it does work for now.
                gdk_pixbuf_get_pixels(pixbuf)[i * 3 + j] = core_image_get_data(image)[i * 3 + j];
            }
        }
    } else if (channel == 1) {  // gray scale
        for (i = 0; i < area; ++i) {
            for (j = 0; j < 3; ++j) {
                gdk_pixbuf_get_pixels(pixbuf)[i * 3 + j] = core_image_get_data(image)[i];
            }
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

static void gui_image_widget_dispose (GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->dispose (obj);
}

static void gui_image_widget_finalize (GObject *obj) {
    G_OBJECT_CLASS (gui_image_widget_parent_class)->finalize (obj);
}
