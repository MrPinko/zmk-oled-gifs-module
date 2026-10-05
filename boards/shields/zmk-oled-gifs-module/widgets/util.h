/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Shared utility helpers for the zmk-oled-gifs-module widgets.
 *
 * Screen geometry (Lily58 with nice!nano v2 + SSD1306 128×32 rotated):
 *   SCREEN_WIDTH  = 68 px  (the short axis, used as canvas height)
 *   SCREEN_HEIGHT = 160 px (the long axis, used as canvas width)
 *   BUFFER_SIZE   = 68 px  (square canvas, rotated 90° to fill the display)
 *
 * The display is physically 128×32 but mounted sideways, so we render into
 * a 68×68 canvas and rotate it 90° before pushing to the framebuffer.
 * BUFFER_OFFSET_BOTTOM positions the second canvas directly below the first.
 */

#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

/* ── Physical display geometry ───────────────────────────────────────────── */
#define SCREEN_WIDTH          68
#define SCREEN_HEIGHT         160

/* ── Canvas / render buffer ──────────────────────────────────────────────── */
#define BUFFER_SIZE           68
/*
 * The second canvas (bottom) is aligned relative to LV_ALIGN_TOP_RIGHT.
 * At −129 px in X it sits immediately left of the first canvas inside
 * the 160-pixel-wide container.
 */
#define BUFFER_OFFSET_BOTTOM  (-129)

/* ── LVGL color aliases ───────────────────────────────────────────────────── */
#define LVGL_BACKGROUND  (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) \
                              ? lv_color_black() : lv_color_white())
#define LVGL_FOREGROUND  (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) \
                              ? lv_color_white() : lv_color_black())

/* ── Shared keyboard status ───────────────────────────────────────────────── */
struct status_state {
    uint8_t battery;
    bool    charging;

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    /* Central-only fields */
    struct zmk_endpoint_instance selected_endpoint;
    int  active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
#else
    /* Peripheral-only field */
    bool connected;
#endif
};

/* ── Drawing helpers ─────────────────────────────────────────────────────── */

/** Convert a null-terminated string to upper-case in place. */
void to_uppercase(char *str);

/**
 * Rotate the canvas 90° clockwise.
 * @param canvas  The lv_canvas_t object to transform.
 * @param cbuf    The backing pixel buffer for @p canvas (used as scratch).
 */
void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]);

/** Fill the canvas with LVGL_BACKGROUND. */
void fill_background(lv_obj_t *canvas);

/** Initialise a rectangle draw descriptor with the given background colour. */
void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color);

/** Initialise a line draw descriptor with the given colour and line width. */
void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width);

/** Initialise a label draw descriptor with colour, font, and alignment. */
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align);
