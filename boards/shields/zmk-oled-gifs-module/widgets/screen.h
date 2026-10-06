/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Central (left) side screen widget for 128×32 vertical OLED:
 *   - Status bar (STATUS_BAR_SIZE × 32 px): [BLE] [Battery] in one row at TOP
 *   - Artwork area: Image / GIF animation (under status bar)
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

#include "util.h"

/**
 * Internal widget state for the central screen.
 * Buffer sized for STATUS_BAR_SIZE × SCREEN_WIDTH.
 */
struct zmk_widget_screen {
    sys_snode_t   node;
    lv_obj_t     *obj;
    lv_color_t    cbuf[STATUS_BAR_SIZE * SCREEN_WIDTH];   /**< Status bar buffer */
    struct status_state state;
};

/**
 * Initialise the central screen widget and attach it to @p parent.
 * @return 0 on success, negative errno on error.
 */
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);

/** Return the root LVGL object for the screen widget. */
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
