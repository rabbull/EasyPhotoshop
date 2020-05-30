//
// Created by karl on 30/5/2020.
//

#ifndef EASYPHOTOSHOP_INPUT_FORMAT_TABLE_H
#define EASYPHOTOSHOP_INPUT_FORMAT_TABLE_H

#include <glib-object.h>
#include <core/image.h>

G_BEGIN_DECLS

#define FILEIO_TYPE_INPUT_FORMAT_TABLE fileio_input_format_table_get_type()

G_DECLARE_DERIVABLE_TYPE(FileIOInputFormatTable, fileio_input_format_table, FILEIO, INPUT_FORMAT_TABLE, GObject)

typedef CoreImage *(*input_method_t)(GString *string);

struct _FileIOInputFormatTableClass {
    GObjectClass parent_class;

    gpointer pad[12];
};

FileIOInputFormatTable *fileio_input_format_table_get_instance();

gsize fileio_input_format_table_get_length(FileIOInputFormatTable *self);

input_method_t fileio_input_format_table_get_input_method(FileIOInputFormatTable *self, gsize index);

void fileio_input_format_table_register(FileIOInputFormatTable *self, input_method_t im);

void fileio_input_format_table_unregister(FileIOInputFormatTable *self, gsize index);

G_END_DECLS

#endif //EASYPHOTOSHOP_INPUT_FORMAT_TABLE_H
