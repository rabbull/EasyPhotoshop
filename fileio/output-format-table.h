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

typedef gboolean (*output_method_t)(CoreImage *image, char const *string);

struct _FileIOOutputFormatTableClass {
    GObjectClass parent_class;

    gpointer pad[12];
};

FileIOOutputFormatTable *fileio_output_format_table_get_instance();

void fileio_output_format_table_release(FileIOOutputFormatTable *self);

GList* fileio_output_format_table_get_names(FileIOOutputFormatTable *self);

output_method_t fileio_output_format_table_get_output_method(FileIOOutputFormatTable *self, char const *name);

gboolean fileio_output_format_table_register(FileIOOutputFormatTable *self, char const *name, output_method_t im);

void fileio_output_format_table_unregister(FileIOOutputFormatTable *self, char const *name);

G_END_DECLS

#endif //EASYPHOTOSHOP_OUTPUT_FORMAT_TABLE_H
