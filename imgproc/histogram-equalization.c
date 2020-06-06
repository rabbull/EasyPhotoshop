//
// Created by karl_ on 2020-5-3.
//

#include <stdlib.h>
#include <math.h>
#include <cblas.h>
#include <core/color-space.h>
#include <imgproc/histogram-equalization.h>
#include "color-convert.h"


// ref: https://www.math.uci.edu/icamp/courses/math77c/demos/hist_eq.pdf
static void
histeq(double const *src, unsigned src_stride, double *dst, unsigned dst_stride, size_t area, unsigned range) {
    unsigned *cdf;
    size_t i;
    cdf = calloc(range, sizeof(unsigned));
    for (i = 0; i < area; ++i) {
        cdf[(int) (src[i * src_stride] * range)] += 1;
    }
    for (i = 1; i < range; ++i) {
        cdf[i] += cdf[i - 1];
    }
    for (i = 0; i < area; ++i) {
        dst[i * dst_stride] = floor((double) (range - 1) * cdf[(int) (src[i * src_stride] * range)] / area) / range;
    }
    free(cdf);
}

static gboolean histeq_grayscale(CoreImage *src, CoreImage **dst) {
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
    histeq(src_data_cast, 1, dst_data_cast, 1, area, 256);

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


//static gboolean histeq_hsl(CoreImage *src, CoreImage **dst) {
//    gdouble *src_data, *dst_data;
//    CoreSize *size;
//    gsize area, block_size;
//
//    src_data = core_image_get_data(src);
//    size = core_image_get_size(src);
//    area = core_size_get_area(size);
//    block_size = area * 3 * sizeof(gdouble);
//    dst_data = g_malloc(block_size);
//    memcpy(src_data, dst_data, block_size);
//    histeq(src_data + 2, 3, dst_data + 2, 3, area, 255);
//
//    if (*dst == NULL) {
//        *dst = core_image_new_with_data(dst_data, CORE_COLOR_SPACE_HSL, CORE_PIXEL_D3, size, FALSE);
//    } else {
//        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_HSL, CORE_PIXEL_D3, size, FALSE);
//    }
//
//    g_object_unref(size);
//}

static gboolean histeq_hsl(CoreImage *src, CoreImage **dst) {
    if (*dst == NULL) {
        *dst = core_image_new_clone(src);
    } else {
        core_image_copy(src, *dst);
    }
}

gboolean imgproc_histogram_equalization(CoreImage *src, CoreImage **dst) {
    CoreColorSpace src_color_space = core_image_get_color_space(src);
    CoreImage *dummy = NULL;
    gboolean ret;

    if (src_color_space == CORE_COLOR_SPACE_GRAY_SCALE) {
        ret = histeq_grayscale(src, dst);
    } else {
        if (src_color_space == CORE_COLOR_SPACE_RGB) {
            imgproc_to_HSL(src, &dummy);
        } else {
            dummy = g_object_ref(src);
        }
        ret = histeq_hsl(dummy, dst);
        g_object_unref(dummy);
        if (src_color_space == CORE_COLOR_SPACE_RGB) {
            imgproc_to_RGB(*dst, dst);
        }
    }
    return ret;
}
