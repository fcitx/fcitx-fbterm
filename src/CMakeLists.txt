
pkg_check_modules(GLIB2 "glib-2.0" REQUIRED)
pkg_check_modules(GIO2 "gio-2.0 gio-unix-2.0" REQUIRED)
pkg_check_modules(DBUS_GLIB "dbus-glib-1" REQUIRED)


_pkgconfig_invoke("glib-2.0" GLIB2 GLIB_GENMARSHAL "" "--variable=glib_genmarshal")

FIND_PROGRAM(GLIB_GENMARSHAL ${GLIB2_GLIB_GENMARSHAL})

add_custom_command(OUTPUT marshall.c
                   COMMAND ${GLIB_GENMARSHAL} --body --prefix=fcitx_marshall ${PROJECT_SOURCE_DIR}/src/marshall.list > marshall.c
)
add_custom_command(OUTPUT marshall.h
                   COMMAND ${GLIB_GENMARSHAL} --header --prefix=fcitx_marshall ${PROJECT_SOURCE_DIR}/src/marshall.list > marshall.h
)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
${GLIB2_INCLUDE_DIRS}
${GIO2_INCLUDE_DIRS}
${DBUS_GLIB_INCLUDE_DIRS}
${FCITX4_FCITX_INCLUDE_DIRS}
${FCITX4_FCITX_UTILS_INCLUDE_DIRS}
${FCITX4_FCITX_CONFIG_INCLUDE_DIRS}
)

link_directories(
${GLIB2_LIBRARY_DIRS}
${GIO2_LIBRARY_DIRS}
${DBUS_GLIB_LIBRARY_DIRS}
${FCITX4_FCITX_LIBRARY_DIRS}
${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
)

set(fcitx_fbterm_SOURCES
    marshall.c
    marshall.h
    client.c
    imapi.c
    fcitx-fbterm.c
    keycode.c
    keymap.c
)

add_executable(fcitx-fbterm ${fcitx_fbterm_SOURCES})
target_link_libraries(fcitx-fbterm
                      ${GLIB2_LIBRARIES}
                      ${DBUS_GLIB_LIBRARIES}
                      ${GIO2_LIBRARIES}
                      ${FCITX4_FCITX_LIBRARIES}
                      ${FCITX4_FCITX_CONFIG_LIBRARIES}
                      ${FCITX4_FCITX_UTILS_LIBRARIES}
                      )

install(TARGETS fcitx-fbterm DESTINATION ${bindir})
