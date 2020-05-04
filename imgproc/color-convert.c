//
// Created by karl_ on 2020-5-3.
//

#include <cblas.h>
#include <imgproc/color-convert.h>


gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst) {
    gsize i;
    CoreSize *size;
    gdouble* dst_data, *src_data;
    gsize channel, area;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);

    size = core_image_get_size(src);
    area = core_size_get_area(size);
    dst_data = g_malloc0(area * sizeof(gdouble));
    src_data = core_image_get_data(src);
    channel = core_image_get_channel(src);
    cblas_dcopy(area, src_data, channel, dst_data, 1);
    for (i = 1; i < channel; ++i) {
        cblas_daxpy(area, 1.0, src_data + i, channel, dst_data, 1);
    }
    if (channel > 1) {
        cblas_daxpy(area, 1.0 / channel - 1.0, dst_data, 1, dst_data, 1);
    }
    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, area, 1, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, area, 1, size, FALSE);
    }
    g_object_unref(size);
    return TRUE;
}


gboolean imgproc_to_binary_threshold(CoreImage *src, CoreImage **dst, gdouble threshold, gboolean inverse) {
    CoreImage* gray_src = NULL;
    CoreSize* size;
    gsize i, area;
    gdouble *src_data, *dst_data;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);

    if (core_image_get_channel(src) > 1) {
        imgproc_to_grayscale(src, &gray_src);
    } else {
        gray_src = g_object_ref(src);
    }
    size = core_image_get_size(gray_src);
    area = core_size_get_area(size);
    src_data = core_image_get_data(gray_src);
    dst_data = g_malloc(sizeof(gdouble) * area);
    for (i = 0; i < area; ++i) {
        dst_data[i] = ((unsigned) (src_data[i] > threshold) ^ (unsigned) inverse) * 255.0;
    }
    if (*dst == NULL) {
        core_image_new_with_data(dst_data, area, 1, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, area, 1, size, FALSE);
    }
    g_object_unref(size);
    g_object_unref(gray_src);
    return TRUE;
}

gboolean imgproc_to_binary_dither() { return FALSE; }

gboolean imgproc_to_binary_ordered_dither() { return FALSE; }