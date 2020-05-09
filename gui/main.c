//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gui/image-widget.h>
#include <core/lena.h>
#include <imgproc/color-convert.h>
#include <imgproc/histogram-equalization.h>

void grayscale(GtkWidget* widget, gpointer data) {
    GuiImageWidget *img_widget = GUI_IMAGE_WIDGET(data);
    CoreImage *image = gui_image_widget_get_image(img_widget);
    imgproc_to_grayscale(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

int gui_main(int argc, char **argv) {
    GtkWidget *window, *button_grayscale, *box;
    GuiImageWidget *gui_image_widget;
    CoreImage *image;

    gtk_init(&argc, &argv);


    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box));

    image = core_image_new_lena();
    gui_image_widget = GUI_IMAGE_WIDGET(gui_image_widget_new_from_core_image(image));
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(gui_image_widget));

    button_grayscale = gtk_button_new_with_label("grayscale");
    g_signal_connect(G_OBJECT(button_grayscale), "clicked", grayscale, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box), GTK_WIDGET(button_grayscale));

    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    g_object_unref(image);

    return EXIT_SUCCESS;
}
