/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_oled_gif_cycle

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <drivers/behavior.h>

/*
 * The renderer (and the public API it exposes) is only compiled on halves
 * that have CONFIG_ZMK_DISPLAY=y.  Guard the call accordingly so the
 * peripheral-side build (which includes this file via BEHAVIOR_LOCALITY_GLOBAL)
 * doesn't produce a linker error if it has no display.
 */
#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
#include <zmk_oled_gif/gif.h>
#endif

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event)
{
#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
    /*
     * Both halves reach this point (global locality).
     * Each side cycles its own local renderer independently.
     * Because both start from GIF 0 and always advance by 1,
     * they remain in sync across presses.
     */
    zmk_oled_gif_cycle();
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event)
{
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api behavior_oled_gif_cycle_driver_api = {
    .binding_pressed  = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    /*
     * BEHAVIOR_LOCALITY_GLOBAL: ZMK automatically dispatches this binding
     * to the peripheral over BLE, so both OLEDs update on a single keypress.
     * Node name in keymap MUST be <= 8 characters (ZMK requirement).
     */
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
};

BEHAVIOR_DT_INST_DEFINE(0, NULL, NULL, NULL, NULL,
                        POST_KERNEL,
                        CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                        &behavior_oled_gif_cycle_driver_api);

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
