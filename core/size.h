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

// clone
gboolean core_size_copy(CoreSize *self, CoreSize **another, GError **error);

// getters
guint32 core_size_get_width(CoreSize *self);

guint32 core_size_get_height(CoreSize *self);

// setters
gboolean core_size_set_width(CoreSize *self, guint32 const width);

gboolean core_size_set_height(CoreSize *self, guint32 const height);

// converters
gboolean core_size_to_string(CoreSize *self, GString **string, GError **error);

G_END_DECLS

#endif //EASYPHOTOSHOP_SIZE_H
