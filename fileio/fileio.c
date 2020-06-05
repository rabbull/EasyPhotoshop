//
// Created by karl on 30/5/2020.
//

#include "input-format-table.h"
#include "bmp.h"

void fileio_init() {
    FileIOInputFormatTable *input_table;
    FileIOOutputFormatTable *output_table;
    input_table = fileio_input_format_table_get_instance();
    output_table = fileio_output_format_table_get_instance();
    bmp_init(input_table, output_table);
    g_object_unref(input_table);
    g_object_unref(output_table);
}

void fileio_exit() {
    FileIOInputFormatTable *table;
    FileIOOutputFormatTable *output_table;
    table = fileio_input_format_table_get_instance();
    output_table = fileio_output_format_table_get_instance();
    bmp_exit(table, output_table);
    fileio_input_format_table_release(table);
    fileio_output_format_table_release(output_table);
}