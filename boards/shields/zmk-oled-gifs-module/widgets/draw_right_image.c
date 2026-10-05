/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Right (peripheral) side artwork: animated GIF display.
 *
 * CONFIG_NICE_RIGHT_ANIMATION=y (default for right/peripheral side):
 *   Uses lv_animimg to cycle through all frames in anim_imgs[] at the speed
 *   set by CONFIG_NICE_RIGHT_ANIMATION_MS.
 *
 * CONFIG_NICE_RIGHT_ANIMATION=n:
 *   Uses lv_img to display the first frame as a static bitmap.
 *
 * To add animation frames: see instructions in draw_right_image.h.
 *
 * ── Quick-start: How to turn your GIF into animation frames ──────────────
 * 1. Split your GIF into individual frames (e.g. with ffmpeg or GIMP).
 * 2. Resize each frame to fit your display (e.g. 68×68 px for Lily58).
 * 3. Go to https://lvgl.io/tools/imageconverter
 *    - Color format: CF_INDEXED_1BIT (monochrome, 1-bit)
 *    - Output format: C array
 *    - Name each frame right_image_frame_0, right_image_frame_1, etc.
 * 4. Paste each generated lv_img_dsc_t into assets/right_image.c.
 * 5. Add LV_IMG_DECLARE() and pointer entries below.
 * 6. Update RIGHT_ANIM_FRAME_COUNT.
 */

#include <zephyr/kernel.h>
#include "draw_right_image.h"

/* ── Frame declarations ──────────────────────────────────────────────────── */

/*
 * Declare the image symbol(s) exported from assets/right_image.c.
 *
 * Example for a 4-frame GIF:
 *   LV_IMG_DECLARE(right_image_frame_0);
 *   LV_IMG_DECLARE(right_image_frame_1);
 *   LV_IMG_DECLARE(right_image_frame_2);
 *   LV_IMG_DECLARE(right_image_frame_3);
 *
 * For now we have a single placeholder frame.
 */
LV_IMG_DECLARE(right_image);

/*
 * Frame pointer array for lv_animimg.
 * Update RIGHT_ANIM_FRAME_COUNT when you add more frames.
 */
#define RIGHT_ANIM_FRAME_COUNT 1

static const lv_img_dsc_t *anim_imgs[RIGHT_ANIM_FRAME_COUNT] = {
    &right_image,
    /*
     * Add more frames here, e.g.:
     *   &right_image_frame_1,
     *   &right_image_frame_2,
     */
};

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_right_image(lv_obj_t *parent) {
#if IS_ENABLED(CONFIG_NICE_RIGHT_ANIMATION)
    /*
     * Animated mode (default for right/peripheral side):
     * lv_animimg loops through all frames indefinitely at the configured speed.
     *
     * The duration is the total time for ONE full loop across all frames.
     * For example, with 16 frames at 2400 ms: each frame shows for 150 ms.
     */
    lv_obj_t *art = lv_animimg_create(parent);

    lv_animimg_set_src(art, (const void **)anim_imgs, RIGHT_ANIM_FRAME_COUNT);
    lv_animimg_set_duration(art, CONFIG_NICE_RIGHT_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
#else
    /*
     * Static mode: display only the first frame.
     */
    lv_obj_t *art = lv_img_create(parent);
    lv_img_set_src(art, &right_image);
#endif

    /*
     * Position: offset 36 px from left inside the parent container,
     * top-aligned — this leaves the leftmost 36 px for the status strip.
     */
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
}
