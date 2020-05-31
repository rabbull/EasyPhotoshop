//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gui/image-widget.h>
#include <core/lena.h>

#include "main.h"
#include "callbacks.h"

int gui_main(int argc, char **argv) {
    GtkWidget *window, *box_main, *box_buttons;
    GtkWidget *button_histeq, *button_grayscale, *button_open;
    GuiImageWidget *gui_image_widget;
    CoreImage *image;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box_main));

    image = core_image_new_lena();
    gui_image_widget = GUI_IMAGE_WIDGET(gui_image_widget_new_from_core_image(image));
    gtk_container_add(GTK_CONTAINER(box_main), GTK_WIDGET(gui_image_widget));

    box_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(box_main), GTK_WIDGET(box_buttons));

    button_grayscale = gtk_button_new_with_label("grayscale");
    g_signal_connect(G_OBJECT(button_grayscale), "clicked", (GCallback) grayscale, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_grayscale));

    button_histeq = gtk_button_new_with_label("histeq");
    g_signal_connect(G_OBJECT(button_histeq), "clicked", (GCallback) histeq, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_histeq));

    button_open = gtk_button_new_with_label("open file");
    g_signal_connect(G_OBJECT(button_open), "clicked", (GCallback) open_file, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_open));

    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    g_object_unref(image);

    return EXIT_SUCCESS;
}
