//
// Created by karl on 31/5/2020.
//

#include "callbacks.h"
#include "image-widget.h"

#include <stdlib.h>

#include <imgproc/color-convert.h>
#include <imgproc/histogram-equalization.h>
#include <imgproc/uniform-quantization.h>
#include <imgproc/lossless-predictive-coding.h>
#include <imgproc/discrete-cosine-transform.h>

static char **request_additional(char const *title, GtkWindow *parent, size_t argc, char const *const *names) {
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

static void free_additional(char **arguments, size_t argc) {
    int i;
    for (i = 0; i < argc; ++i) {
        free(arguments[i]);
    }
    free(arguments);
}

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

void dct(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image, *transformed;
    long block_size;
    char const *field_names[] = {"Block Size"};
    char **fields;

    fields = request_additional("Discrete Cosine Transform", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    block_size = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);

    image = gui_image_widget_get_image(img_widget);
    transformed = imgproc_discrete_cosine_transform(image, block_size);
    g_object_unref(image);
    if (transformed == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, transformed);
    g_object_unref(transformed);
}

void dct_drop(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image, *dropped;
    long block_size;
    char const *field_names[] = {"Block Size"};
    char **fields;

    fields = request_additional("Discrete Cosine Transform Drop Half", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    block_size = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);

    image = gui_image_widget_get_image(img_widget);
    dropped = imgproc_discrete_cosine_transform_drop_half(image, block_size);
    g_object_unref(image);
    if (dropped == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, dropped);
    g_object_unref(dropped);
}

void idct(GtkWidget *widget, gpointer data) {
    struct dct_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image, *recovered;
    long block_size;
    char const *field_names[] = {"Block Size"};
    char **fields;

    fields = request_additional("Inverse Discrete Cosine Transform", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    block_size = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);

    image = gui_image_widget_get_image(img_widget);
    recovered = imgproc_inverse_discrete_cosine_transform(image, block_size);
    g_object_unref(image);
    if (recovered == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, recovered);
    g_object_unref(recovered);
}

void uniform(GtkWidget *widget, gpointer data) {
    struct uniform_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *uniformed;
    long ratio;
    char const *field_names[] = {"Ratio"};
    char **fields;

    fields = request_additional("Uniform Quantization", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    ratio = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);

    uniformed = imgproc_uniform_quantization(image, ratio);
    g_object_unref(image);
    if (uniformed == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, uniformed);
    g_object_unref(uniformed);
}


void threshold(GtkWidget *widget, gpointer data) {
    struct threshold_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *binarized = NULL;
    guint threshold;
    char const *field_names[] = {"Threshold"};
    char **fields;

    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        goto fail;
    }
    fields = request_additional("Binarize", args->parent, 1, field_names);
    if (fields == NULL) {
        goto fail;
    }
    threshold = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);
    imgproc_to_binary_threshold(image, &binarized, threshold, FALSE);
    if (binarized == NULL) {
        goto fail;
    }
    gui_image_widget_update_image(img_widget, binarized);
    g_object_unref(binarized);

    fail:
    g_object_unref(image);
}

void dither(GtkWidget *widget, gpointer data) {
    struct dither_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *dithered = NULL;
    guint rank;
    char const *field_names[] = {"Rank"};
    char **fields;
    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        return;
    }
    fields = request_additional("Dither", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    rank = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);
    dithered = imgproc_to_binary_dither(image, rank, 0);
    g_object_unref(image);
    if (dithered == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, dithered);
    g_object_unref(dithered);
}

void ordered_dither(GtkWidget *widget, gpointer data) {
    struct dither_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *dithered = NULL;
    char const *field_names[] = {"Rank"};
    char **fields;
    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        return;
    }
    fields = request_additional("Ordered Dither", args->parent, 1, field_names);
    if (fields == NULL) {
        return;
    }
    guint rank = strtol(fields[0], NULL, 10);
    free_additional(fields, 1);
    dithered = imgproc_to_binary_dither(image, rank, 1);
    g_object_unref(image);
    if (dithered == NULL) {
        return;
    }
    gui_image_widget_update_image(img_widget, dithered);
    g_object_unref(dithered);
}

void predictive_coding(GtkWidget *widget, gpointer data) {
    struct lpc_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *coded_image = NULL;
    guint32 rank;
    gdouble *coef = NULL;
    gdouble sum;
    char *cursor = NULL;
    char const *field_names[] = {"Rank", "Coefficients (separated by space)"};
    char **fields = NULL;
    guint32 i;

    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        goto fail;
    }

    fields = request_additional("Predictive Coding", args->parent, 2, field_names);
    if (fields == NULL) {
        goto fail;
    }

    rank = strtol(fields[0], NULL, 10);
    cursor = fields[1];
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
    /* sum of all coefs should be 1 */
    if (sum < 1 - 3 * DBL_EPSILON || sum >= 1 + 3 * DBL_EPSILON) {
        goto fail;
    }
    coded_image = imgproc_predictive_coding(image, rank, coef);
    g_object_unref(image);
    if (coded_image == NULL) {
        goto fail;
    }
    gui_image_widget_update_image(img_widget, coded_image);
    g_object_unref(coded_image);

    fail:
    if (fields) {
        free_additional(fields, 2);
    }
    free(coef);
}

void predictive_decoding(GtkWidget *widget, gpointer data) {
    struct lpc_args *args = data;
    GuiImageWidget *img_widget = args->gui_image_widget;
    CoreImage *image = gui_image_widget_get_image(img_widget);
    CoreImage *coded_image = NULL;
    guint32 rank;
    gdouble *coef = NULL;
    gdouble sum;
    char *cursor = NULL;
    char const *additional_argument_names[] = {"Rank", "Coefficients"};
    char **additional_arguments = NULL;
    guint32 i;

    if (core_image_get_color_space(image) != CORE_COLOR_SPACE_GRAY_SCALE) {
        goto fail;
    }

    additional_arguments = request_additional("Predictive Decoding", args->parent, 2,
                                              additional_argument_names);
    if (additional_arguments == NULL) {
        goto fail;
    }

    rank = strtol(additional_arguments[0], NULL, 10);
    cursor = additional_arguments[1];
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
    if (sum < 1 - 3 * DBL_EPSILON || sum >= 1 + 3 * DBL_EPSILON) {
        goto fail;
    }
    coded_image = imgproc_predictive_decoding(image, rank, coef);
    gui_image_widget_update_image(img_widget, coded_image);
    g_object_unref(image);
    g_object_unref(coded_image);

    for (i = 0; i < 2; ++i) {
        free(additional_arguments[i]);
    }
    free(additional_arguments);
    free(coef);
    return;

    fail:
    if (additional_arguments) {
        for (i = 0; i < 2; ++i) {
            free(additional_arguments[i]);
        }
        free(additional_arguments);
    }
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
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        selected_file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));
        filename = g_file_get_path(selected_file);
        image = core_image_new_open(filename);
        gui_image_widget_update_image(args->gui_image_widget, image);
        g_object_unref(image);
        g_object_unref(selected_file);
    }
    gtk_widget_destroy(dialog);
}

void save_file(GtkWidget *widget, gpointer data) {
    struct save_file_args *args = data;
    GtkWidget *dialog;
    char const *file;
    GuiImageWidget *img_widget = GUI_IMAGE_WIDGET(args->gui_image_widget);

    dialog = gtk_file_chooser_dialog_new("save file", args->parent, GTK_FILE_CHOOSER_ACTION_SAVE, "Cancel",
                                         GTK_RESPONSE_CANCEL, "Save", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (!core_image_save(gui_image_widget_get_image(img_widget), file, CORE_IMAGE_SAVE_METHOD_BMP)) {
            g_printerr("Failed to save!\n");
        }
    }
    gtk_widget_destroy(dialog);
}
