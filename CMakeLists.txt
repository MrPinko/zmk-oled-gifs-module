# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT


if(CONFIG_ZMK_OLED_GIF)
    zephyr_library_sources(src/behaviors/gif_renderer.c)
    zephyr_library_sources(src/behaviors/behavior_oled_gif_cycle.c)
endif()

if(CONFIG_ZMK_OLED_GIF_BONGO_CAT)
    zephyr_library_sources(src/gifs/bongo_cat.c)
    zephyr_library_sources(src/gifs/test2.c)
endif()

zephyr_include_directories(include)