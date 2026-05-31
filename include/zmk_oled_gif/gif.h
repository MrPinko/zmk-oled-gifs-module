/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>
#include <zephyr/sys/iterable_sections.h>

/**
 * @brief Describes a single GIF animation stored in flash.
 *
 * Frames are 1-bit-per-pixel monochrome bitmaps in MONO01 format
 * (the default for SSD1306 in ZMK), packed 8 pixels per byte, row-major.
 *
 * Frame byte sizes:
 *   128x32 OLED → (128 * 32) / 8 = 512 bytes per frame
 *   128x64 OLED → (128 * 64) / 8 = 1024 bytes per frame
 */
struct zmk_oled_gif {
    /** Array of pointers to individual frame bitmaps (stored in flash). */
    const uint8_t *const *frames;

    /** Delay between frames in milliseconds (e.g. 100 = 10 fps). */
    uint16_t frame_delay_ms;

    /** Number of frames in this animation. */
    uint8_t frame_count;

    /** Frame width in pixels (typically 128). */
    uint8_t width;

    /** Frame height in pixels (typically 32 or 64). */
    uint8_t height;
};

/**
 * @brief Register a GIF so the renderer discovers it at boot.
 *
 * Uses Zephyr's iterable sections — all registered GIFs are linked
 * into a dedicated flash section and iterated at runtime without a
 * dynamic list.
 *
 * @param _name  C identifier for this GIF instance (must be unique).
 * @param ...    Designated initializer for struct zmk_oled_gif.
 *
 * Example (in your gifs/my_animation.c):
 *
 *   #include <zmk_oled_gif/gif.h>
 *   #include "my_animation_frames.h"   // defines frame byte arrays
 *
 *   static const uint8_t *my_frames[] = {
 *       frame_00, frame_01, frame_02, frame_03,
 *   };
 *
 *   ZMK_OLED_GIF_DEFINE(my_animation, {
 *       .frames          = my_frames,
 *       .frame_count     = ARRAY_SIZE(my_frames),
 *       .frame_delay_ms  = 100,
 *       .width           = 128,
 *       .height          = 32,
 *   });
 */
#define ZMK_OLED_GIF_DEFINE(_name, ...)                              \
    STRUCT_SECTION_ITERABLE(zmk_oled_gif, _name) = __VA_ARGS__

/**
 * @brief Advance to the next registered GIF.
 *
 * Stops the current frame timer, resets to frame 0 of the next GIF,
 * renders it immediately, then restarts the timer.
 * Wraps around after the last registered GIF.
 *
 * Called by the behavior on both halves independently.
 *
 * @return 0 on success, -ENODEV if no GIFs are registered.
 */
int zmk_oled_gif_cycle(void);

/**
 * @brief Return the index of the currently active GIF (0-based).
 */
int zmk_oled_gif_current_index(void);
