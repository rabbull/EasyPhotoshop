//
// Created by karl_ on 2020-5-3.
//

#include <math.h>
#include <cblas.h>
#include <imgproc/color-convert.h>

gboolean imgproc_to_HSL(CoreImage *src, CoreImage **dst) {
    CoreSize *size;
    CoreColorSpace src_color_space;
    CorePixelType src_pixel_type;
    gdouble *src_data_cast, *dst_data;
    gdouble *hsl_pixel, *rgb_pixel, c_max, c_min, c;
    gsize i, j, area, block_size;

    size = core_image_get_size(src);
    area = core_size_get_area(size);
    block_size = sizeof(gdouble) * area * 3;
    src_color_space = core_image_get_color_space(src);
    src_pixel_type = core_image_get_pixel_type(src);

    g_return_val_if_fail(src_color_space == CORE_COLOR_SPACE_RGB, FALSE);

    if (core_pixel_is_double(src_pixel_type)) {
        src_data_cast = core_image_get_data(src);
    } else if (core_pixel_is_uint8(src_pixel_type)) {
        src_data_cast = malloc(block_size);
        for (i = 0; i < core_size_get_area(size) * 3; ++i) {
            src_data_cast[i] = ((guint8 *) core_image_get_data(src))[i] / core_pixel_get_range(src_pixel_type);
        }
    } else {
        return FALSE;
    }

    dst_data = g_malloc(block_size);
    for (i = 0; i < area; ++i) {
        hsl_pixel = dst_data + i * 3;
        rgb_pixel = src_data_cast + i * 3;
        c_min = 1;
        c_max = 0;
        for (j = 0; j < 3; ++j) {
            if (rgb_pixel[j] > c_max) {
                c_max = rgb_pixel[j];
            }
            if (rgb_pixel[j] < c_min) {
                c_min = rgb_pixel[j];
            }
        }
        c = c_max - c_min;

        /* Lightness */
        hsl_pixel[2] = (c_min + c_max) / 2;

        /* Saturation */
        if (!c) {
            hsl_pixel[1] = 0;
        } else {
            hsl_pixel[1] = c / (1 - fabs(hsl_pixel[2] * 2 - 1));
        }

        /* Hue */
        if (!c) {
            hsl_pixel[0] = 0;
        } else {
            if (rgb_pixel[0] == c_max) {
                hsl_pixel[0] = 1.0 / 6 * (rgb_pixel[1] - rgb_pixel[2]) / c;
            } else if (rgb_pixel[1] == c_max) {
                hsl_pixel[0] = 1.0 / 6 * (2 + (rgb_pixel[2] - rgb_pixel[0]) / c);
            } else if (rgb_pixel[2] == c_max) {
                hsl_pixel[0] = 1.0 / 6 * (4 + (rgb_pixel[0] - rgb_pixel[1]) / c);
            }
        }
        while (hsl_pixel[0] > 1) {
            hsl_pixel[0] -= 1;
        }
        while (hsl_pixel[0] < 0) {
            hsl_pixel[0] += 1;
        }
    }

    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, CORE_COLOR_SPACE_HSL, CORE_PIXEL_D3, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_HSL, CORE_PIXEL_D3, size, FALSE);
    }

    if (core_pixel_is_uint8(src_pixel_type)) {
        free(src_data_cast);
    }
    g_object_unref(size);

    return TRUE;
}

gboolean imgproc_to_RGB(CoreImage *src, CoreImage **dst) {
    CoreSize *size;
    CoreColorSpace color_space;
    CorePixelType pixel_type;
    gdouble *src_data, *dst_data;
    gdouble *hsl_pixel, *rgb_pixel;
    gdouble c, h, h_ceil, x, m, r, g, b;
    gsize i, area;

    g_return_val_if_fail(src, FALSE);
    color_space = core_image_get_color_space(src);
    pixel_type = core_image_get_pixel_type(src);
    g_return_val_if_fail(color_space == CORE_COLOR_SPACE_HSL, FALSE);
    g_return_val_if_fail(pixel_type == CORE_PIXEL_D3, FALSE);
    size = core_image_get_size(src);
    area = core_size_get_area(size);
    src_data = core_image_get_data(src);
    dst_data = g_malloc(sizeof(gdouble) * area * 3);
    for (i = 0; i < area; ++i) {
        hsl_pixel = src_data + i * 3;
        rgb_pixel = dst_data + i * 3;

        c = (1 - fabs(2 * hsl_pixel[2] - 1)) * hsl_pixel[1];
        h = hsl_pixel[0] * 6.0;
        x = c * (1 - fabs(h - floor(h / 2) * 2 - 1));
        h_ceil = floor(h);
        if (h_ceil >= 5) {
            r = c;
            g = 0;
            b = x;
        } else if (h_ceil >= 4) {
            r = x;
            g = 0;
            b = c;
        } else if (h_ceil >= 3) {
            r = 0;
            g = x;
            b = c;
        } else if (h_ceil >= 2) {
            r = 0;
            g = c;
            b = x;
        } else if (h_ceil >= 1) {
            r = x;
            g = c;
            b = 0;
        } else if (h_ceil >= 0) {
            r = c;
            g = x;
            b = 0;
        } else {
            r = g = b = 0;
        }
        m = hsl_pixel[2] - c / 2;
        rgb_pixel[0] = r + m;
        rgb_pixel[1] = g + m;
        rgb_pixel[2] = b + m;
    }

    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, CORE_COLOR_SPACE_RGB, CORE_PIXEL_D3, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_RGB, CORE_PIXEL_D3, size, FALSE);
    }

    g_object_unref(size);
    return TRUE;
}

gboolean imgproc_to_grayscale(CoreImage *src, CoreImage **dst) {
    gsize i;
    CoreSize *size;
    gdouble *dst_data_cast, *src_data_cast;
    gpointer src_data, dst_data;
    gsize channel, area;
    CorePixelType pixel_type;
    g_return_val_if_fail(src != NULL, FALSE);
    g_return_val_if_fail(dst != NULL, FALSE);

    size = core_image_get_size(src);
    area = core_size_get_area(size);
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
        for (i = 0; i < area; ++i) {
            ((guint8 *) dst_data_cast)[i] = dst_data_cast[i];
        }
    }
    dst_data = dst_data_cast;
    if (core_pixel_is_uint8(pixel_type)) {
        dst_data = g_realloc(dst_data, sizeof(guint8) * area);
    }

    /* assign result */
    if (*dst == NULL) {
        *dst = core_image_new_with_data(dst_data, CORE_COLOR_SPACE_GRAY_SCALE, pixel_type, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_GRAY_SCALE, pixel_type, size, FALSE);
    }

    /* free additional allocated memory */
    if (core_pixel_is_uint8(pixel_type)) {
        g_free(src_data_cast);
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
        core_image_new_with_data(dst_data, CORE_COLOR_SPACE_BIN, CORE_PIXEL_U1, size, FALSE);
    } else {
        core_image_assign_data(*dst, dst_data, CORE_COLOR_SPACE_BIN, CORE_PIXEL_U1, size, FALSE);
    }
    g_object_unref(size);
    g_object_unref(gray_src);
    return TRUE;
}

gboolean imgproc_to_binary_dither() { return FALSE; }

gboolean imgproc_to_binary_ordered_dither() { return FALSE; }