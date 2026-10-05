/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Output / connectivity widget header.
 *
 * On the central side: shows USB or BLE connectivity icon + BLE profile state.
 * On the peripheral side: shows BLE connection state.
 */

#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

#include "util.h"

/* ── State structs ───────────────────────────────────────────────────────── */

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
/** State for the central (left) side output widget. */
struct output_status_state {
    struct zmk_endpoint_instance selected_endpoint;
    int  active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
};
#else
/** State for the peripheral (right) side output widget. */
struct peripheral_status_state {
    bool connected;
};
#endif

/* ── Drawing ─────────────────────────────────────────────────────────────── */

/**
 * Draw the output / connectivity status icon onto @p canvas using @p state.
 */
void draw_output_status(lv_obj_t *canvas, const struct status_state *state);