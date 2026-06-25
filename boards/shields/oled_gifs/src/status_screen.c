/*
 * status_screen.c — custom ZMK status screen using oled_gifs
 *
 * USAGE
 * -----
 * Copy this file into your keyboard's shield folder (e.g.
 *   config/boards/shields/lily58/status_screen.c).
 * It replaces ZMK's built-in status screen.
 *
 * Your .conf must contain:
 *   CONFIG_ZMK_DISPLAY=y
 *   CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y
 *   CONFIG_OLED_GIFS=y
 *   CONFIG_OLED_GIF_BLINK=y          # or whatever GIF you want
 *
 * Your west.yml must pull in this module:
 *   - name: zmk-oled-gifs
 *     remote: <your-remote>
 *     revision: main
 */

#include <zephyr/kernel.h>
#include <lvgl.h>

#include <zmk/display.h>            /* zmk_display_status_screen()           */

#include "oled_gifs.h"

/* Pull in the GIF you want — only compiled if its Kconfig is enabled */
#if IS_ENABLED(CONFIG_OLED_GIF_BLINK)
extern const gif_anim_t *const gif_blink;
#endif

/* ── screen setup ─────────────────────────────────────────────────────────── */

lv_obj_t *zmk_display_status_screen(void)
{
    /* Root screen object */
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);

    /* Canvas that the GIF driver will paint into */
    lv_obj_t *canvas = lv_canvas_create(screen);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);

    /* Start the animation engine */
    oled_gifs_init(canvas);

    /* Select which GIF plays on boot */
#if IS_ENABLED(CONFIG_OLED_GIF_BLINK)
    oled_gifs_set_active(gif_blink);
#endif

    return screen;
}
