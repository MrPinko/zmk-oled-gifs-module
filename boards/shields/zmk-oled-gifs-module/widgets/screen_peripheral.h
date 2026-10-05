/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Peripheral (right) side screen widget: aggregates battery status and
 * BLE peripheral connectivity, plus the right-side animated artwork.
 */

#pragma once

#include <lvgl.h>
#include <zephyr/kernel.h>

#include "util.h"

/**
 * Internal widget state for the peripheral screen.
 * Only one canvas buffer is used (top status strip only—no profile dots
 * on the peripheral side).
 */
struct zmk_widget_screen {
    sys_snode_t  node;
    lv_obj_t    *obj;
    lv_color_t   cbuf[BUFFER_SIZE * BUFFER_SIZE];  /**< Status strip buffer */
    struct status_state state;
};

/**
 * Initialise the peripheral screen widget and attach it to @p parent.
 * @return 0 on success, negative errno on error.
 */
int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);

/** Return the root LVGL object for the peripheral screen widget. */
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);