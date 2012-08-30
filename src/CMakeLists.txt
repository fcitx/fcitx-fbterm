include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${FCITX4_FCITX_INCLUDE_DIRS}
${FCITX4_FCITX_UTILS_INCLUDE_DIRS}
${FCITX4_FCITX_CONFIG_INCLUDE_DIRS}
${FCITX_GCLIENT_INCLUDE_DIRS}
)

link_directories(
${FCITX4_FCITX_LIBRARY_DIRS}
${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
${FCITX_GCLIENT_LIBRARY_DIRS}
)

set(fcitx_fbterm_SOURCES
    imapi.c
    fcitx-fbterm.c
    keycode.c
    keymap.c
)

add_executable(fcitx-fbterm ${fcitx_fbterm_SOURCES})
target_link_libraries(fcitx-fbterm
                      ${FCITX_GCLIENT_LIBRARIES}
                      ${FCITX4_FCITX_LIBRARIES}
                      ${FCITX4_FCITX_CONFIG_LIBRARIES}
                      ${FCITX4_FCITX_UTILS_LIBRARIES}
                      )

install(TARGETS fcitx-fbterm DESTINATION ${bindir})
