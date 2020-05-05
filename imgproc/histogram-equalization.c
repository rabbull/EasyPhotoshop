//
// Created by karl_ on 2020-5-3.
//

#include <stdlib.h>
#include <math.h>
#include <cblas.h>
#include <imgproc/histogram-equalization.h>


// ref: https://www.math.uci.edu/icamp/courses/math77c/demos/hist_eq.pdf
static void histeq(double const *src, double *dst, size_t area, unsigned int range) {
    unsigned int *cdf;
    size_t i;
    cdf = calloc(range, sizeof(unsigned int));
    for (i = 0; i < area; ++i) {
        cdf[(int) src[i]] += 1;
    }
    for (i = 1; i < range; ++i) {
        cdf[i] += cdf[i - 1];
    }
    for (i = 0; i < area; ++i) {
        dst[i] = floor((double) (range - 1) * cdf[(int) src[i]] / area);
    }
    free(cdf);
}


gboolean imgproc_histogram_equalization(CoreImage *src, CoreImage *dst) {
    gdouble *src_data = NULL;
    gdouble *dst_data = NULL;
    CoreSize *size = NULL;
    guint8 channel;
    gboolean in_place = FALSE;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);
    g_return_val_if_fail(core_image_get_channel(src) == 1, FALSE);

    in_place = (src == dst);
    src = g_object_ref(src);
    src_data = core_image_get_data(src);
    size = core_image_get_size(src);
    if (in_place) {
        dst = core_image_new_empty_with_size(size, core_image_get_channel(src));
        g_object_unref(size);
    } else {
        dst = g_object_ref(dst);
    }
    dst_data = core_image_get_data(dst);

    histeq(src_data, dst_data, core_size_get_area(size), 256);
    if (in_place) {
        core_image_copy(dst, &src);
    }

    g_object_unref(dst);
    g_object_unref(src);
    return TRUE;
}
