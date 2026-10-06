/*
 * Copyright (c) 2024 Federico (MrPinko)
 * SPDX-License-Identifier: MIT
 *
 * Central (left) side screen widget implementation for 128×32 vertical OLED:
 *
 * Layout along the 128-pixel length:
 *   ┌──────────────────────┬────────────────────────────────────────────┐
 *   │  Top Status (32×32)  │  Left Artwork / Animated GIF (up to 96×32) │
 *   │  BT/USB + Battery    │  (under status canvas)                     │
 *   └──────────────────────┴────────────────────────────────────────────┘
 *   0                     32                                          128
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/usb.h>

#include "draw_left_image.h"
#include "battery.h"
#include "output.h"
#include "screen.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

/* ── Canvas draw helper ──────────────────────────────────────────────────── */

/**
 * Redraw the 32×32 status canvas: output (BT/USB) + battery.
 */
static void draw_top(lv_obj_t *widget, lv_color_t cbuf[], const struct status_state *state) {
    lv_obj_t *canvas = lv_obj_get_child(widget, 0);

    prepare_status_canvas(canvas);

    draw_output_status(canvas, state);
    draw_battery_status(canvas, state);

    rotate_canvas(canvas, cbuf);
}

/* ── Battery listener ────────────────────────────────────────────────────── */

static void set_battery_status(struct zmk_widget_screen *widget,
                               struct battery_status_state state) {
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
    widget->state.charging = state.usb_present;
#endif
    widget->state.battery = state.level;

    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_battery_status(widget, state);
    }
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_status_state){
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
        .usb_present = zmk_usb_is_powered(),
#endif
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);
#endif

/* ── Output / endpoint listener ──────────────────────────────────────────── */

static void set_output_status(struct zmk_widget_screen *widget,
                              const struct output_status_state *state) {
    widget->state.selected_endpoint        = state->selected_endpoint;
    widget->state.active_profile_index     = state->active_profile_index;
    widget->state.active_profile_connected = state->active_profile_connected;
    widget->state.active_profile_bonded    = state->active_profile_bonded;

    draw_top(widget->obj, widget->cbuf, &widget->state);
}

static void output_status_update_cb(struct output_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
        set_output_status(widget, &state);
    }
}

static struct output_status_state output_status_get_state(const zmk_event_t *_eh) {
    return (struct output_status_state){
        .selected_endpoint        = zmk_endpoints_selected(),
        .active_profile_index     = zmk_ble_active_profile_index(),
        .active_profile_connected = zmk_ble_active_profile_is_connected(),
        .active_profile_bonded    = !zmk_ble_active_profile_is_open(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, output_status_get_state);
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);
#if IS_ENABLED(CONFIG_USB_DEVICE_STACK)
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
#endif
#if defined(CONFIG_ZMK_BLE)
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);
#endif

/* ── Widget initialisation ───────────────────────────────────────────────── */

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    /* Resolution: SCREEN_HEIGHT (128) px long × SCREEN_WIDTH (32) px wide */
    lv_obj_set_size(widget->obj, SCREEN_HEIGHT, SCREEN_WIDTH);
    lv_obj_align(widget->obj, LV_ALIGN_TOP_LEFT, 0, 0);

    /* Status bar canvas placed at physical top of OLED */
    // lv_obj_t *top = lv_canvas_create(widget->obj);
    // lv_canvas_set_buffer(top, widget->cbuf, STATUS_BAR_SIZE, SCREEN_WIDTH,
    //                      LV_IMG_CF_TRUE_COLOR);
#if IS_ENABLED(CONFIG_NICE_OLED_FLIP)
    lv_obj_align(top, LV_ALIGN_TOP_LEFT, 0, 0);
#else
    lv_obj_align(top, LV_ALIGN_TOP_RIGHT, 0, 0);
#endif

    /* Artwork / GIF animation starts immediately below the status bar */
    draw_left_image(widget->obj);

    /* Register this widget instance and start the event-driven listeners. */
    sys_slist_append(&widgets, &widget->node);
    // widget_battery_status_init();
    // widget_output_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) {
    return widget->obj;
}
