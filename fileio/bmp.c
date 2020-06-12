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

static void do_bmp_load(char const *path, void **buffer, unsigned *h, unsigned *w);

static gboolean bmp_save(CoreImage *image, char const *path);

static gboolean do_bmp_save(char const *path, uint8_t const *data, size_t height, size_t width);

void bmp_init(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_register(input_table, bmp_load);
    fileio_output_format_table_register(output_table, "BMP", bmp_save);
}

void bmp_exit(FileIOInputFormatTable *input_table, FileIOOutputFormatTable *output_table) {
    fileio_input_format_table_unregister(input_table, bmp_load);
    fileio_output_format_table_unregister(output_table, "BMP");
}

static CoreImage *bmp_load(const char *const path) {
    CoreImage *image = NULL;
    void *buffer = NULL;
    CoreSize *size;
    unsigned h, w;
    do_bmp_load(path, &buffer, &h, &w);
    size = core_size_new_with_value(h, w);
    image = core_image_new_with_data(buffer, CORE_COLOR_SPACE_RGB, CORE_PIXEL_U3, size, TRUE);
    return image;
}

static void do_bmp_load(char const *const path, void **buffer, unsigned *h, unsigned *w) {
    FILE *file;
    BITMAPFILEHEADER bmp_file_header;
    BITMAPINFOHEADER bmp_info;
    RGBQUAD *rgbquad = NULL;
    gsize height, width;
    guint8 *line;
    guint8 *dst_data;
    gsize i, j, k;
    guint8 tmp;

    file = fopen(path, "rb");
    if (file == NULL) {
        printf("failed to open");
    }

    fread(&bmp_file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&bmp_info, sizeof(BITMAPINFOHEADER), 1, file);
    height = abs(bmp_info.biHeight);
    width = abs(bmp_info.biWidth);
    *h = height;
    *w = width;

    if (bmp_info.biBitCount <= 8) {
        rgbquad = malloc((1u << bmp_info.biBitCount) * sizeof(RGBQUAD));
        fread(rgbquad, (1u << bmp_info.biBitCount) * sizeof(RGBQUAD), 1, file);
    }

    gsize line_size = width * bmp_info.biBitCount / 8;
    if (line_size % 4) {
        line_size += 4 - line_size % 4;
    }
    line = malloc(line_size);
    dst_data = g_malloc(height * width * sizeof(guint8) * 3);
    switch (bmp_info.biBitCount) {
        case 8:
            for (i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (j = 0; j < width; ++j) {
                    dst_data[i * width * 3 + j * 3 + 0] = rgbquad[(int) line[j]].rgbRed;
                    dst_data[i * width * 3 + j * 3 + 1] = rgbquad[(int) line[j]].rgbGreen;
                    dst_data[i * width * 3 + j * 3 + 2] = rgbquad[(int) line[j]].rgbBlue;
                }
            }
            break;

        case 24:
            for (i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (j = 0; j < width; ++j) {
                    dst_data[i * width * 3 + j * 3 + 0] = line[j * 3 + 2];
                    dst_data[i * width * 3 + j * 3 + 1] = line[j * 3 + 1];
                    dst_data[i * width * 3 + j * 3 + 2] = line[j * 3 + 0];
                }
            }
            break;

        case 32:
            for (i = 0; i < height; ++i) {
                fread(line, 1, line_size, file);
                for (j = 0; j < width; ++j) {
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
        for (i = 0; i < height / 2; ++i) {
            for (j = 0; j < line_width; ++j) {
                tmp = dst_data[i * line_width + j];
                dst_data[i * line_width + j] = dst_data[(height - i - 1) * line_width + j];
                dst_data[(height - i - 1) * line_width + j] = tmp;
            }
        }
    }
    if (bmp_info.biWidth < 0) {
        for (i = 0; i < height / 2; ++i) {
            for (j = 0; j < width; ++j) {
                for (k = 0; k < 3; ++k) {
                    tmp = dst_data[i * width * 3 + j * 3 + k];
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

static gboolean bmp_save(CoreImage *image, char const *path) {
    /* TODO: add necessary information to interface */
    gboolean result;
    gpointer image_data;
    CoreSize *size;
    size_t height, width;

    image_data = core_image_get_data(image);
    size = core_image_get_size(image);
    height = core_size_get_height(size);
    width = core_size_get_width(size);
    result = do_bmp_save(path, image_data, height, width);

    g_object_unref(size);
    return result;
}

/* NOTE: return whether succeeded */
static gboolean do_bmp_save(char const *path, uint8_t const *data, size_t height, size_t width) {
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    FILE *file;
    uint8_t *output;
    size_t bfsize = 0;
    size_t offset = 0;
    bfsize = height * ((3 * width + 3) / 4 * 4) + 54;
    output = malloc(bfsize);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            output[i * width * 3 + j * 3 + 0 + 54 + offset] = data[(height - i - 1) * width * 3 + j * 3 + 2];
            output[i * width * 3 + j * 3 + 1 + 54 + offset] = data[(height - i - 1) * width * 3 + j * 3 + 1];
            output[i * width * 3 + j * 3 + 2 + 54 + offset] = data[(height - i - 1) * width * 3 + j * 3 + 0];
            if (j == width - 1 && (3 * width) % 4 != 0) {
                int new_offset = (3 * width + 3) / 4 * 4 - 3 * width;
                if (new_offset > 0) output[i * width * 3 + j * 3 + 54 + offset + 1] = (uint8_t) 0;
                if (new_offset > 1) output[i * width * 3 + j * 3 + 54 + offset + 2] = (uint8_t) 0;
                if (new_offset > 2) output[i * width * 3 + j * 3 + 54 + offset + 3] = (uint8_t) 0;

                offset += new_offset;
            }
            if (i == height - 1) bfsize = i * width * 3 + width * 3 + 54 + offset;
        }
        file_header.bfType = 0x4D42;
        file_header.bfReserved1 = 0;
        file_header.bfReserved2 = 0;
        file_header.bfSize = bfsize;

        file_header.bfOffBits = 54;
        info_header.biSize = 40;
        info_header.biWidth = width;
        info_header.biHeight = height;
        info_header.biPlanes = (uint8_t) 1;
        info_header.biBitCount = (uint8_t) 24;
        info_header.biCompression = 0;
        info_header.biSizeImage = bfsize * 8;
        info_header.biXpelsPerMeter = 0;
        info_header.biYpelsPerMeter = 0;
        info_header.biClrUsed = 16777216;
        info_header.biClrImportant = 0;

    }
    memcpy((void *) output, &file_header, sizeof(BITMAPFILEHEADER));
    memcpy((void *) output + 14, &info_header, sizeof(BITMAPINFOHEADER));
    file = fopen(path, "wb");
    fwrite(output, bfsize, 1L, file);
    free(output);
    fclose(file);
    return TRUE;
}