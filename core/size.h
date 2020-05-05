//
// Created by karl on 4/28/20.
//

#ifndef EASYPHOTOSHOP_SIZE_H
#define EASYPHOTOSHOP_SIZE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define CORE_TYPE_SIZE core_size_get_type()

G_DECLARE_FINAL_TYPE(CoreSize, core_size, CORE, SIZE, GObject)

CoreSize *core_size_new(void);

CoreSize *core_size_new_with_value(guint32 width, guint32 height);

// clone
gboolean core_size_copy(CoreSize *self, CoreSize **another);

// getters
guint32 core_size_get_width(CoreSize *self);

guint32 core_size_get_height(CoreSize *self);

guint32 core_size_get_area(CoreSize *self);

// setters
gboolean core_size_set_width(CoreSize *self, guint32 width);

gboolean core_size_set_height(CoreSize *self, guint32 height);

// converters
gboolean core_size_to_string(CoreSize *self, GString **string);

G_END_DECLS

#endif //EASYPHOTOSHOP_SIZE_H
