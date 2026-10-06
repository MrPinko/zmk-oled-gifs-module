/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Battery widget implementation:
 *   - Drawn in the right section (x = 13..31) on the SAME ROW as Bluetooth.
 *   - Normal: percentage text right-aligned (e.g. "95%", "100").
 *   - Charging: bolt icon + percentage text.
 */

#include <zephyr/kernel.h>
#include <stdio.h>

#include "battery.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bolt);

static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_8, LV_TEXT_ALIGN_RIGHT);

    char text[8];
    if (state->battery >= 100) {
        snprintf(text, sizeof(text), "100");
    } else {
        snprintf(text, sizeof(text), "%d%%", state->battery);
    }
    /* Right side of row: x = 13, y = 3, width = 19 */
    lv_canvas_draw_text(canvas, 13, 3, 19, &label_dsc, text);
}

static void draw_charging_level(lv_obj_t *canvas, const struct status_state *state) {
    /* Bolt icon (5×9 px) */
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 13, 5, &bolt, &img_dsc);

    /* Text next to bolt: x = 19, y = 3, width = 13 */
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_8, LV_TEXT_ALIGN_LEFT);

    char text[8];
    snprintf(text, sizeof(text), "%d", state->battery);
    lv_canvas_draw_text(canvas, 19, 3, 13, &label_dsc, text);
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    if (state->charging) {
        draw_charging_level(canvas, state);
        return;
    }
#endif
    draw_level(canvas, state);
}