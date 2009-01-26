project(plasma-dataengine-makestatus)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

# We add our source code here
set(makestatus_engine_SRCS makestatusengine.cpp)

QT4_ADD_DBUS_ADAPTOR(makestatus_engine_SRCS org.MakeStatusEngine.xml
                     makestatusengine.h MakeStatusEngine)

# Now make sure all files get to the right place
kde4_add_plugin(plasma_engine_makestatus ${makestatus_engine_SRCS})
target_link_libraries(plasma_engine_makestatus
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_PLASMA_LIBS})

install(TARGETS plasma_engine_makestatus
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-engine-makestatus.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES org.MakeStatusEngine.xml
        DESTINATION ${KDE4_DBUS_INTERFACES_DIR})
