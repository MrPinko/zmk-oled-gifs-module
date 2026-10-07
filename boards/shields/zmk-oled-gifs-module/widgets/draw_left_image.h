/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Left (central) side artwork display.
 *
 * draw_left_image()     – draws the first frame as a static image.
 * draw_left_animation() – drives lv_animimg to loop through all GIF frames.
 *
 * Which function is active is controlled by CONFIG_NICE_LEFT_ANIMATION:
 *   y → animation
 *   n → static first frame
 *
 * The image frames are declared in assets/left_image.c.
 * By convention the symbol names follow the pattern:
 *   left_image_frame_<n>  for multi-frame GIFs
 *   left_image            for a single / static image
 *
 * HOW TO ADD MORE FRAMES:
 *   1. Convert each GIF frame to a C array with the LVGL Image Converter
 *      (https://lvgl.io/tools/imageconverter), choose CF_INDEXED_1BIT.
 *   2. Add each lv_img_dsc_t to assets/left_image.c.
 *   3. Add the corresponding LV_IMG_DECLARE() below and add a pointer to
 *      anim_imgs[].
 *   4. Update LEFT_ANIM_FRAME_COUNT.
 */

#pragma once

#include <lvgl.h>

/** Draw the left-side artwork (static or animated). */
void draw_left_image(lv_obj_t *parent);


struct left_playlist_entry
{
    const lv_img_dsc_t *const *frames;
    uint8_t frame_count;
    uint32_t duration_ms;
};


LV_IMG_DECLARE(left_image);
LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);

static const lv_img_dsc_t *const left_image_frames[] = {&left_image};

static const lv_img_dsc_t *const crystal_frames[] = {
    &crystal_01, &crystal_02, &crystal_03};

static const struct left_playlist_entry left_playlist[] = {
    {left_image_frames, ARRAY_SIZE(left_image_frames), 0},
    {crystal_frames, ARRAY_SIZE(crystal_frames), 2400},
};
#define LEFT_CYCLE_COUNT ARRAY_SIZE(left_playlist)

