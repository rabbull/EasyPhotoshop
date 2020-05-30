//
// Created by karl on 30/5/2020.
//

#include "bmp.h"

static CoreImage *bmp_load(GString *path);

void bmp_init(FileIOInputFormatTable *table) {
    fileio_input_format_table_register(table, bmp_load);
}

static CoreImage *bmp_load(GString *path) {
    /* always return false until being implemented */
    return FALSE;
}