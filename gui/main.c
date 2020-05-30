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

void histeq(GtkWidget* widget, gpointer data) {
    GuiImageWidget* img_widget = GUI_IMAGE_WIDGET(data);
    CoreImage* image = gui_image_widget_get_image(img_widget);
    imgproc_histogram_equalization(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

void open(GtkWidget* widget,gpointer data){
    GtkWidget *dialog;
    dialog =gtk_file_chooser_dialog_new("open file",NULL,GTK_FILE_CHOOSER_ACTION_OPEN,
                                                GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
                                                GTK_STOCK_SAVE,GTK_RESPONSE_ACCEPT,
                                                NULL);
    GtkFileFilter *filter;
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter,"bmp");
    gtk_file_filter_add_pattern(filter,"*.bmp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog),filter);
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {

        GFile *file;
        file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (dialog));

        g_object_unref(file);
    }
    gtk_widget_destroy (dialog);
}


int gui_main(int argc, char **argv) {
    GtkWidget *window, *box_main, *box_buttons;
    GtkWidget *button_histeq, *button_grayscale,*button_open;
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
    g_signal_connect(G_OBJECT(button_grayscale), "clicked", grayscale, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_grayscale));

    button_histeq = gtk_button_new_with_label("histeq");
    g_signal_connect(G_OBJECT(button_histeq), "clicked", histeq, gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons), GTK_WIDGET(button_histeq));

    button_open = gtk_button_new_with_label("open");
    g_signal_connect(G_OBJECT(button_open),"clicked",open,gui_image_widget);
    gtk_container_add(GTK_CONTAINER(box_buttons),GTK_WIDGET(button_open));

    gtk_widget_show_all(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();

    g_object_unref(image);

    return EXIT_SUCCESS;
}
