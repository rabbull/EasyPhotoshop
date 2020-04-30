#include <stdio.h>
#include <stdlib.h>
#include "core/image.h"

#define print_bool(expr) printf((expr) ? "true\n" : "false")

int main(int argc, char **argv) {
    CoreImage *img = 0;
    CoreSize *size = 0;
    guint8 pix[3] = {1, 2, 3};
    guint8 bpp = 24;
    GString *str = 0;
    gsize i, j, k;

    size = core_size_new_with_value(5, 4);
    img = core_image_new_fill_with_color(size, bpp, pix);
    g_object_unref(size);
    size = NULL;

    core_image_get_size(img, &size);
    core_size_to_string(size, &str);
    printf("%s\n", str->str);
    for (i = 0; i < core_size_get_area(size); ++i) {
        for (j = 0; j < (bpp >> 3u); ++j) {
            printf("%d, ", (gint32) core_image_get_data(img)[i * (bpp >> 3u) + j]);
        }
    }
    printf("\n");
    for (i = 0; i < core_size_get_height(size); ++i) {
        for (j = 0; j < core_size_get_width(size); ++j) {
            printf("[");
            for (k = 0; k < core_image_get_byte_per_pixel(img); ++k) {
                if (k != 0) printf(" ");
                printf("%d", (gint32) core_image_get_pixel(img, j, i)[k]);
            }
            printf("], ");
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}
