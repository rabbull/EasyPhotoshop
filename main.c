#include <stdio.h>
#include <stdlib.h>
#include "core/image.h"

#define print_bool(expr) printf((expr) ? "true\n" : "false")

int main(int argc, char **argv) {
    CoreImage *img = 0;
    CoreSize *size = 0;
    guint8 data[3*8*5*8] = {0};
    guint8 *data_got;
    GString *str = 0;
    gsize i, j;

    for (i = 0; i < 3*8; ++i) {
        for (j = 0; j < 5*8; ++j) {
            data[i * 5*8 + j] = i * j;
        }
    }
    size = core_size_new_with_value(3, 5);
    img = core_image_new_with_data(data, 3*8*5*8, 8, size, TRUE);
    g_object_unref(size);
    size = NULL;

    data_got = core_image_get_data(img);
    core_image_get_size(img, &size);
    core_size_to_string(size, &str);
    printf("%s\n", str->str);
    for (i = 0; i < 3*8; ++i) {
        for (j = 0; j < 5*8; ++j) {
            printf("%u, ", (guint32) data_got[i * 5*8 + j]);
        }
        printf("\n");
    }
    return EXIT_SUCCESS;
}
