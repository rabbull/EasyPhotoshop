//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include "main.h"
#include "../core/image.h"

int gui_main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *image_widget;
    CoreImage *image;
    CoreSize *size;
    guint8 pix[3] = {255, 0, 0};
    GdkPixbuf *pixbuf;
    gsize i, j;

    gtk_init(&argc, &argv);

    size = core_size_new_with_value(640, 480);
    image = core_image_new_fill_with_color(size, 24, pix);
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, core_size_get_width(size), core_size_get_height(size));
    for (i = 0; i < core_size_get_area(size); ++i) {
        for (j = 0; j < 3; ++j) {
            // note: No information about how pixel data are stored in pixbuf instance provided.
            // I guess it'll work and it does work for now.
            gdk_pixbuf_get_pixels(pixbuf)[i * 3 + j] = core_image_get_data(image)[i * 3 + j];
        }
    }
    image_widget = gtk_image_new_from_pixbuf(pixbuf);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(image_widget));
    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return EXIT_SUCCESS;
}
