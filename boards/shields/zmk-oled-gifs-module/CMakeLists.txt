if(CONFIG_ZMK_DISPLAY)
  zephyr_library()
  zephyr_library_include_directories(include)

  zephyr_library_sources(
    src/custom_status_screen.c
    src/image_viewer.c
  )

  if(NOT CONFIG_ZMK_SPLIT OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    zephyr_library_sources(assets/left_frames.c src/left_pages.c)
  else()
    zephyr_library_sources(assets/right_frames.c src/right_pages.c)
  endif()
endif()
