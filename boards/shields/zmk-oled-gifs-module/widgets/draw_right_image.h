/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Right (peripheral) side artwork display.
 *
 * draw_right_image()     – draws the first frame as a static image.
 * draw_right_animation() – drives lv_animimg to loop through all GIF frames.
 *
 * Which function is active is controlled by CONFIG_NICE_RIGHT_ANIMATION:
 *   y (default) → animation
 *   n           → static first frame
 *
 * The image frames are declared in assets/right_image.c.
 *
 * HOW TO ADD MORE FRAMES (GIF animation):
 *   1. Convert each GIF frame to a C array with the LVGL Image Converter
 *      (https://lvgl.io/tools/imageconverter), choose CF_INDEXED_1BIT.
 *   2. Add each lv_img_dsc_t declaration to assets/right_image.c and give
 *      each symbol a unique name, e.g. right_image_frame_0, right_image_frame_1…
 *   3. Add LV_IMG_DECLARE() lines below for each new symbol.
 *   4. Add the symbol pointer to anim_imgs[].
 *   5. Update RIGHT_ANIM_FRAME_COUNT.
 */

#pragma once

#include <lvgl.h>

/** Draw the right-side artwork (static or animated, per Kconfig). */
void draw_right_image(lv_obj_t *parent);
