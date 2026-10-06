/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Shared utility helpers for the zmk-oled-gifs-module widgets.
 *
 * Screen geometry: Standard 128×32 OLED (SSD1306) mounted vertically on Lily58:
 *   SCREEN_WIDTH    = 32 px  (physical width across the board)
 *   SCREEN_HEIGHT   = 128 px (physical height along the board)
 *   STATUS_BAR_SIZE = 20 px  (status bar height along the OLED, configurable)
 *
 * Layout along the 128 px vertical length:
 *   Physical TOP    : Status bar (STATUS_BAR_SIZE × 32 px): [BLE] [Battery] in one row
 *   Physical BOTTOM : Image / Animated GIF area (under status bar)
 */

#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

/* ── Physical display geometry (128×32 OLED vertical) ────────────────────── */
#define SCREEN_WIDTH          32
#define SCREEN_HEIGHT         128

/*
 * Height of the status bar in pixels (along the vertical OLED length).
 * Can be modified here or overridden via CONFIG_NICE_OLED_STATUS_BAR_SIZE.
 * Default: 20 px.
 */
#ifndef STATUS_BAR_SIZE
#ifdef CONFIG_NICE_OLED_STATUS_BAR_SIZE
#define STATUS_BAR_SIZE       CONFIG_NICE_OLED_STATUS_BAR_SIZE
#else
#define STATUS_BAR_SIZE       10
#endif
#endif

/* ── LVGL color aliases ───────────────────────────────────────────────────── */
#define LVGL_BACKGROUND  (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) \
                              ? lv_color_black() : lv_color_white())
#define LVGL_FOREGROUND  (IS_ENABLED(CONFIG_NICE_OLED_WIDGET_INVERTED) \
                              ? lv_color_white() : lv_color_black())

/* ── Shared keyboard status ───────────────────────────────────────────────── */
struct status_state {
    uint8_t battery;
    bool    charging;
    bool    connected;

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    /* Central-only fields */
    struct zmk_endpoint_instance selected_endpoint;
    int  active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
#endif
};

/* ── Drawing helpers ─────────────────────────────────────────────────────── */

/** Convert a null-terminated string to upper-case in place. */
void to_uppercase(char *str);

/**
 * Prepares the canvas for unrotated horizontal drawing (32 px wide × STATUS_BAR_SIZE tall)
 * and fills it with background color.
 */
void prepare_status_canvas(lv_obj_t *canvas);

/**
 * Rotates the 32×STATUS_BAR_SIZE unrotated drawing 90° clockwise into @p cbuf
 * (STATUS_BAR_SIZE×32) and flushes to the canvas.
 * @param canvas  The lv_canvas_t object.
 * @param cbuf    The backing pixel buffer for display.
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
