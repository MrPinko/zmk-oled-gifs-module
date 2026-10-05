/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Profile indicator widget implementation.
 *
 * The "profiles" image (defined in assets/images.c) contains 5 hollow 3×3 px
 * dots evenly spaced at 7 px intervals. We overlay a filled 3×3 px rectangle
 * on the active profile position.
 */

#include <zephyr/kernel.h>

#include "profile.h"

/* Profile dots image defined in assets/images.c. */
LV_IMG_DECLARE(profiles);

/* ── Private helpers ─────────────────────────────────────────────────────── */

static void draw_inactive_profiles(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    /*
     * The y coordinate accounts for BUFFER_OFFSET_BOTTOM so the profiles row
     * appears at the very bottom of the composite display area.
     */
    lv_canvas_draw_img(canvas, 18, 145 + BUFFER_OFFSET_BOTTOM, &profiles, &img_dsc);
}

static void draw_active_profile(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_dsc;
    init_rect_dsc(&rect_dsc, LVGL_FOREGROUND);

    /* Each dot is 7 px apart; shift by active_profile_index × 7. */
    int offset = state->active_profile_index * 7;
    lv_canvas_draw_rect(canvas,
                        18 + offset,
                        145 + BUFFER_OFFSET_BOTTOM,
                        3, 3,            /* 3×3 px filled dot */
                        &rect_dsc);
}

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
    draw_inactive_profiles(canvas, state);
    draw_active_profile(canvas, state);
}
