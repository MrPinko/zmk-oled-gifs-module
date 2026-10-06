/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Output / connectivity widget implementation:
 *   - Drawn in the left section (x = 0..12) on the SAME ROW as battery.
 *   - Central side: USB or Bluetooth icon.
 *   - Peripheral side: Bluetooth connection status icon.
 */

#include <zephyr/kernel.h>
#include <stdio.h>

#include "output.h"
#include "../assets/custom_fonts.h"

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 0, 4, &usb, &img_dsc);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 0, 2, &bt_unbonded, &img_dsc);
}

static void draw_ble_central(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    if (!state->active_profile_bonded) {
        draw_ble_unbonded(canvas);
        return;
    }

    if (state->active_profile_connected) {
        lv_canvas_draw_img(canvas, 0, 2, &bt, &img_dsc);
    } else {
        lv_canvas_draw_img(canvas, 0, 2, &bt_no_signal, &img_dsc);
    }
}

#else /* peripheral */

static void draw_ble_peripheral(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);

    if (state->connected) {
        lv_canvas_draw_img(canvas, 0, 2, &bt, &img_dsc);
    } else {
        lv_canvas_draw_img(canvas, 0, 2, &bt_no_signal, &img_dsc);
    }
}

#endif

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