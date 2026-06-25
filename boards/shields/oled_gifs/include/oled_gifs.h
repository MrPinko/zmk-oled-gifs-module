#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * One animation: a flat array of frames and the metadata needed to play them.
 *
 * Each frame is a 1-bit-per-pixel XBM-style bitmap:
 *   width × height pixels, row-major, LSB first.
 *   Byte count per frame = ceil(width / 8) * height
 *
 * The display driver calls oled_gifs_set_active() once, then the timer
 * advances through frames automatically.
 */
typedef struct {
    const uint8_t *const *frames;  /* array of pointers, one per frame       */
    uint8_t        frame_count;    /* number of frames                        */
    uint8_t        width;          /* pixels wide                             */
    uint8_t        height;         /* pixels tall                             */
} gif_anim_t;

/**
 * Register a gif_anim_t so the driver can find it by Kconfig symbol name.
 * Place this macro in the GIF's .c file after the frames are defined.
 *
 * Usage:
 *   OLED_GIF_REGISTER(blink, &blink_anim);
 */
#define OLED_GIF_REGISTER(name, ptr)                                    \
    const gif_anim_t *__oled_gif_##name                                 \
        __attribute__((used, section(".oled_gifs"))) = (ptr)

/* ── driver API (called from your status_screen.c) ─────────────────────── */

/**
 * Set which animation is playing.  Pass NULL to blank the canvas.
 * Safe to call from the display thread.
 */
void oled_gifs_set_active(const gif_anim_t *anim);

/**
 * Call once from your custom status-screen init function.
 * Starts the frame-advance timer.
 */
void oled_gifs_init(struct lv_obj_t *canvas);
