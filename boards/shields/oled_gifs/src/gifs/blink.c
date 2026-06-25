/*
 * blink.c — minimal 2-frame test animation for CONFIG_OLED_GIF_BLINK
 *
 * Frame size: 16×16 pixels, XBM layout (LSB = leftmost pixel per byte).
 *
 * Frame 0: filled  (all pixels on)
 * Frame 1: empty   (all pixels off)
 *
 * Replace or supplement this file once you have your real GIF data.
 * To convert a real GIF:
 *   1. Export each frame as a 1-bit PNG/BMP at your OLED resolution.
 *   2. Use ImageMagick:
 *        convert frame0.png -depth 1 xbm:- | grep "0x" | tr -d ' \n'
 *      to get the byte array for each frame.
 *   3. Fill in the arrays below and update BLINK_W / BLINK_H.
 */

#include "oled_gifs.h"

#define BLINK_W 16
#define BLINK_H 16

/* 16 px wide → 2 bytes per row → 2 × 16 = 32 bytes per frame */

static const uint8_t frame0[32] = {
    /* all pixels ON */
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
    0xFF, 0xFF,
};

static const uint8_t frame1[32] = {
    /* all pixels OFF */
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};

static const uint8_t *const blink_frames[] = { frame0, frame1 };

static const gif_anim_t blink_anim = {
    .frames      = blink_frames,
    .frame_count = 2,
    .width       = BLINK_W,
    .height      = BLINK_H,
};

OLED_GIF_REGISTER(blink, &blink_anim);

/* Expose for direct use: oled_gifs_set_active(&gif_blink); */
const gif_anim_t *const gif_blink = &blink_anim;
