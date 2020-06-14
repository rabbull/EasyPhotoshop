//
// Created by karl on 31/5/2020.
//

#ifndef EASYPHOTOSHOP_CALLBACKS_H
#define EASYPHOTOSHOP_CALLBACKS_H

#include <gtk/gtk.h>
#include "image-widget.h"

struct grayscale_args {
    GuiImageWidget *gui_image_widget;
};

void grayscale(GtkWidget *widget, gpointer data);

struct histeq_args {
    GuiImageWidget *gui_image_widget;
};

void histeq(GtkWidget *widget, gpointer data);

struct dct_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void dct(GtkWidget *widget, gpointer data);

void dct_drop(GtkWidget *widget, gpointer data);

void idct(GtkWidget *widget, gpointer data);

struct uniform_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void uniform(GtkWidget *widget, gpointer data);

struct threshold_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void threshold(GtkWidget *widget, gpointer data);

struct dither_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void dither(GtkWidget *widget, gpointer data);

void ordered_dither(GtkWidget *widget, gpointer data);

struct lpc_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void predictive_coding(GtkWidget *widget, gpointer data);

void predictive_decoding(GtkWidget *widget, gpointer data);

struct open_file_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void open_file(GtkWidget *widget, gpointer data);

struct save_file_args {
    GtkWindow *parent;
    GuiImageWidget *gui_image_widget;
};

void save_file(GtkWidget *widget, gpointer data);

#endif //EASYPHOTOSHOP_CALLBACKS_H
