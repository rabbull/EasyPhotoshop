//
// Created by karl on 30/5/2020.
//

#include "input-format-table.h"

typedef struct {
    GList *table;
} FileIOInputFormatTablePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(FileIOInputFormatTable, fileio_input_format_table, G_TYPE_OBJECT)

static void fileio_input_format_table_dispose(GObject *obj) {
    G_OBJECT_CLASS(fileio_input_format_table_parent_class)->dispose(obj);
}

static void fileio_input_format_table_finalize(GObject *obj) {
    G_OBJECT_CLASS(fileio_input_format_table_parent_class)->finalize(obj);
}

static void fileio_input_format_table_init(FileIOInputFormatTable *table) {}

static void fileio_input_format_table_class_init(FileIOInputFormatTableClass *class) {
    G_OBJECT_CLASS(class)->dispose = fileio_input_format_table_dispose;
    G_OBJECT_CLASS(class)->finalize = fileio_input_format_table_finalize;
}

FileIOInputFormatTable *fileio_input_format_table_get_instance() {
    static FileIOInputFormatTable *instance = NULL;
    if (instance == NULL) {
        instance = g_object_new(FILEIO_TYPE_INPUT_FORMAT_TABLE, NULL);
    }

    /* add ref count before return to avoid user accidentally release the instance */
    return g_object_ref(instance);
}

/* NOTE: This method release the instance anyway. Used only when the whole program is exiting. */
void fileio_input_format_table_release(FileIOInputFormatTable *self) {
    g_object_unref(self);
    g_object_unref(self);
}

gsize fileio_input_format_table_get_length(FileIOInputFormatTable *self) {
    FileIOInputFormatTablePrivate *private = fileio_input_format_table_get_instance_private(self);
    return g_list_length(private->table);
}

input_method_t fileio_input_format_table_get_input_method(FileIOInputFormatTable *self, gsize index) {
    FileIOInputFormatTablePrivate *private = fileio_input_format_table_get_instance_private(self);
    return g_list_nth_data(private->table, index);
}

void fileio_input_format_table_register(FileIOInputFormatTable *self, input_method_t im) {
    FileIOInputFormatTablePrivate *private = fileio_input_format_table_get_instance_private(self);
    private->table = g_list_append(private->table, im);
}

void fileio_input_format_table_unregister(FileIOInputFormatTable *self, input_method_t im) {
    FileIOInputFormatTablePrivate *private = fileio_input_format_table_get_instance_private(self);
    private->table = g_list_remove(private->table, im);
}
