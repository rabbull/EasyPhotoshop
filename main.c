#include <stdio.h>
#include <stdlib.h>
#include "core/size.h"


int main(int argc, char **argv) {
    CoreSize *size = NULL;
    GString *string = NULL;
    GError *error = NULL;

    size = g_object_new(CORE_TYPE_SIZE, NULL);
    core_size_set_width(size, 10, &error);
    core_size_set_height(size, 20, &error);

    core_size_to_string(size, &string, &error);
    g_print("%s\n", string->str);

    g_object_unref(string);
    g_object_unref(size);
    return EXIT_SUCCESS;
}
