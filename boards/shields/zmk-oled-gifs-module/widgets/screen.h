/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Central (left) side screen widget: aggregates battery, output (BLE/USB),
 * BLE profile indicator, and the left artwork into a single LVGL container.
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

#include "util.h"

/**
 * Internal widget state for the central screen.
 * Two canvas buffers: one for the top status strip, one for the bottom strip.
 */
struct zmk_widget_screen {
    sys_snode_t   node;
    lv_obj_t     *obj;
    lv_color_t    cbuf[BUFFER_SIZE * BUFFER_SIZE];   /**< Top canvas buffer  */
    lv_color_t    cbuf3[BUFFER_SIZE * BUFFER_SIZE];  /**< Bottom canvas buffer */
    struct status_state state;
};

/**
 * Initialise the central screen widget and attach it to @p parent.
 * @return 0 on success, negative errno on error.
 */
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);

/** Return the root LVGL object for the screen widget. */
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
