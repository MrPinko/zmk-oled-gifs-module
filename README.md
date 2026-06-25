# zmk-oled-gifs

Minimal ZMK external module for looping frame animations on an OLED display.
No widgets, no WPM, no HID — just GIFs.

## Directory layout

```
zmk-oled-gifs/
├── zephyr/module.yml                        ← ZMK module entry-point
├── Kconfig                                  ← top-level Kconfig (sources shield one)
├── CMakeLists.txt                           ← top-level (minimal)
└── boards/shields/oled_gifs/
    ├── Kconfig                              ← CONFIG_OLED_GIFS + per-GIF bools
    ├── CMakeLists.txt                       ← conditionally compiles GIF sources
    ├── include/
    │   └── oled_gifs.h                      ← gif_anim_t, OLED_GIF_REGISTER, API
    └── src/
        ├── oled_gifs.c                      ← k_timer + k_work + LVGL canvas driver
        ├── status_screen.c                  ← template — copy into your keyboard shield
        └── gifs/
            └── blink.c                      ← built-in 2-frame blink test
```

## How it works

1. `oled_gifs_init(canvas)` — called once from your `status_screen.c`, starts a
   `k_timer` that ticks every `CONFIG_OLED_GIF_FRAME_PERIOD_MS` ms.
2. The timer ISR submits a `k_work` item (never touches LVGL directly).
3. The work handler advances the frame counter and redraws the LVGL canvas using
   the XBM byte arrays stored in each GIF's `.c` file.
4. `oled_gifs_set_active(anim)` — swap which animation is playing at runtime.

## Adding a new GIF

### 1. Convert your frames to XBM byte arrays

```bash
# For each frame (frame0.png, frame1.png, …):
convert frame0.png -resize 32x128 -depth 1 -monochrome xbm:- \
  | grep "0x" | tr -d ' \n' > frame0_bytes.txt
```

ImageMagick writes LSB-first XBM data, which matches what the driver expects.

### 2. Create `src/gifs/mycat.c`

```c
#include "oled_gifs.h"

#define CAT_W 32
#define CAT_H 32   /* adjust to your frame dimensions */

static const uint8_t frame0[CAT_H * ((CAT_W + 7) / 8)] = {
    /* paste bytes from frame0_bytes.txt */
    0x00, 0x3C, ...
};

static const uint8_t frame1[CAT_H * ((CAT_W + 7) / 8)] = {
    /* paste bytes from frame1_bytes.txt */
    ...
};

static const uint8_t *const cat_frames[] = { frame0, frame1 };

static const gif_anim_t cat_anim = {
    .frames      = cat_frames,
    .frame_count = 2,
    .width       = CAT_W,
    .height      = CAT_H,
};

OLED_GIF_REGISTER(cat, &cat_anim);
const gif_anim_t *const gif_cat = &cat_anim;
```

### 3. Add a Kconfig option in `boards/shields/oled_gifs/Kconfig`

```kconfig
config OLED_GIF_CAT
    bool "Enable cat animation"
    default n
```

### 4. Add it to `CMakeLists.txt`

```cmake
if(CONFIG_OLED_GIF_CAT)
  target_sources(app PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/gifs/cat.c
  )
endif()
```

### 5. Use it in your `status_screen.c`

```c
#if IS_ENABLED(CONFIG_OLED_GIF_CAT)
extern const gif_anim_t *const gif_cat;
oled_gifs_set_active(gif_cat);
#endif
```

### 6. Enable it in your `.conf`

```
CONFIG_OLED_GIF_CAT=y
```

That's it. Every GIF that isn't enabled is never compiled, so flash stays lean.

## Integration quick-start

See `INTEGRATION.conf.example` for the full `west.yml`, `.conf`, and `build.yaml`
snippets needed to wire this into your keyboard config.

The `src/status_screen.c` in this module is a **template** — copy it into your
keyboard's shield folder (where your `lily58_left.conf` lives) and adjust it
there. Your local copy overrides ZMK's built-in status screen because
`CONFIG_ZMK_DISPLAY_STATUS_SCREEN_CUSTOM=y` tells ZMK to look for a local
`zmk_display_status_screen()` instead.

## Notes

- Frame pixel format: 1-bit XBM, row-major, LSB = leftmost pixel per byte.
- The LVGL canvas buffer in `oled_gifs.c` is sized for 128×64. Shrink
  `CANVAS_BUF_SIZE` if your OLED is smaller to save RAM.
- On a split keyboard both halves run the animation independently. If you only
  want it on one side, add the `oled_gifs` shield only to that build target.
