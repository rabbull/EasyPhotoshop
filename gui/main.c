//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gui/image-widget.h>
#include <core/lena.h>
#include <imgproc/color-convert.h>
#include <imgproc/histogram-equalization.h>

int gui_main(int argc, char **argv) {
    GtkWidget *window;
    GuiImageWidget *gui_image_widget;
    CoreImage *image;

    gtk_init(&argc, &argv);

    image = core_image_new_lena();
    gui_image_widget = GUI_IMAGE_WIDGET(gui_image_widget_new_from_core_image(image));
    g_object_unref(image);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(gui_image_widget));
    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return EXIT_SUCCESS;
}
