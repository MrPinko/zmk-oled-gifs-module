/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Battery widget implementation.
 * Draws "BAT XX%" on the canvas, plus a bolt icon when the keyboard is charging
 * via USB.
 */

#include <zephyr/kernel.h>

#include "battery.h"
#include "../assets/custom_fonts.h"

/* The bolt icon is defined in assets/images.c. */
LV_IMG_DECLARE(bolt);

/* ── Private helpers ─────────────────────────────────────────────────────── */

static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10];
    snprintf(text, sizeof(text), "%d%%", state->battery);
    lv_canvas_draw_text(canvas, 26, 19, 42, &label_dsc, text);
}

static void draw_charging_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_RIGHT);

    char text[10];
    snprintf(text, sizeof(text), "%d%%", state->battery);
    /* Narrower text to make room for the bolt icon. */
    lv_canvas_draw_text(canvas, 26, 19, 35, &label_dsc, text);

    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 62, 21, &bolt, &img_dsc);
}

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    /* Draw the "BAT" label. */
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 19, 25, &label_dsc, "BAT");

#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    if (state->charging) {
        draw_charging_level(canvas, state);
        return;
    }
#endif
    draw_level(canvas, state);
}