//
// Created by karl on 4/28/20.
//

#include <core/size.h>


struct _CoreSize {
    GObject parent_instance;

    gint32 width;
    gint32 height;
};

G_DEFINE_TYPE(CoreSize, core_size, G_TYPE_OBJECT)


// constructors
static void core_size_class_init(CoreSizeClass *cls) {}

static void core_size_init(CoreSize *self) {
    self->width = self->height = 0;
}

CoreSize *core_size_new(void) {
    return g_object_new(CORE_TYPE_SIZE, NULL);
}

CoreSize *core_size_new_with_value(guint32 width, guint32 height) {
    CoreSize *size;
    size = core_size_new();
    size->width = width;
    size->height = height;
    return size;
}

CoreSize *core_size_new_clone(CoreSize* old) {
    CoreSize* new;
    new = core_size_new();
    core_size_copy(old, new);
    return new;
}

// destructors
static void core_size_dispose(GObject *obj) {
    g_print("core size obj dispose.\n");
    G_OBJECT_CLASS(core_size_parent_class)->dispose(obj);
}

static void core_size_finalize(GObject *obj) {
    g_print("core size obj finalize.\n");
    G_OBJECT_CLASS(core_size_parent_class)->finalize(obj);
}


// public methods
// copy
gboolean core_size_copy(CoreSize *self, CoreSize *another) {
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(another != NULL, FALSE);
    another->width = self->width;
    another->height = self->height;
    return TRUE;
}

// getters
guint32 core_size_get_width(CoreSize *self) {
    return self->width;
}

guint32 core_size_get_height(CoreSize *self) {
    return self->height;
}

guint32 core_size_get_area(CoreSize *self) {
    return self->width * self->height;
}


// setters
gboolean core_size_set_width(CoreSize *self, guint32 const width) {
    self->width = width;
    return TRUE;
}

gboolean core_size_set_height(CoreSize *self, guint32 const height) {
    self->height = height;
    return TRUE;
}

// converters
gboolean core_size_to_string(CoreSize *self, GString **string) {
    g_return_val_if_fail(self != NULL, FALSE);
    g_return_val_if_fail(string != NULL, FALSE);
    if (*string == NULL) {
        *string = g_string_new(0);
    }
    g_string_printf(*string, "[%u, %u]", self->width, self->height);
    return TRUE;
}
