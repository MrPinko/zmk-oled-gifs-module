cmake_minimum_required(VERSION 3.20.0)

if(CONFIG_OLED_GIFS)

  # Core animation driver (always needed when the feature is on)
  target_sources(app PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/oled_gifs.c
  )

  target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
  )

  # ── per-GIF frame data ────────────────────────────────────────────────────
  # Each GIF lives in its own .c file so only selected ones are compiled.

  if(CONFIG_OLED_GIF_BLINK)
    target_sources(app PRIVATE
      ${CMAKE_CURRENT_LIST_DIR}/src/gifs/blink.c
    )
  endif()

  # Add more blocks here as you add GIFs:
  # if(CONFIG_OLED_GIF_CAT)
  #   target_sources(app PRIVATE
  #     ${CMAKE_CURRENT_LIST_DIR}/src/gifs/cat.c
  #   )
  # endif()

endif()
