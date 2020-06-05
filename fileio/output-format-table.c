//
// Created by karl on 3/6/2020.
//

//
// Created by karl on 30/5/2020.
//

#include "output-format-table.h"

typedef struct {
    GHashTable *table;
} FileIOOutputFormatTablePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(FileIOOutputFormatTable, fileio_output_format_table, G_TYPE_OBJECT)

static void fileio_output_format_table_dispose(GObject *obj) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(
            FILEIO_OUTPUT_FORMAT_TABLE(obj));
    g_clear_object(&private->table);
    G_OBJECT_CLASS(fileio_output_format_table_parent_class)->dispose(obj);
}

static void fileio_output_format_table_finalize(GObject *obj) {
    G_OBJECT_CLASS(fileio_output_format_table_parent_class)->finalize(obj);
}

static void fileio_output_format_table_init(FileIOOutputFormatTable *table) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(table);
    private->table = g_hash_table_new(g_str_hash, g_str_equal);
}

static void fileio_output_format_table_class_init(FileIOOutputFormatTableClass *class) {
    G_OBJECT_CLASS(class)->dispose = fileio_output_format_table_dispose;
    G_OBJECT_CLASS(class)->finalize = fileio_output_format_table_finalize;
}

FileIOOutputFormatTable *fileio_output_format_table_get_instance() {
    static FileIOOutputFormatTable *instance = NULL;
    if (instance == NULL) {
        instance = g_object_new(FILEIO_TYPE_OUTPUT_FORMAT_TABLE, NULL);
    }
    return g_object_ref(instance);
}

/* NOTE: This method release the instance anyway. Used only when the whole program is exiting. */
void fileio_output_format_table_release(FileIOOutputFormatTable *self) {
    g_object_unref(self);
}

/* NOTE: use g_list_free() to release the returned list when done using it */
GList *fileio_output_format_table_get_names(FileIOOutputFormatTable *self) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    return g_hash_table_get_keys(private->table);
}

output_method_t fileio_output_format_table_get_output_method(FileIOOutputFormatTable *self, char const *name) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    return g_hash_table_lookup(private->table, name);
}

gboolean fileio_output_format_table_register(FileIOOutputFormatTable *self, char const *name, output_method_t im) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    if (!g_hash_table_lookup(private->table, name)) {
        g_hash_table_insert(private->table, (gpointer) name, im);
        return TRUE;
    } else {
        return FALSE;
    }
}

void fileio_output_format_table_unregister(FileIOOutputFormatTable *self, char const *name) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    g_hash_table_remove(private->table, name);
}
