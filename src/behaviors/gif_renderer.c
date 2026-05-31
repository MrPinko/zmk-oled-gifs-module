/*
 * Copyright (c) 2024 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/matrix.h>
#include <zmk/keymap.h>


#include <zmk_oled_gif/gif.h>

LOG_MODULE_REGISTER(zmk_oled_gif, CONFIG_ZMK_LOG_LEVEL);

/* ── State ────────────────────────────────────────────────────────────── */

static int current_gif_idx   = 0;
static int current_frame_idx = 0;

/* ── Work queue item (display writes must NOT happen in ISR context) ──── */

static struct k_work  frame_advance_work;
static struct k_timer frame_timer;

/* ── Helpers ──────────────────────────────────────────────────────────── */

static int gif_count(void)
{
    int n = 0;
    STRUCT_SECTION_FOREACH(zmk_oled_gif, gif) { n++; }
    return n;
}

static const struct zmk_oled_gif *gif_get(int idx)
{
    int i = 0;
    STRUCT_SECTION_FOREACH(zmk_oled_gif, gif) {
        if (i == idx) return gif;
        i++;
    }
    return NULL;
}

/* ── Display ──────────────────────────────────────────────────────────── */

static void render_frame(const struct zmk_oled_gif *gif, int frame_idx)
{
    const struct device *display = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

    if (!device_is_ready(display)) {
        LOG_WRN("Display device not ready — skipping frame render");
        return;
    }

    /*
     * MONO01: 1 bit per pixel, 8 pixels per byte, row-major.
     * buf_size = width * height / 8 bytes.
     * pitch    = number of pixels in one row = width.
     */
    const struct display_buffer_descriptor desc = {
        .buf_size = (uint32_t)(gif->width * gif->height) / 8U,
        .width    = gif->width,
        .height   = gif->height,
        .pitch    = gif->width,
    };

    display_write(display, 0, 0, &desc, gif->frames[frame_idx]);
}

/* ── Timer / work ─────────────────────────────────────────────────────── */

static void frame_advance_work_handler(struct k_work *work)
{
    const struct zmk_oled_gif *gif = gif_get(current_gif_idx);
    if (!gif) return;

    current_frame_idx = (current_frame_idx + 1) % gif->frame_count;
    render_frame(gif, current_frame_idx);
}

/* Timer fires in ISR context — defer to the system work queue. */
static void frame_timer_callback(struct k_timer *timer)
{
    k_work_submit(&frame_advance_work);
}

/* ── Public API ───────────────────────────────────────────────────────── */

int zmk_oled_gif_current_index(void)
{
    return current_gif_idx;
}

int zmk_oled_gif_cycle(void)
{
    int total = gif_count();
    if (total == 0) {
        LOG_WRN("No GIFs registered — nothing to cycle");
        return -ENODEV;
    }

    /* Stop the running timer so we don't race with the work item. */
    k_timer_stop(&frame_timer);

    current_gif_idx   = (current_gif_idx + 1) % total;
    current_frame_idx = 0;

    const struct zmk_oled_gif *gif = gif_get(current_gif_idx);
    if (!gif) return -ENODEV;

    /* Render first frame immediately (we're already in thread context). */
    render_frame(gif, 0);

    /* Restart animation only for multi-frame GIFs. */
    if (gif->frame_count > 1) {
        k_timer_start(&frame_timer,
                      K_MSEC(gif->frame_delay_ms),
                      K_MSEC(gif->frame_delay_ms));
    }

    LOG_DBG("Cycled to GIF %d (%d frames @ %d ms)", current_gif_idx,
            gif->frame_count, gif->frame_delay_ms);

    return 0;
}

/* ── Init ─────────────────────────────────────────────────────────────── */

static int gif_renderer_init(void)
{
    k_work_init(&frame_advance_work, frame_advance_work_handler);
    k_timer_init(&frame_timer, frame_timer_callback, NULL);

    int total = gif_count();
    LOG_INF("GIF renderer init: %d GIF(s) registered", total);

    if (total == 0) {
        return 0;  /* no GIFs yet, nothing to start */
    }

    const struct zmk_oled_gif *gif = #include <stdlib.h>

#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h>

#include <drivers/behavior.h>

#include <zmk/behavior.h>
#include <zmk/event_manager.h>
#include <zmk/events/position_state_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>
#include <zmk/matrix.h>
#include <zmk/keymap.h>(0);
    if (!gif) return -ENODEV;

    render_frame(gif, 0);

    if (gif->frame_count > 1) {
        k_timer_start(&frame_timer,
                      K_MSEC(gif->frame_delay_ms),
                      K_MSEC(gif->frame_delay_ms));
    }

    return 0;
}

SYS_INIT(gif_renderer_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
