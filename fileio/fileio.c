//
// Created by karl on 30/5/2020.
//

#include "input-format-table.h"
#include "bmp.h"

void fileio_init() {
    FileIOInputFormatTable *table;
    table = fileio_input_format_table_get_instance();
    bmp_init(table, NULL);
    g_object_unref(table);
}

void fileio_exit() {
    FileIOInputFormatTable *table;
    table = fileio_input_format_table_get_instance();
    bmp_exit(table, NULL);
    fileio_input_format_table_release(table);
}