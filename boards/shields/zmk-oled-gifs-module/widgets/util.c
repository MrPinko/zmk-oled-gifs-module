/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Shared drawing utility functions for the zmk-oled-gifs-module widgets.
 */

#include <ctype.h>
#include <zephyr/kernel.h>

#include "util.h"

/* Scratch buffer for unrotated drawing (width = SCREEN_WIDTH, height = STATUS_BAR_SIZE) */
static lv_color_t draw_buf[SCREEN_WIDTH * STATUS_BAR_SIZE];

void to_uppercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

void prepare_status_canvas(lv_obj_t *canvas) {
    lv_canvas_set_buffer(canvas, draw_buf, SCREEN_WIDTH, STATUS_BAR_SIZE, LV_IMG_CF_TRUE_COLOR);
    fill_background(canvas);
}

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    /* 90° clockwise rotation from draw_buf (32×STATUS_BAR_SIZE) into cbuf (STATUS_BAR_SIZE×32) */
    for (int y = 0; y < STATUS_BAR_SIZE; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int rx = STATUS_BAR_SIZE - 1 - y;
            int ry = x;
            cbuf[ry * STATUS_BAR_SIZE + rx] = draw_buf[y * SCREEN_WIDTH + x];
        }
    }
    lv_canvas_set_buffer(canvas, cbuf, STATUS_BAR_SIZE, SCREEN_WIDTH, LV_IMG_CF_TRUE_COLOR);
}

void fill_background(lv_obj_t *canvas) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_BACKGROUND);
    lv_canvas_draw_rect(canvas, 0, 0, SCREEN_WIDTH, STATUS_BAR_SIZE, &rect_dsc);
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