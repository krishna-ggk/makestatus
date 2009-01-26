project(plasma-applet-makestatus)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set(makestatus_applet_SRCS makestatusapplet.cpp)
kde4_add_plugin(plasma_applet_makestatus ${makestatus_applet_SRCS})
target_link_libraries(plasma_applet_makestatus ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_makestatus DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-makestatus.desktop DESTINATION ${SERVICES_INSTALL_DIR})
