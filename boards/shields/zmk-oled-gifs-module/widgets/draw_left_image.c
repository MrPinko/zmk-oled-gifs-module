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

static lv_obj_t *left_art_parent;
static lv_obj_t *left_art_obj = NULL;
static int left_current_image_idx = 0;

static void show_current_entry(void)
{
    const struct left_playlist_entry *entry =
        &left_playlist[left_current_image_idx];

    if (left_art_obj != NULL)
    {
        lv_obj_del(left_art_obj);
        left_art_obj = NULL;
    }

    if (entry->frame_count == 1)
    {
        left_art_obj = lv_img_create(left_art_parent);
        lv_img_set_src(left_art_obj, entry->frames[0]);
    }
    else
    {
        left_art_obj = lv_animimg_create(left_art_parent);
        lv_animimg_set_src(left_art_obj, (const void **)entry->frames,
                           entry->frame_count);
        lv_animimg_set_duration(left_art_obj, entry->duration_ms);
        lv_animimg_set_repeat_count(left_art_obj, LV_ANIM_REPEAT_INFINITE);
        lv_animimg_start(left_art_obj);
    }

    lv_obj_align(left_art_obj, LV_ALIGN_TOP_LEFT, 0, 0);
}

/* ── Cycle event listener ────────────────────────────────────────────────── */

static void cycle_image_update_cb(struct zmk_oled_cycle_event ev)
{
    if (!left_art_obj || LEFT_CYCLE_COUNT <= 1)
    {
        return;
    }
    left_current_image_idx = (left_current_image_idx + 1) % LEFT_CYCLE_COUNT;
    LOG_INF("Cycling left OLED image to index %d", left_current_image_idx);

    show_current_entry();

    // lv_img_set_src(left_art_obj, left_cycle_images[left_current_image_idx]);
}

static struct zmk_oled_cycle_event cycle_image_get_state(const zmk_event_t *eh)
{
    return (struct zmk_oled_cycle_event){};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_left_image_cycle, struct zmk_oled_cycle_event,
                            cycle_image_update_cb, cycle_image_get_state);
ZMK_SUBSCRIPTION(widget_left_image_cycle, zmk_oled_cycle_event);

/* ── Public API ──────────────────────────────────────────────────────────── */

void draw_left_image(lv_obj_t *parent)
{

    left_art_parent = parent;
    show_current_entry();
    widget_left_image_cycle_init();
}
