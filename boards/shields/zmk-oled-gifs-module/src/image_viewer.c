#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/events/battery_state_changed.h>

#include "events/oled_cycle_event.h"
#include "image_viewer.h"

LOG_MODULE_REGISTER(zmk_oled_viewer, CONFIG_ZMK_LOG_LEVEL);

#define PORTRAIT_WIDTH  32
#define PORTRAIT_HEIGHT 128
#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  32
#define BATTERY_VIEW    oled_image_page_count

/* The panel is physically vertical; LVGL still draws into its 128x32 buffer. */
/* These offsets move LVGL's centered rotation into the full 128x32 canvas. */
#if IS_ENABLED(CONFIG_NICE_OLED_ROTATE_270)
#define VIEW_ROTATION 2700
#define VIEW_OFFSET_X 48
#define VIEW_OFFSET_Y -49
#else
#define VIEW_ROTATION 900
#define VIEW_OFFSET_X 47
#define VIEW_OFFSET_Y -48
#endif

static lv_obj_t *portrait_canvas;
static lv_obj_t *display_canvas;
static lv_color_t portrait_buffer[PORTRAIT_WIDTH * PORTRAIT_HEIGHT];
static lv_color_t display_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

static uint8_t current_view;
static uint8_t current_frame;
static uint8_t battery_level;
static lv_timer_t *animation_timer;

static void rotate_portrait_to_display(void) {
    lv_canvas_fill_bg(display_canvas, lv_color_black(), LV_OPA_COVER);

    lv_canvas_transform(display_canvas, lv_canvas_get_img(portrait_canvas), VIEW_ROTATION,
                        LV_IMG_ZOOM_NONE, VIEW_OFFSET_X, VIEW_OFFSET_Y,
                        PORTRAIT_WIDTH / 2, PORTRAIT_HEIGHT / 2, false);
}

static void draw_image_frame(const lv_img_dsc_t *image) {
    lv_canvas_fill_bg(portrait_canvas, lv_color_black(), LV_OPA_COVER);

    if (image->header.w != PORTRAIT_WIDTH || image->header.h != PORTRAIT_HEIGHT) {
        LOG_ERR("OLED image must be 32x128; got %u x %u", image->header.w, image->header.h);
        rotate_portrait_to_display();
        return;
    }

    lv_draw_img_dsc_t draw;
    lv_draw_img_dsc_init(&draw);
    lv_canvas_draw_img(portrait_canvas, 0, 0, image, &draw);
    rotate_portrait_to_display();
}

static void draw_battery_view(void) {
    lv_canvas_fill_bg(portrait_canvas, lv_color_black(), LV_OPA_COVER);

    lv_draw_label_dsc_t label;
    lv_draw_label_dsc_init(&label);
    label.color = lv_color_white();
    label.font = &lv_font_unscii_8;
    label.align = LV_TEXT_ALIGN_CENTER;

    char percentage[5];
    snprintf(percentage, sizeof(percentage), "%u%%", (unsigned)battery_level);
    lv_canvas_draw_text(portrait_canvas, 0, 54, PORTRAIT_WIDTH, &label, "BAT");
    lv_canvas_draw_text(portrait_canvas, 0, 68, PORTRAIT_WIDTH, &label, percentage);
    rotate_portrait_to_display();
}

static void stop_animation(void) {
    if (animation_timer != NULL) {
        lv_timer_del(animation_timer);
        animation_timer = NULL;
    }
}

static void animation_tick(lv_timer_t *timer) {
    ARG_UNUSED(timer);

    const struct oled_image_page *page = &oled_image_pages[current_view];
    current_frame = (current_frame + 1) % page->frame_count;
    draw_image_frame(page->frames[current_frame]);
}

static void draw_current_view(void) {
    stop_animation();

    if (current_view == BATTERY_VIEW) {
        draw_battery_view();
        return;
    }

    const struct oled_image_page *page = &oled_image_pages[current_view];
    if (page->frames == NULL || page->frame_count == 0) {
        LOG_ERR("OLED image page %u has no frames", current_view);
        lv_canvas_fill_bg(display_canvas, lv_color_black(), LV_OPA_COVER);
        return;
    }

    current_frame = 0;
    draw_image_frame(page->frames[current_frame]);

    if (page->frame_count > 1 && page->frame_interval_ms > 0) {
        animation_timer = lv_timer_create(animation_tick, page->frame_interval_ms, NULL);
    }
}

static void cycle_image_update_cb(struct zmk_oled_cycle_event event) {
    ARG_UNUSED(event);

    if (oled_image_page_count == 0) {
        current_view = BATTERY_VIEW;
    } else {
        current_view = (current_view + 1) % (oled_image_page_count + 1);
    }
    draw_current_view();
}

static struct zmk_oled_cycle_event cycle_image_get_state(const zmk_event_t *event) {
    ARG_UNUSED(event);
    return (struct zmk_oled_cycle_event){};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_oled_image_cycle, struct zmk_oled_cycle_event,
                            cycle_image_update_cb, cycle_image_get_state);
ZMK_SUBSCRIPTION(widget_oled_image_cycle, zmk_oled_cycle_event);

struct battery_view_state {
    uint8_t level;
};

static struct battery_view_state battery_view_get_state(const zmk_event_t *event) {
    const struct zmk_battery_state_changed *battery = as_zmk_battery_state_changed(event);
    return (struct battery_view_state){
        .level = battery != NULL ? battery->state_of_charge : zmk_battery_state_of_charge(),
    };
}

static void battery_view_update_cb(struct battery_view_state battery) {
    battery_level = battery.level;
    if (current_view == BATTERY_VIEW) {
        draw_battery_view();
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_oled_battery_view, struct battery_view_state,
                            battery_view_update_cb, battery_view_get_state);
ZMK_SUBSCRIPTION(widget_oled_battery_view, zmk_battery_state_changed);

void oled_image_viewer_init(lv_obj_t *screen) {
    portrait_canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(portrait_canvas, portrait_buffer, PORTRAIT_WIDTH, PORTRAIT_HEIGHT,
                         LV_IMG_CF_TRUE_COLOR);
    lv_obj_add_flag(portrait_canvas, LV_OBJ_FLAG_HIDDEN);

    display_canvas = lv_canvas_create(screen);
    lv_canvas_set_buffer(display_canvas, display_buffer, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                         LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_size(display_canvas, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_align(display_canvas, LV_ALIGN_TOP_LEFT, 0, 0);

    widget_oled_image_cycle_init();
    widget_oled_battery_view_init();
    draw_current_view();
}
