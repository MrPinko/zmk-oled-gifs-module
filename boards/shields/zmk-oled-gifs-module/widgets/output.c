/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Output / connectivity widget implementation.
 *
 * Icon layout (inside a 24×15 px white-filled rectangle at x=43, y=0):
 *   USB connected  → USB plug icon  (20×11 px) at x=45, y=2
 *   BLE bonded+connected    → Bluetooth icon (12×15 px) at x=49, y=0
 *   BLE bonded+disconnected → BT-no-signal   (12×15 px) at x=49, y=0
 *   BLE unbonded            → BT-unbonded    (22×15 px) at x=44, y=0
 */

#include <zephyr/kernel.h>

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
    lv_canvas_draw_img(canvas, 45, 2, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 44, 0, &bt_unbonded, &img_dsc);
}

#endif /* central */

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 49, 0, &bt_no_signal, &img_dsc);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 49, 0, &bt, &img_dsc);
}

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
    /* Draw "SIG" label. */
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 1, 25, &label_dsc, "SIG");

    /* White filled rectangle as icon background. */
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);
    lv_canvas_draw_rect(canvas, 43, 0, 24, 15, &rect_dsc);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    /* Central: select icon based on active transport and BLE bond state. */
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (!state->active_profile_bonded) {
            draw_ble_unbonded(canvas);
        } else if (state->active_profile_connected) {
            draw_ble_connected(canvas);
        } else {
            draw_ble_disconnected(canvas);
        }
        break;
    }
#else
    /* Peripheral: show connection state only. */
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}