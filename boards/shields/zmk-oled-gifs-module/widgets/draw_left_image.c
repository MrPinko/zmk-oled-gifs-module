/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Left (central) side artwork display with image cycle support.
 *
 * CONFIG_NICE_LEFT_ANIMATION=n (default for left side):
 *   Shows the current static image. Pressing the cycle combo advances to
 *   the next image in left_cycle_images[].
 *
 * CONFIG_NICE_LEFT_ANIMATION=y:
 *   Loops through all animation frames in anim_imgs[].
 *
 * HOW TO ADD MORE IMAGES TO CYCLE:
 *   1. Convert each 32 px wide image with LVGL Image Converter (CF_INDEXED_1BIT).
 *   2. Paste the generated lv_img_dsc_t into assets/left_image.c (e.g. left_image_2).
 *   3. Declare it below: LV_IMG_DECLARE(left_image_2);
 *   4. Add &left_image_2 to left_cycle_images[].
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/display.h>
#include <events/oled_cycle_event.h>

#include "draw_left_image.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/* ── Image & frame declarations ──────────────────────────────────────────── */

LV_IMG_DECLARE(left_image);
LV_IMG_DECLARE(left_image_2);
/* Add more image declarations here:
 * LV_IMG_DECLARE(left_image_2);
 * LV_IMG_DECLARE(left_image_3);
 */

/* List of images to cycle through when cycle combo is pressed */
static const lv_img_dsc_t *left_cycle_images[] = {
    &left_image,
    &left_image_2,
    /* Add additional images to cycle through here:
     * &left_image_2,
     * &left_image_3,
     */
};

#define LEFT_CYCLE_COUNT (sizeof(left_cycle_images) / sizeof(left_cycle_images[0]))

/* Animation frame list (used when CONFIG_NICE_LEFT_ANIMATION=y) */
#define LEFT_ANIM_FRAME_COUNT 2

static const lv_img_dsc_t *anim_imgs[LEFT_ANIM_FRAME_COUNT] = {
    &left_image,
    &left_image_2,
};

static lv_obj_t *left_art_obj = NULL;
static int left_current_image_idx = 0;

/* ── Cycle event listener ────────────────────────────────────────────────── */

static void cycle_image_update_cb(struct zmk_oled_cycle_event ev) {
    if (!left_art_obj || LEFT_CYCLE_COUNT <= 1) {
        return;
    }
    left_current_image_idx = (left_current_image_idx + 1) % LEFT_CYCLE_COUNT;
    LOG_INF("Cycling left OLED image to index %d", left_current_image_idx);
    lv_img_set_src(left_art_obj, left_cycle_images[left_current_image_idx]);
}

static struct zmk_oled_cycle_event cycle_image_get_state(const zmk_event_t *eh) {
    return (struct zmk_oled_cycle_event){};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_left_image_cycle, struct zmk_oled_cycle_event,
                            cycle_image_update_cb, cycle_image_get_state);
ZMK_SUBSCRIPTION(widget_left_image_cycle, zmk_oled_cycle_event);

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_left_image(lv_obj_t *parent) {
#if IS_ENABLED(CONFIG_NICE_LEFT_ANIMATION)
    /* Animated mode: loop all frames in anim_imgs[] */
    lv_obj_t *art = lv_animimg_create(parent);

    lv_animimg_set_src(art, (const void **)anim_imgs, LEFT_ANIM_FRAME_COUNT);
    lv_animimg_set_duration(art, CONFIG_NICE_LEFT_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
    left_art_obj = art;
#else
    /* Static mode: display the current image from left_cycle_images[] */
    lv_obj_t *art = lv_img_create(parent);
    lv_img_set_src(art, left_cycle_images[left_current_image_idx]);
    left_art_obj = art;

    widget_left_image_cycle_init();
#endif

    /*
     * Position: starts at x=0 (fills the space under the TOP_RIGHT status bar).
     */
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 0, 0);
}
