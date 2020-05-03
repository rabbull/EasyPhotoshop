//
// Created by karl_ on 2020-5-3.
//

#include <cblas.h>
#include <imgproc/histogram-equalization.h>

gboolean imgproc_histogram_equalization(CoreImage *src, CoreImage *dst, GError *error) {
    gdouble *src_data = NULL;
    gdouble *dst_data = NULL;
    CoreSize* size = NULL;
    guint8 channel;
    gboolean in_place = FALSE;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);
    g_return_val_if_fail(error == NULL, FALSE);
    g_return_val_if_fail(core_image_get_channel(src) == 1, FALSE);

    in_place = (src == dst);
    src = g_object_ref(src);
    src_data = core_image_get_data(src);
    if (in_place) {
        size = core_image_get_size(src);
        dst = core_image_new_empty_with_size(size, core_image_get_channel(src));
        g_object_unref(size);
    } else {
        dst = g_object_ref(dst);
    }
    dst_data = core_image_get_data(dst);

    g_object_unref(dst);
    g_object_unref(src);
    return TRUE;
}
