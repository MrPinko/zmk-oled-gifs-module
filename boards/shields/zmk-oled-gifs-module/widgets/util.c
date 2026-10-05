/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Shared drawing utility functions for the zmk-oled-gifs-module widgets.
 */

#include <ctype.h>
#include <zephyr/kernel.h>

#include "util.h"

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    /* Copy the current canvas content into a temporary buffer. */
    static lv_color_t cbuf_tmp[BUFFER_SIZE * BUFFER_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

    /* Wrap the temp buffer in an image descriptor for lv_canvas_transform. */
    lv_img_dsc_t img = {
        .data        = (const uint8_t *)cbuf_tmp,
        .header.cf   = LV_IMG_CF_TRUE_COLOR,
        .header.w    = BUFFER_SIZE,
        .header.h    = BUFFER_SIZE,
    };

    /* Clear the canvas, then blit the image rotated 90° (angle = 900 decidegrees). */
    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
    lv_canvas_transform(canvas, &img,
                        900,              /* angle in 0.1-degree units → 90° */
                        LV_IMG_ZOOM_NONE,
                        -1, 0,            /* pivot x, y offset */
                        BUFFER_SIZE / 2,  /* pivot column centre */
                        BUFFER_SIZE / 2,  /* pivot row centre */
                        false);           /* anti-alias: off (1-bit display) */
}

void fill_background(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_BACKGROUND);
    lv_canvas_draw_rect(canvas, 0, 0, BUFFER_SIZE, BUFFER_SIZE, &rect_dsc);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font  = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}