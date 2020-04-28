//
// Created by karl on 4/28/20.
//

#include "size.h"


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
gboolean core_size_copy(CoreSize *self, CoreSize **another, GError **error) {
    if (another == NULL) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "another is NULL.");
        return FALSE;
    }
    if (*another == NULL) {
        *another = core_size_new();
    }
    (*another)->width = self->width;
    (*another)->height = self->height;
    return TRUE;
}

// getters
guint32 core_size_get_width(CoreSize *self, GError **error) {
    return self->width;
}

guint32 core_size_get_height(CoreSize *self, GError **error) {
    return self->height;
}

// setters
gboolean core_size_set_width(CoreSize *self, guint32 const width, GError **error) {
    self->width = width;
    return TRUE;
}

gboolean core_size_set_height(CoreSize *self, guint32 const height, GError **error) {
    self->height = height;
    return TRUE;
}

// converters
gboolean core_size_to_string(CoreSize *self, GString **string, GError **error) {
    if (string == NULL) {
        g_set_error(error, G_OPTION_ERROR, G_OPTION_ERROR_BAD_VALUE, "Pointer to string is NULL.");
        return FALSE;
    }
    if (*string == NULL) {
        *string = g_string_new(0);
    }
    g_string_printf(*string, "[%u, %u]", self->width, self->height);
    return TRUE;
}
