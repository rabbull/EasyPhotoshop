//
// Created by karl on 31/5/2020.
//

#include "callbacks.h"
#include "image-widget.h"

#include <imgproc/color-convert.h>
#include <imgproc/histogram-equalization.h>

void grayscale(GtkWidget *widget, gpointer data) {
    struct grayscale_args *args = data;
    GuiImageWidget *img_widget = GUI_IMAGE_WIDGET(args->gui_image_widget);
    CoreImage *image = gui_image_widget_get_image(img_widget);
    imgproc_to_grayscale(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

void histeq(GtkWidget *widget, gpointer data) {
    struct histeq_args *args = data;
    GuiImageWidget *img_widget = GUI_IMAGE_WIDGET(args->gui_image_widget);
    CoreImage *image = gui_image_widget_get_image(img_widget);
    imgproc_histogram_equalization(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

void dct(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
}

void dct_drop(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
}

void idct(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
}

void open_file(GtkWidget *widget, gpointer data) {
    struct open_file_args *args = data;
    GtkWidget *dialog;
    GtkFileFilter *filter;
    char const *filename;
    CoreImage *image;
    GFile *selected_file;

    dialog = gtk_file_chooser_dialog_new("open file", args->parent, GTK_FILE_CHOOSER_ACTION_OPEN, "Cancel",
                                         GTK_RESPONSE_CANCEL, "Open", GTK_RESPONSE_ACCEPT, NULL);
    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "bmp");
    gtk_file_filter_add_pattern(filter, "*.bmp");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
        selected_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER (dialog));
        filename = g_file_get_path(selected_file);
        image = core_image_new_open(filename);
        gui_image_widget_update_image(args->gui_image_widget, image);
        g_object_unref(image);
        g_object_unref(selected_file);
    }
    gtk_widget_destroy(dialog);
}
