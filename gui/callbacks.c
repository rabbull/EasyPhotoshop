//
// Created by karl on 31/5/2020.
//

#include "callbacks.h"
#include "image-widget.h"

#include <stdlib.h>

#include <imgproc/color-convert.h>
#include <imgproc/histogram-equalization.h>
#include <imgproc/lossless-predictive-coding.h>

void grayscale(GtkWidget *widget, gpointer data) {
    struct grayscale_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    imgproc_to_grayscale(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

void histeq(GtkWidget *widget, gpointer data) {
    struct histeq_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    imgproc_histogram_equalization(image, &image);
    gui_image_widget_update_image(img_widget, image);
    g_object_unref(image);
}

static char **request_arguments(char const *title, GtkWindow *parent, size_t argc, char const *const *names) {
    char **args = NULL;
    char const *arg = NULL;
    size_t i;
    GtkWidget *dialog;
    GtkWidget *grid;
    GtkWidget **labels;
    GtkWidget **entries;
    dialog = gtk_dialog_new_with_buttons(title, parent, GTK_DIALOG_MODAL,
                                         "Confirm", GTK_RESPONSE_ACCEPT,
                                         "Cancel", GTK_RESPONSE_CANCEL, NULL);
    grid = gtk_grid_new();
    labels = malloc(sizeof(gpointer) * argc);
    entries = malloc(sizeof(gpointer) * argc);
    for (i = 0; i < argc; ++i) {
        labels[i] = gtk_label_new(names[i]);
        entries[i] = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), labels[i], 0, i, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), entries[i], 1, i, 1, 1);
    }
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid, 0, 1, 0);
    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        args = g_malloc(sizeof(char const *) * argc);
        for (i = 0; i < argc; ++i) {
            arg = gtk_entry_get_text(GTK_ENTRY(entries[i]));
            args[i] = g_malloc(strlen(arg) + 1);
            strcpy(args[i], arg);
        }
    }
    gtk_widget_destroy(dialog);
    free(labels);
    free(entries);
    return args;
}

void dither(GtkWidget *widget, gpointer data) {
    struct dither_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *dithered = NULL;
    char const *argument_names[] = {"Rank"};
    char **requested_arguments;
    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        return;
    }
    requested_arguments = request_arguments("Dither", args->parent, 1, argument_names);
    if (requested_arguments == NULL) {
        return;
    }
    guint rank = strtol(requested_arguments[0], NULL, 10);
    dithered = imgproc_to_binary_dither(image, rank, 1);
    gui_image_widget_update_image(img_widget, dithered);
    g_object_unref(image);
}

void ordered_dither(GtkWidget *widget, gpointer data) {
    struct dither_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *dithered = NULL;
    char const *argument_names[] = {"Rank"};
    char **requested_arguments;
    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        return;
    }
    requested_arguments = request_arguments("Ordered Dither", args->parent, 1, argument_names);
    guint rank = strtol(requested_arguments[0], NULL, 10);
    dithered = imgproc_to_binary_dither(image, rank, 1);
    gui_image_widget_update_image(img_widget, dithered);
    g_object_unref(image);
}

void lpc(GtkWidget *widget, gpointer data) {
    struct lpc_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *coded_image = NULL;
    GtkWidget *dialog = NULL;
    GtkWidget *label_rank, *label_coef;
    GtkWidget *entry_rank, *entry_coef;
    GtkWidget *grid_layout;
    guint32 rank;
    gdouble *coef = NULL;
    gdouble sum;
    char *coef_text = NULL, *cursor = NULL;
    guint32 i;

    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        goto fail;
    }

    dialog = gtk_dialog_new_with_buttons("Rank and Coefficients", args->parent, GTK_DIALOG_MODAL, "Confirm",
                                         GTK_RESPONSE_ACCEPT, "Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_CANCEL);

    label_rank = gtk_label_new("Rank");
    label_coef = gtk_label_new("Coefficients");
    entry_rank = gtk_entry_new();
    entry_coef = gtk_entry_new();

    grid_layout = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid_layout), label_rank, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_layout), label_coef, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_layout), entry_rank, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid_layout), entry_coef, 1, 1, 1, 1);

    gtk_grid_set_column_spacing(GTK_GRID(grid_layout), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid_layout), 5);

    gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid_layout, 0, 1, 0);
    gtk_widget_show_all(dialog);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        rank = strtol(gtk_entry_get_text(GTK_ENTRY(entry_rank)), NULL, 10);
        coef_text = calloc(strlen(gtk_entry_get_text(GTK_ENTRY(entry_rank))) + 1, 1);
        cursor = coef_text;
        strcpy(coef_text, gtk_entry_get_text(GTK_ENTRY(entry_coef)));
        coef = malloc(sizeof(gdouble) * rank);
        sum = 0;
        for (i = 0; i < rank; ++i) {
            while (*cursor == ' ') {
                cursor += 1;
            }
            coef[i] = strtod(cursor, &cursor);
            if (coef[i] < 0) {
                goto fail;
            }
            sum += coef[i];
        }
        if (sum != 1) {
            goto fail;
        }
        coded_image = imgproc_lossless_predictive_coding(image, rank, coef);
        gui_image_widget_update_image(img_widget, coded_image);
        g_object_unref(image);
        g_object_unref(coded_image);

        free(coef_text);
        free(coef);
    }
    gtk_widget_destroy(dialog);
    return;

    fail:
    if (dialog) gtk_widget_destroy(dialog);
    free(coef_text);
    free(coef);
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
