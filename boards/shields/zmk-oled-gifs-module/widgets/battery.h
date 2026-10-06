/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Battery widget: draws the battery percentage (and a bolt icon when charging)
 * onto an LVGL canvas.
 */

#pragma once

#include <lvgl.h>
#include "util.h"

struct battery_status_state {
    uint8_t level;          /**< State of charge, 0–100 */
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    bool usb_present;       /**< True when USB power is detected */
#endif
};

/**
 * Draw the battery status (percentage text, optional bolt icon) onto @p canvas
 * using the current @p state.
 */
void draw_battery_status(lv_obj_t *canvas, const struct status_state *state);
