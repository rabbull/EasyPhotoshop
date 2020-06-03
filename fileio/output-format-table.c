//
// Created by karl on 3/6/2020.
//

//
// Created by karl on 30/5/2020.
//

#include "output-format-table.h"

typedef struct {
    GList *table;
} FileIOOutputFormatTablePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(FileIOOutputFormatTable, fileio_output_format_table, G_TYPE_OBJECT)

static void fileio_output_format_table_dispose(GObject *obj) {
    G_OBJECT_CLASS(fileio_output_format_table_parent_class)->dispose(obj);
}

static void fileio_output_format_table_finalize(GObject *obj) {
    G_OBJECT_CLASS(fileio_output_format_table_parent_class)->finalize(obj);
}

static void fileio_output_format_table_init(FileIOOutputFormatTable *table) {}

static void fileio_output_format_table_class_init(FileIOOutputFormatTableClass *class) {
    G_OBJECT_CLASS(class)->dispose = fileio_output_format_table_dispose;
    G_OBJECT_CLASS(class)->finalize = fileio_output_format_table_finalize;
}

FileIOOutputFormatTable *fileio_output_format_table_get_instance() {
    static FileIOOutputFormatTable *instance = NULL;
    if (instance == NULL) {
        instance = g_object_new(FILEIO_TYPE_OUTPUT_FORMAT_TABLE, NULL);
    }

    /* add ref count before return to avoid user accidentally release the instance */
    return g_object_ref(instance);
}

/* NOTE: This method release the instance anyway. Used only when the whole program is exiting. */
void fileio_output_format_table_release(FileIOOutputFormatTable *self) {
    g_object_unref(self);
    g_object_unref(self);
}

gsize fileio_output_format_table_get_length(FileIOOutputFormatTable *self) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    return g_list_length(private->table);
}

output_method_t fileio_output_format_table_get_output_method(FileIOOutputFormatTable *self, gsize index) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    return g_object_ref(g_list_nth_data(private->table, index));
}

void fileio_output_format_table_register(FileIOOutputFormatTable *self, output_method_t im) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    private->table = g_list_append(private->table, im);
}

void fileio_output_format_table_unregister(FileIOOutputFormatTable *self, output_method_t im) {
    FileIOOutputFormatTablePrivate *private = fileio_output_format_table_get_instance_private(self);
    private->table = g_list_remove(private->table, im);
}
