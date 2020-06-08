//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include <gui/image-widget.h>

#include "main.h"
#include "callbacks.h"

int gui_main(int argc, char **argv) {
    GtkWidget *window, *box_main, *box_buttons;
    GtkWidget *button_histeq, *button_grayscale, *button_dither, *button_ordered_dither, *button_lpc, *button_open;
    GtkWidget *button_ilpc;
    GuiImageWidget *gui_image_widget;
    CoreImage *image;

    struct grayscale_args grayscale_args;
    struct histeq_args histeq_args;
    struct lpc_args lpc_args;
    struct open_file_args open_file_args;
    struct dither_args dither_args;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    box_main = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(box_main));

    image = NULL;
    gui_image_widget = GUI_IMAGE_WIDGET(gui_image_widget_new());
    gtk_container_add(GTK_CONTAINER(box_main), GTK_WIDGET(gui_image_widget));

    box_buttons = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    gtk_container_add(GTK_CONTAINER(box_main), GTK_WIDGET(box_buttons));

    /* CONVERT TO GRAYSCALE */
    button_grayscale = gtk_button_new_with_label("conversion to grayscale");
    grayscale_args.gui_image_widget = gui_image_widget;
    g_signal_connect(G_OBJECT(button_grayscale), "clicked", G_CALLBACK(grayscale), &grayscale_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_grayscale));

    /* HISTOGRAM EQUALIZATION */
    button_histeq = gtk_button_new_with_label("histogram equalization");
    histeq_args.gui_image_widget = gui_image_widget;
    g_signal_connect(G_OBJECT(button_histeq), "clicked", G_CALLBACK(histeq), &histeq_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_histeq));

    /* DITHERING */
    button_dither = gtk_button_new_with_label("dithering");
    dither_args.parent = GTK_WINDOW(window);
    dither_args.gui_image_widget = gui_image_widget;
    g_signal_connect(G_OBJECT(button_dither), "clicked", G_CALLBACK(dither), &dither_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_dither));

    button_ordered_dither = gtk_button_new_with_label("ordered dithering");
    g_signal_connect(G_OBJECT(button_ordered_dither), "clicked", G_CALLBACK(ordered_dither), &dither_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_ordered_dither));

    /* PREDICTIVE CODING */
    button_lpc = gtk_button_new_with_label("predictive coding");
    lpc_args.parent = GTK_WINDOW(window);
    lpc_args.gui_image_widget = gui_image_widget;
    g_signal_connect(G_OBJECT(button_lpc), "clicked", G_CALLBACK(lpc), &lpc_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_lpc));

    button_ilpc = gtk_button_new_with_label("predictive decoding");
    g_signal_connect(G_OBJECT(button_ilpc), "clicked", G_CALLBACK(ilpc), &lpc_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_ilpc));

    /* OPEN FILE */
    button_open = gtk_button_new_with_label("open file");
    open_file_args.parent = GTK_WINDOW(window);
    open_file_args.gui_image_widget = gui_image_widget;
    g_signal_connect(G_OBJECT(button_open), "clicked", G_CALLBACK(open_file), &open_file_args);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_open));

    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    g_object_unref(image);

    return EXIT_SUCCESS;
}
