# Simple vertical OLED artwork viewer

This module is a small ZMK custom status screen for a black 128×32 OLED mounted vertically. It follows the portrait-canvas approach used by [zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled): draw in a 32×128 portrait canvas, then rotate that canvas into the OLED's native 128×32 buffer.

The screen is black with white artwork/text. The battery is a separate page at the end of the cycle; it is not drawn over the images.

## What appears on each side

- The left firmware build uses `assets/left_frames.c` and `src/left_pages.c`.
- The right firmware build uses `assets/right_frames.c` and `src/right_pages.c`.
- Pressing the existing OLED cycle combo advances through that side's pages and then the battery page. The next press returns to the first image.
- Each side has its own page list, so left and right can show different pictures and animations.

## Add a still image

1. Convert the image to **32×128**, 1-bit indexed C data with LVGL's image converter. Compose it upright for a portrait screen.
2. Add the generated descriptor and data array to `assets/left_frames.c` or `assets/right_frames.c`.
3. In the matching `src/*_pages.c`, add `LV_IMG_DECLARE(your_image);`, a one-frame array, and one page entry:

   ```c
   LV_IMG_DECLARE(your_image);
   static const lv_img_dsc_t *const your_still[] = {&your_image};

   /* Add to oled_image_pages: */
   {.frames = your_still, .frame_count = 1, .frame_interval_ms = 0},
   ```

The viewer checks that every frame is exactly 32×128, so an incorrectly sized image shows an error instead of being silently cropped.

## Add an animation

Put each 32×128 frame descriptor in the side's `*_frames.c` file, then add the frame list as one page. The interval is the delay between frames in milliseconds:

```c
LV_IMG_DECLARE(frame_0);
LV_IMG_DECLARE(frame_1);
static const lv_img_dsc_t *const walk[] = {&frame_0, &frame_1};

/* Add to oled_image_pages: */
{.frames = walk, .frame_count = 2, .frame_interval_ms = 120},
```

The keyboard combo changes pages; an animation advances through its frames automatically while its page is visible.

## OLED direction

The default rotation is clockwise. If the display is mounted the other way, set `CONFIG_NICE_OLED_ROTATE_270=y` in that side's `.conf` file. Set it independently in the left and right build configs if their OLEDs face different directions.
