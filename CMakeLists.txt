# Copyright (c) 2024 The ZMK Contributors
# SPDX-License-Identifier: MIT


if(CONFIG_ZMK_OLED_GIF)
    target_sources(app PRIVATE src/behaviors/gif_renderer.c)
    target_sources(app PRIVATE src/behaviors/behavior_oled_gif_cycle.c)
endif()

if(CONFIG_ZMK_OLED_GIF_BONGO_CAT)
    target_sources(app PRIVATE src/gifs/bongo_cat.c)
    target_sources(app PRIVATE src/gifs/test2.c)
endif()

zephyr_include_directories(include)