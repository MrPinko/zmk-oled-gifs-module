/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Right (peripheral) side artwork: animated GIF display with cycle support.
 *
 * CONFIG_NICE_RIGHT_ANIMATION=y (default for right side):
 *   Uses lv_animimg to cycle through all frames in anim_imgs[] at the speed
 *   set by CONFIG_NICE_RIGHT_ANIMATION_MS.
 *
 * CONFIG_NICE_RIGHT_ANIMATION=n:
 *   Displays static images and cycles through right_cycle_images[] when the
 *   cycle combo is pressed.
 *
 * To add animation frames: see instructions in draw_right_image.h.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <events/oled_cycle_event.h>

#include "draw_right_image.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* ── Frame declarations ──────────────────────────────────────────────────── */

LV_IMG_DECLARE(right_image);
/* Add additional static or animation frame declarations here */

#define RIGHT_ANIM_FRAME_COUNT 1

static const lv_img_dsc_t *anim_imgs[RIGHT_ANIM_FRAME_COUNT] = {
    &right_image,
};

static const lv_img_dsc_t *right_cycle_images[] = {
    &right_image,
};

#define RIGHT_CYCLE_COUNT (sizeof(right_cycle_images) / sizeof(right_cycle_images[0]))

static lv_obj_t *right_art_obj = NULL;
static int right_current_image_idx = 0;

/* ── Cycle event listener ────────────────────────────────────────────────── */

static void cycle_image_update_cb(struct zmk_oled_cycle_event ev) {
    if (!right_art_obj || RIGHT_CYCLE_COUNT <= 1) {
        return;
    }
    right_current_image_idx = (right_current_image_idx + 1) % RIGHT_CYCLE_COUNT;
    LOG_INF("Cycling right OLED image to index %d", right_current_image_idx);
#if !IS_ENABLED(CONFIG_NICE_RIGHT_ANIMATION)
    lv_img_set_src(right_art_obj, right_cycle_images[right_current_image_idx]);
#endif
}

static struct zmk_oled_cycle_event cycle_image_get_state(const zmk_event_t *eh) {
    return (struct zmk_oled_cycle_event){};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_right_image_cycle, struct zmk_oled_cycle_event,
                            cycle_image_update_cb, cycle_image_get_state);
ZMK_SUBSCRIPTION(widget_right_image_cycle, zmk_oled_cycle_event);

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_right_image(lv_obj_t *parent) {
#if IS_ENABLED(CONFIG_NICE_RIGHT_ANIMATION)
    /* Animated mode: loop all frames in anim_imgs[] */
    lv_obj_t *art = lv_animimg_create(parent);

    lv_animimg_set_src(art, (const void **)anim_imgs, RIGHT_ANIM_FRAME_COUNT);
    lv_animimg_set_duration(art, CONFIG_NICE_RIGHT_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
    right_art_obj = art;
#else
    /* Static mode: display the current image from right_cycle_images[] */
    lv_obj_t *art = lv_img_create(parent);
    lv_img_set_src(art, right_cycle_images[right_current_image_idx]);
    right_art_obj = art;

    widget_right_image_cycle_init();
#endif

    /*
     * Position: starts at x=0 (fills the space under the TOP_RIGHT status bar).
     */
    lv_obj_align(art, NULL, LV_ALIGN_TOP_LEFT, 0, 0);
}
