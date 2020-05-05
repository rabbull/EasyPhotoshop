//
// Created by karl_ on 2020-5-3.
//

#include <cblas.h>
#include <imgproc/color-convert.h>


gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst) {
    gsize i;
    CoreSize *size;
    gdouble *dst_data_cast, *src_data_cast;
    gpointer src_data, dst_data;
    gsize channel, area;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);

    size = core_image_get_size(src);
    area = core_size_get_area(size);
    color_space = core_image_get_color_space(src);
    pixel_type = core_image_get_pixel_type(src);
    channel = core_image_get_channel(src);
    src_data = core_image_get_data(src);

    if (core_pixel_is_uint8(pixel_type)) {
        src_data_cast = g_malloc(area * sizeof(gdouble) * channel);
        for (i = 0; i < area * channel; ++i) {
            src_data_cast[i] = (gdouble) ((guint8 *) src_data)[i];
        }
    } else {
        /* do not need to cast if pixel type is Dx */
        src_data_cast = src_data;
    }
    dst_data_cast = g_malloc(area * sizeof(gdouble));

    cblas_dcopy(area, src_data_cast, channel, dst_data_cast, 1);
    for (i = 1; i < channel; ++i) {
        cblas_daxpy(area, 1.0, src_data_cast + i, channel, dst_data_cast, 1);
    }
    if (channel > 1) {
        cblas_daxpy(area, 1.0 / channel - 1.0, dst_data_cast, 1, dst_data_cast, 1);
    }

    if (core_pixel_is_uint8(pixel_type)) {
        pixel_type = CORE_PIXEL_U1;
    } else if (core_pixel_is_double(pixel_type)) {
        pixel_type = CORE_PIXEL_D1;
    } else {
        g_return_val_if_fail(FALSE, FALSE);
    }

    /* cast dst_data from double back to uchar if needed */
    if (core_pixel_is_uint8(pixel_type)) {
        dst_data = g_malloc(area * sizeof(guint8));
        for (i = 0; i < area; ++i) {
            ((guint8 *) dst_data)[i] = dst_data_cast[i];
        }
        g_free(dst_data_cast);
    } else {
        dst_data = dst_data_cast;
    }

    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, CORE_COLOR_SPACE_GRAY_SCALE, pixel_type, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_GRAY_SCALE, pixel_type, size, FALSE);
    }
    g_object_unref(size);
    return TRUE;
}


gboolean imgproc_to_binary_threshold(CoreImage *src, CoreImage **dst, gdouble threshold, gboolean inverse) {
    CoreImage *gray_src = NULL;
    CoreSize *size;
    gsize i, area;
    gdouble *src_data, *dst_data;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
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
        core_image_new_with_data(dst_data, 1, CORE_PIXEL_D4, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, 1, CORE_PIXEL_D4, size, FALSE);
    }
    g_object_unref(size);
    g_object_unref(gray_src);
    return TRUE;
}

gboolean imgproc_to_binary_dither() { return FALSE; }

gboolean imgproc_to_binary_ordered_dither() { return FALSE; }