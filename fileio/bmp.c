//
// Created by karl on 30/5/2020.
//

#include "bmp.h"

static CoreImage *bmp_load(GString *path);

static void do_bmp_load(char const *path, void **buffer);

static void bmp_save(CoreImage *image, GString *path);

static void do_bmp_save(char const *path, void *buffer);

void bmp_init(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_register(input_table, bmp_load);
    fileio_output_format_table_register(output_table, bmp_save);
}

void bmp_exit(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_unregister(input_table, bmp_load);
    fileio_output_format_table_unregister(output_table, bmp_save);
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

static void bmp_save(CoreImage *image, GString *path) {
    /* TODO: add necessary information to interface */
    do_bmp_save(path->str, core_image_get_data(image));
}

static void do_bmp_save(char const *path, void *buffer) {
    /* TODO: implement it */
}