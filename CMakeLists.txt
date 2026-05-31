# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT

zephyr_library()
zephyr_include_directories(include)
zephyr_include_directories(${ZEPHYR_ZMK_MODULE_DIR}/app/include)

if(CONFIG_ZMK_OLED_GIF)
    zephyr_linker_sources(RODATA linker/zmk_oled_gif.ld)

    target_sources(app PRIVATE src/behaviors/behavior_oled_gif_cycle.c)
    target_sources(app PRIVATE src/behaviors/gif_renderer.c)

    #if(CONFIG_ZMK_OLED_GIF_BONGO_CAT)
        target_sources(app PRIVATE src/gifs/bongo_cat.c)
        target_sources(app PRIVATE src/gifs/test2.c)
    #endif()

endif()