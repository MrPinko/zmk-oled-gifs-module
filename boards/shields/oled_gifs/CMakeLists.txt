if(CONFIG_ZMK_DISPLAY)

    zephyr_library()
    file(GLOB font_sources src/fonts/*.c)
    zephyr_library_include_directories(${ZEPHYR_LVGL_MODULE_DIR})
    zephyr_library_include_directories(${ZEPHYR_BASE}/lib/gui/lvgl/)
    zephyr_library_include_directories(${ZEPHYR_BASE}/drivers)
    zephyr_library_include_directories(${CMAKE_SOURCE_DIR}/include)
    zephyr_library_include_directories(${ZEPHYR_CURRENT_MODULE_DIR}/include)
    zephyr_library_include_directories(${ZEPHYR_CURRENT_CMAKE_DIR}/include)
    zephyr_library_include_directories(include)
    zephyr_library_sources(${font_sources})


    zephyr_library_sources(custom_status_screen.c)
    zephyr_library_sources(assets/images.c)

    zephyr_library_sources(widgets/battery.c)
    zephyr_library_sources(widgets/output.c)
    zephyr_library_sources(widgets/util.c)


    target_sources_ifdef(CONFIG_NICE_OLED_WIDGET_ANIMATION_PERIPHERAL_CAT app PRIVATE assets/cat.c)
    
    zephyr_library_sources(widgets/animation.c)
    zephyr_library_sources(widgets/screen_peripheral.c)
endif()