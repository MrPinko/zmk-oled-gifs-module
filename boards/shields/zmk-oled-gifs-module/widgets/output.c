/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Output / connectivity widget implementation for 32×32 status canvas:
 *   - Upper half (y = 1..16) of the 32×32 canvas.
 *   - Central side:
 *       USB: centered USB plug icon (20×11 px)
 *       BLE bonded: Bluetooth icon (12×15 px) + active profile number (1..5)
 *       BLE unbonded: centered unbonded icon (22×15 px)
 *   - Peripheral side:
 *       Centered Bluetooth icon (connected or no signal)
 */

#include <zephyr/kernel.h>
#include <stdio.h>

#include "output.h"
#include "../assets/custom_fonts.h"

/* Icon image descriptors defined in assets/images.c. */
LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

/* ── Private draw helpers ────────────────────────────────────────────────── */

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    /* USB icon is 20×11 px; center in 32 px: x = (32 - 20) / 2 = 6 */
    lv_canvas_draw_img(canvas, 6, 2, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    /* Unbonded icon is 22×15 px; center in 32 px: x = (32 - 22) / 2 = 5 */
    lv_canvas_draw_img(canvas, 5, 1, &bt_unbonded, &img_dsc);
}

static void draw_profile_number(lv_obj_t *canvas, int profile_index) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);

    char text[4];
    snprintf(text, sizeof(text), "%d", profile_index + 1);
    /* Profile number at x=18, y=2 next to 12 px BT icon */
    lv_canvas_draw_text(canvas, 18, 2, 12, &label_dsc, text);
}

static void draw_ble_central(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    if (!state->active_profile_bonded) {
        draw_ble_unbonded(canvas);
        return;
    }

    /* BT icon at x=4, y=1 (12 px wide) + profile number at x=18 */
    if (state->active_profile_connected) {
        lv_canvas_draw_img(canvas, 4, 1, &bt, &img_dsc);
    } else {
        lv_canvas_draw_img(canvas, 4, 1, &bt_no_signal, &img_dsc);
    }

    draw_profile_number(canvas, state->active_profile_index);
}

#else /* peripheral */

static void draw_ble_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    /* Center 12×15 px BT icon in 32 px: x = (32 - 12) / 2 = 10 */
    if (state->connected) {
        lv_canvas_draw_img(canvas, 10, 1, &bt, &img_dsc);
    } else {
        lv_canvas_draw_img(canvas, 10, 1, &bt_no_signal, &img_dsc);
    }
}

#endif

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        draw_ble_central(canvas, state);
        break;
    }
#else
    draw_ble_peripheral(canvas, state);
#endif
}