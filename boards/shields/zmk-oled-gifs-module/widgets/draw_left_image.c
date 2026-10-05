/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Left (central) side artwork: static image display.
 *
 * CONFIG_NICE_LEFT_ANIMATION=n (default for left side):
 *   Uses lv_img to display the first frame of left_image as a static bitmap.
 *
 * CONFIG_NICE_LEFT_ANIMATION=y:
 *   Uses lv_animimg to cycle through all frames in anim_imgs[] at the speed
 *   set by CONFIG_NICE_LEFT_ANIMATION_MS.
 *
 * To add animation frames: see instructions in draw_left_image.h.
 */

#include <zephyr/kernel.h>
#include "draw_left_image.h"

/* ── Frame declarations ──────────────────────────────────────────────────── */

/*
 * Declare the image symbol(s) exported by assets/left_image.c.
 * If your GIF has multiple frames, add more LV_IMG_DECLARE() lines here
 * and add the corresponding pointers to anim_imgs[].
 */
LV_IMG_DECLARE(left_image);

/*
 * Array of image frame pointers for lv_animimg.
 * Update LEFT_ANIM_FRAME_COUNT when adding more frames.
 */
#define LEFT_ANIM_FRAME_COUNT 1

static const lv_img_dsc_t *anim_imgs[LEFT_ANIM_FRAME_COUNT] = {
    &left_image,
    /* Add more frames here, e.g.: &left_image_frame_1, &left_image_frame_2, … */
};

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_left_image(lv_obj_t *parent) {
#if IS_ENABLED(CONFIG_NICE_LEFT_ANIMATION)
    /*
     * Animated mode: create an lv_animimg widget and set it to loop through
     * all frames in anim_imgs[] indefinitely.
     */
    lv_obj_t *art = lv_animimg_create(parent);

    lv_animimg_set_src(art, (const void **)anim_imgs, LEFT_ANIM_FRAME_COUNT);
    lv_animimg_set_duration(art, CONFIG_NICE_LEFT_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
#else
    /*
     * Static mode: display only the first frame as a regular image.
     */
    lv_obj_t *art = lv_img_create(parent);
    lv_img_set_src(art, &left_image);
#endif

    /*
     * Position: left side of the display canvas, offset 36 px from the
     * right edge to leave room for the status strip.
     */
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
