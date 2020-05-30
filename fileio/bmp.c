//
// Created by karl on 30/5/2020.
//

#include "bmp.h"

static CoreImage *bmp_load(GString *path);

static void do_bmp_load(char const *path, void **buffer);

void bmp_init(FileIOInputFormatTable *table) {
    fileio_input_format_table_register(table, bmp_load);
}

static CoreImage *bmp_load(GString *path) {
    CoreImage *image = NULL;
    void *buffer = NULL;
    do_bmp_load(path->str, &buffer);

    /* TODO: convert buffer into CoreImage */
    /* always return NULL until being implemented */
    return image;
}

static void do_bmp_load(char const *const path, void **buffer) {
    /* TODO: implement it */
}
