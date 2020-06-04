//
// Created by karl on 30/5/2020.
//

#include <stdint.h>
#include <stdio.h>

#include "bmp.h"

typedef uint16_t WORD, BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct __attribute__((__packed__)) {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    DWORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct __attribute__((__packed__)) {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXpelsPerMeter;
    LONG biYpelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct __attribute__((__packed__)) {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

static CoreImage *bmp_load(GString *path);

static void do_bmp_load(char const *path, void **buffer);

static gboolean bmp_save(CoreImage *image, GString *path);

static gboolean do_bmp_save(char const *path, void *buffer);

void bmp_init(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_register(input_table, bmp_load);
    fileio_output_format_table_register(output_table, "BMP", bmp_save);
}

void bmp_exit(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_unregister(input_table, bmp_load);
    fileio_output_format_table_unregister(output_table, "BMP");
}

static CoreImage *bmp_load(GString *path) {
    CoreImage *image = NULL;
    void *buffer = NULL;
    do_bmp_load(path->str, &buffer);

    /* TODO: convert buffer into CoreImage */
    image = core_image_new_with_data(buffer, CORE_COLOR_SPACE_RGB, CORE_PIXEL_U3, NULL, TRUE);

    /* always return NULL until being implemented */
    return image;
}

static void do_bmp_load(char const *const path, void **buffer) {
    FILE *file;
    BITMAPINFOHEADER bmp_file_header;
    BITMAPINFOHEADER bmp_info;

    file = fopen(path, "rb");
    fread(&bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, file);

    /* TODO: further implementation */
}

static gboolean bmp_save(CoreImage *image, GString *path) {
    /* TODO: add necessary information to interface */
    return do_bmp_save(path->str, core_image_get_data(image));
}

/* NOTE: return whether succeeded */
static gboolean do_bmp_save(char const *path, void *buffer) {
    /* TODO: implement it */
    return FALSE;
}
