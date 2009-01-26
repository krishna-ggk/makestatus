project(makeinvoker)

# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set (makeinvoker_SRCS
    makeinvoker.h
    makeinvoker.cpp
    main.cpp
)

kde4_add_executable(makeinvoker ${makeinvoker_SRCS})
target_link_libraries(makeinvoker ${KDE4_KDEUI_LIBS})


install(TARGETS makeinvoker DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES makeinvoker.notifyrc DESTINATION ${DATA_INSTALL_DIR}/makeinvoker)
