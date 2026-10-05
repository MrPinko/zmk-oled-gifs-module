/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Battery widget implementation for 32×32 status canvas:
 *   - Lower half (y = 17..31) of the 32×32 canvas.
 *   - Normal: percentage centered across the 32 px width (e.g. "95%").
 *   - Charging: bolt icon + percentage text.
 */

#include <zephyr/kernel.h>
#include <stdio.h>

#include "battery.h"
#include "../assets/custom_fonts.h"

/* The bolt icon (5×9 px) is defined in assets/images.c. */
LV_IMG_DECLARE(bolt);

/* ── Private helpers ─────────────────────────────────────────────────────── */

static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);

    char text[8];
    snprintf(text, sizeof(text), "%d%%", state->battery);
    /* Centered across the full 32 px width */
    lv_canvas_draw_text(canvas, 0, 18, 32, &label_dsc, text);
}

static void draw_charging_level(lv_obj_t *canvas, const struct status_state *state) {
    /* Bolt icon (5×9 px) on the left */
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 2, 19, &bolt, &img_dsc);

    /* Percentage text next to bolt */
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);

    char text[8];
    snprintf(text, sizeof(text), "%d%%", state->battery);
    lv_canvas_draw_text(canvas, 9, 18, 23, &label_dsc, text);
}

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    if (state->charging) {
        draw_charging_level(canvas, state);
        return;
    }
#endif
    draw_level(canvas, state);
}