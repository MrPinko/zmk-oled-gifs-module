/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Entry point for the zmk-oled-gifs-module custom status screen.
 *
 * ZMK calls zmk_display_status_screen() to create the root LVGL screen
 * object.  We create a bare lv_obj_t screen and, if the widget is enabled,
 * attach the zmk_widget_screen to it and align it to the top-left corner.
 *
 * The font file (pixel_operator_mono.c) is #included directly here because
 * it defines an lv_font_t that LVGL's font registration mechanism expects
 * to be available at link time via LV_FONT_DECLARE in custom_fonts.h.
 */

#include "widgets/screen.h"
#include "assets/custom_fonts.h"

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* Include font data (defines pixel_operator_mono lv_font_t). */
#include "assets/pixel_operator_mono.c"

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STATUS)
static struct zmk_widget_screen screen_widget;
#endif /* CONFIG_NICE_OLED_WIDGET_STATUS */

lv_obj_t *zmk_display_status_screen(void) {
    lv_obj_t *screen = lv_obj_create(NULL);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_STATUS)
    zmk_widget_screen_init(&screen_widget, screen);
    lv_obj_align(zmk_widget_screen_obj(&screen_widget), LV_ALIGN_TOP_LEFT, 0, 0);
#endif /* CONFIG_NICE_OLED_WIDGET_STATUS */

    return screen;
}
