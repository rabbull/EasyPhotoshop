//
// Created by karl_ on 2020-5-3.
//

#include <stdlib.h>
#include <math.h>
#include <cblas.h>
#include <core/color-space.h>
#include <imgproc/histogram-equalization.h>


// ref: https://www.math.uci.edu/icamp/courses/math77c/demos/hist_eq.pdf
static void histeq(double const *src, double *dst, size_t area, unsigned range) {
    unsigned *cdf;
    size_t i;
    cdf = calloc(range, sizeof(unsigned));
    for (i = 0; i < area; ++i) {
        cdf[(int) (src[i] * range)] += 1;
    }
    for (i = 1; i < range; ++i) {
        cdf[i] += cdf[i - 1];
    }
    for (i = 0; i < area; ++i) {
        dst[i] = floor((double) (range - 1) * cdf[(int) (src[i] * range)] / area) / range;
    }
    free(cdf);
}


gboolean imgproc_histogram_equalization(CoreImage *src, CoreImage **dst) {
    gdouble *src_data_cast = NULL;
    gdouble *dst_data_cast = NULL;
    gpointer src_data, dst_data;
    CoreSize *size = NULL;
    guint8 channel;
    gsize i, area;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    gdouble range, inv_range;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);

    /* TODO: impl. for multiple channels */
    channel = core_image_get_channel(src);
    g_return_val_if_fail(channel == 1, FALSE);

    src = g_object_ref(src);
    src_data = core_image_get_data(src);
    size = core_image_get_size(src);
    area = core_size_get_area(size);
    color_space = core_image_get_color_space(src);
    pixel_type = core_image_get_pixel_type(src);
    range = core_pixel_get_range(pixel_type);
    inv_range = 1.0 / range;

    /* cast to double and normalize pixel data to [0, 1] */
    if (core_pixel_is_double(pixel_type)) {
        src_data_cast = src_data;
    } else if (core_pixel_is_uint8(pixel_type)) {
        src_data_cast = g_malloc(sizeof(gdouble) * area);
        for (i = 0; i < area; ++i) {
            src_data_cast[i] = ((guint8 *) src_data)[i] * inv_range;
        }
    } else {
        g_return_val_if_fail(FALSE, FALSE);
    }
    dst_data_cast = g_malloc(sizeof(gdouble) * area);

    /* run the algorithm */
    histeq(src_data_cast, dst_data_cast, area, 256);

    /* cast back to uchar */
    if (core_pixel_is_double(pixel_type)) {
        /* exactly do nothing */
    } else if (core_pixel_is_uint8(pixel_type)) {
        /* reuse allocated memory */
        for (i = 0; i < area; ++i) {
            ((guint8 *) dst_data_cast)[i] = dst_data_cast[i] * 255.0;
        }
    } else {
        g_return_val_if_fail(FALSE, FALSE);
    }
    dst_data = dst_data_cast;
    if (core_pixel_is_uint8(pixel_type)) {
        /* release useless memory, can be removed for speed */
        dst_data = g_realloc(dst_data, sizeof(guint8) * area);
    }

    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, color_space, pixel_type, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, color_space, pixel_type, size, FALSE);
    }

    /* free allocated additional memory */
    if (core_pixel_is_uint8(pixel_type)) {
        g_free(src_data_cast);
    }
    g_object_unref(size);
    g_object_unref(src);
    return TRUE;
}
