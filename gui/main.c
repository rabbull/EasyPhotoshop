//
// Created by karl on 4/28/20.
//

#include <stdlib.h>
#include <gtk/gtk.h>
#include "main.h"

int gui_main(int argc, char **argv) {
    GtkWidget *window;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "EasyPhotoshop");
    gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_show(window);

    g_signal_connect_swapped(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return EXIT_SUCCESS;
}
