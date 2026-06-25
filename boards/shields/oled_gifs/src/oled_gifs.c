/*
 * oled_gifs.c — minimal OLED GIF animation driver for ZMK
 *
 * Approach
 * --------
 * - A k_timer fires every CONFIG_OLED_GIF_FRAME_PERIOD_MS milliseconds.
 * - The timer ISR only submits a k_work item (ISR-safe).
 * - The work handler runs in the system work-queue, advances the frame
 *   counter, and redraws the LVGL canvas.
 * - No LVGL calls ever happen inside the ISR.
 *
 * Split keyboards
 * ---------------
 * Run this on whichever side has the OLED.  If you want the same animation
 * on the peripheral, add this shield to the peripheral build too — the
 * animation runs independently on each side, which is fine for decorative
 * GIFs.  If you need synchronised state (e.g. react to keypresses on the
 * other half) you can hook into ZMK's split listener API separately; that
 * complexity is intentionally out of scope here.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>

#include "oled_gifs.h"

LOG_MODULE_REGISTER(oled_gifs, LOG_LEVEL_INF);

/* ── state ────────────────────────────────────────────────────────────────── */

static lv_obj_t        *g_canvas      = NULL;
static const gif_anim_t *g_anim       = NULL;
static uint8_t          g_frame_idx   = 0;

/* LVGL needs a back-buffer for the canvas.
 * Size = ceil(MAX_W/8) * MAX_H bytes.
 * 128×64 is the largest common OLED; adjust if yours differs. */
#define CANVAS_BUF_SIZE LV_CANVAS_BUF_SIZE_INDEXED_1BIT(128, 64)
static uint8_t g_canvas_buf[CANVAS_BUF_SIZE];

/* ── forward declarations ─────────────────────────────────────────────────── */
static void frame_timer_cb(struct k_timer *timer);
static void frame_work_cb(struct k_work *work);

static K_TIMER_DEFINE(g_frame_timer, frame_timer_cb, NULL);
static K_WORK_DEFINE(g_frame_work, frame_work_cb);

/* ── internal helpers ─────────────────────────────────────────────────────── */

static void draw_frame(void)
{
    if (!g_canvas || !g_anim || g_anim->frame_count == 0) {
        return;
    }

    const uint8_t *frame = g_anim->frames[g_frame_idx];

    /* Clear canvas */
    lv_canvas_fill_bg(g_canvas, lv_color_black(), LV_OPA_COVER);

    /* Draw every set pixel.
     * XBM layout: row-major, LSB = leftmost pixel in each byte. */
    uint8_t bytes_per_row = (g_anim->width + 7) / 8;

    for (uint8_t y = 0; y < g_anim->height; y++) {
        for (uint8_t bx = 0; bx < bytes_per_row; bx++) {
            uint8_t byte = frame[y * bytes_per_row + bx];
            for (uint8_t bit = 0; bit < 8; bit++) {
                uint8_t x = bx * 8 + bit;
                if (x >= g_anim->width) break;
                if (byte & (1u << bit)) {
                    lv_canvas_set_px_color(g_canvas, x, y, lv_color_white());
                }
            }
        }
    }
}

/* ── timer / work callbacks ───────────────────────────────────────────────── */

static void frame_timer_cb(struct k_timer *timer)
{
    k_work_submit(&g_frame_work);
}

static void frame_work_cb(struct k_work *work)
{
    if (!g_anim || g_anim->frame_count == 0) {
        return;
    }

    g_frame_idx = (g_frame_idx + 1) % g_anim->frame_count;
    draw_frame();
}

/* ── public API ───────────────────────────────────────────────────────────── */

void oled_gifs_set_active(const gif_anim_t *anim)
{
    g_anim      = anim;
    g_frame_idx = 0;
    draw_frame();
}

void oled_gifs_init(lv_obj_t *canvas)
{
    g_canvas = canvas;

    lv_canvas_set_buffer(g_canvas, g_canvas_buf,
                         128, 64,          /* adjust to your OLED size */
                         LV_IMG_CF_INDEXED_1BIT);
    lv_canvas_fill_bg(g_canvas, lv_color_black(), LV_OPA_COVER);

    LOG_INF("oled_gifs: init done, frame period %d ms",
            CONFIG_OLED_GIF_FRAME_PERIOD_MS);

    k_timer_start(&g_frame_timer,
                  K_MSEC(CONFIG_OLED_GIF_FRAME_PERIOD_MS),
                  K_MSEC(CONFIG_OLED_GIF_FRAME_PERIOD_MS));
}
