#pragma once

#include <lvgl.h>

/* One page is a still image (one frame) or an animation (multiple frames). */
struct oled_image_page {
    const lv_img_dsc_t *const *frames;
    uint8_t frame_count;
    uint16_t frame_interval_ms;
};

extern const struct oled_image_page oled_image_pages[];
extern const uint8_t oled_image_page_count;

void oled_image_viewer_init(lv_obj_t *screen);
