//
// Created by karl on 30/5/2020.
//

#include <stdint.h>
#include <stdio.h>

#include "bmp.h"

typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

typedef struct __attribute__((__packed__)) {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
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

static CoreImage *bmp_load(const char *path);

static void do_bmp_load(char const *path, void **buffer, CoreSize **size);

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

static CoreImage *bmp_load(const char * const path) {
    CoreImage *image = NULL;
    void *buffer = NULL;
    CoreSize *size = core_size_new();
    do_bmp_load(path, &buffer, &size);
    image = core_image_new_with_data(buffer, CORE_COLOR_SPACE_RGB, CORE_PIXEL_U3, size, TRUE);
    return image;
}

static void do_bmp_load(char const *const path, void **buffer, CoreSize **size) {

    FILE *file;
    BITMAPFILEHEADER bmp_file_header;
    BITMAPINFOHEADER bmp_info;
    RGBQUAD *rgbquad = NULL;
    file = fopen(path, "rb");
    if (file == NULL) {
        printf("failed to open");
    }

    fread(&bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, file);
    int width = bmp_info.biWidth;
    int height = bmp_info.biHeight;
    if (width < 0) { width = -width; }
    if (height < 0) { height = -height; }

    core_size_set_height(*size, height);
    core_size_set_width(*size, width);

    if (bmp_info.biBitCount <= 8) {
        rgbquad = malloc((1u << bmp_info.biBitCount) * sizeof(RGBQUAD));
        fread(rgbquad, (1u << bmp_info.biBitCount) * sizeof(RGBQUAD), 1, file);
    }

    gsize line_size = width * bmp_info.biBitCount / 8;
    if (line_size % 4) {
        line_size += 4 - line_size % 4;
    }
    guint8 *line = malloc(line_size);
    guint8 *dst_data = g_malloc(height * width * sizeof(guint8) * 3);
    switch (bmp_info.biBitCount) {
        case 8:
            for (int i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (int j = 0; j < width; ++j) {
                    dst_data[i * width * 3 + j * 3 + 0] = rgbquad[(int) line[j]].rgbRed;
                    dst_data[i * width * 3 + j * 3 + 1] = rgbquad[(int) line[j]].rgbGreen;
                    dst_data[i * width * 3 + j * 3 + 2] = rgbquad[(int) line[j]].rgbBlue;
                }
            }
            break;

        case 24:
            for (int i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (int j = 0; j < width; ++j) {
                    dst_data[i * width * 3 + j * 3 + 0] = line[j * 3 + 2];
                    dst_data[i * width * 3 + j * 3 + 1] = line[j * 3 + 1];
                    dst_data[i * width * 3 + j * 3 + 2] = line[j * 3 + 0];
                }
            }
            break;

        case 32:
            for (int i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (int j = 0; j < width; ++j) {
                    dst_data[i * width * 3 + j * 3 + 0] = line[j * 4 + 2];
                    dst_data[i * width * 3 + j * 3 + 1] = line[j * 4 + 1];
                    dst_data[i * width * 3 + j * 3 + 2] = line[j * 4 + 0];
                }
            }
            break;
    }

    /* reverse if needed */
    if (bmp_info.biHeight > 0) {
        gsize line_width = width * 3;
        for (int i = 0; i < height / 2; ++i) {
            for (int j = 0; j < line_width; ++j) {
                guint8 tmp = dst_data[i * line_width + j];
                dst_data[i * line_width + j] = dst_data[(height - i - 1) * line_width + j];
                dst_data[(height - i - 1) * line_width + j] = tmp;
            }
        }
    }
    if (bmp_info.biWidth < 0) {
        for (int i = 0; i < height / 2; ++i) {
            for (int j = 0; j < width; ++j) {
                for (int k = 0; k < 3; ++k) {
                    guint8 tmp = dst_data[i * width * 3 + j * 3 + k];
                    dst_data[i * width * 3 + j * 3 + k] = dst_data[i * width * 3 + (width - j - 1) * 3 + k];
                    dst_data[i * width * 3 + (width - j - 1) * 3 + k] = tmp;
                }
            }
        }
    }

    *buffer = dst_data;
    free(rgbquad);
    free(line);
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
