#include <stdio.h>
#include <stdlib.h>
#include "core/image.h"

#define print_bool(expr) printf((expr) ? "true\n" : "false")

int main(int argc, char **argv) {
    CoreImage *img = 0;
    CoreSize *sz1 = 0, *sz2 = 0;
    GString *str = 0;

    img = g_object_new(CORE_TYPE_IMAGE, NULL);
    print_bool(core_image_get_size(img, &sz1, NULL));
    core_size_copy(sz1, &sz2, NULL);
    core_size_set_width(sz2, 100, NULL);
    core_size_set_height(sz2, 200, NULL);
    str = g_string_new("");
    print_bool(core_size_to_string(sz1, &str, NULL));
    g_print("1: %s\n", str->str);
    print_bool(core_size_to_string(sz2, &str, NULL));
    g_print("2: %s\n", str->str);

    core_image_set_size(img, sz2, NULL);
    sz1 = NULL;
    core_image_get_size(img, &sz1, NULL);
    print_bool(core_size_to_string(sz1, &str, NULL));
    g_print("11: %s\n", str->str);

    g_object_unref(img);
    g_object_unref(sz1);
    g_object_unref(sz2);
    g_object_unref(str);
    return EXIT_SUCCESS;
}
