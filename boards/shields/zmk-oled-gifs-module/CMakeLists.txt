# Copyright (c) 2024 Federico (MrPinko)
# SPDX-License-Identifier: MIT
#
# CMakeLists.txt for the zmk-oled-gifs-module shield.
#
# Source files are split by role:
#   - Always compiled (both sides): custom_status_screen.c, shared assets & widgets
#   - Central (left) only:  screen.c, profile.c, left artwork & animation
#   - Peripheral (right) only: screen_peripheral.c, right artwork & animation

if(CONFIG_ZMK_DISPLAY)
  zephyr_library()
  zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
  zephyr_library_include_directories(include)

  # ── Shared sources (compiled on both central and peripheral) ──────────────
  zephyr_library_sources(
    custom_status_screen.c
    assets/images.c
    widgets/battery.c
    widgets/output.c
    widgets/util.c
  )

  # ── Central (left) side ───────────────────────────────────────────────────
  if(NOT CONFIG_ZMK_SPLIT OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    zephyr_library_sources(
      assets/left_image.c
      widgets/screen.c
      widgets/draw_left_image.c
    )
  # ── Peripheral (right) side ───────────────────────────────────────────────
  else()
    zephyr_library_sources(
      assets/right_image.c
      widgets/screen_peripheral.c
      widgets/draw_right_image.c
    )
  endif()
endif()
