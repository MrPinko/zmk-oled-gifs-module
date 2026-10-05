/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Profile indicator widget: draws 5 small dots representing BLE profiles,
 * with the active profile dot filled in.
 */

#pragma once

#include <lvgl.h>
#include "util.h"

/**
 * Draw the BLE profile indicator onto @p canvas using @p state.
 * Shows up to 5 profile dots; the active profile dot is filled.
 */
void draw_profile_status(lv_obj_t *canvas, const struct status_state *state);