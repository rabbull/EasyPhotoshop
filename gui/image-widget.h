//
// Created by karl_ on 2020-4-30.
//

#ifndef EASYPHOTOSHOP_IMAGE_WIDGET_H
#define EASYPHOTOSHOP_IMAGE_WIDGET_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include <core/image.h>

G_BEGIN_DECLS

#define GUI_TYPE_IMAGE_WIDGET gui_image_widget_get_type()
G_DECLARE_DERIVABLE_TYPE(GuiImageWidget, gui_image_widget, GUI, IMAGE_WIDGET, GtkImage)

struct _GuiImageWidgetClass {
    GtkImageClass parent_class;
    gpointer padding[12];
};

GuiImageWidget* gui_image_widget_new_from_core_image(CoreImage* image);

G_END_DECLS

#endif //EASYPHOTOSHOP_IMAGE_WIDGET_H
