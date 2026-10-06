#include <lvgl.h>

#include "image_viewer.h"

LV_IMG_DECLARE(left_image_1);
LV_IMG_DECLARE(left_image_2);

static const lv_img_dsc_t *const left_image_1_frames[] = {
    &left_image_1,
};

static const lv_img_dsc_t *const left_image_2_frames[] = {
    &left_image_2,
};

/* Add each still image or animation as one entry in this list. */
const struct oled_image_page oled_image_pages[] = {
    {.frames = left_image_1_frames, .frame_count = 1, .frame_interval_ms = 0},
    {.frames = left_image_2_frames, .frame_count = 1, .frame_interval_ms = 0},
};

const uint8_t oled_image_page_count = sizeof(oled_image_pages) / sizeof(oled_image_pages[0]);
