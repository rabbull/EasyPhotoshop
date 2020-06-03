//
// Created by karl on 3/6/2020.
//

#ifndef EASYPHOTOSHOP_OUTPUT_FORMAT_TABLE_H
#define EASYPHOTOSHOP_OUTPUT_FORMAT_TABLE_H
#include <glib-object.h>
#include <core/image.h>

G_BEGIN_DECLS

#define FILEIO_TYPE_OUTPUT_FORMAT_TABLE fileio_output_format_table_get_type()

G_DECLARE_DERIVABLE_TYPE(FileIOOutputFormatTable, fileio_output_format_table, FILEIO, OUTPUT_FORMAT_TABLE, GObject)

typedef void (*output_method_t)(CoreImage *image, GString *string);

struct _FileIOOutputFormatTableClass {
    GObjectClass parent_class;

    gpointer pad[12];
};

FileIOOutputFormatTable *fileio_output_format_table_get_instance();

void fileio_output_format_table_release(FileIOOutputFormatTable *self);

gsize fileio_output_format_table_get_length(FileIOOutputFormatTable *self);

output_method_t fileio_output_format_table_get_output_method(FileIOOutputFormatTable *self, gsize index);

void fileio_output_format_table_register(FileIOOutputFormatTable *self, output_method_t im);

void fileio_output_format_table_unregister(FileIOOutputFormatTable *self, output_method_t im);

G_END_DECLS

#endif //EASYPHOTOSHOP_OUTPUT_FORMAT_TABLE_H
