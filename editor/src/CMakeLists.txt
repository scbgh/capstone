project(PGEDITOR)

find_package(Qt4 REQUIRED)

set(PGEDITOR_SOURCES
    main.cpp
    mainwindow.cpp
)

set (PGEDITOR_HEADERS
    mainwindow.h
)

qt4_wrap_cpp(PGEDITOR_HEADERS_MOC ${PGEDITOR_HEADERS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
add_executable(pgedit ${PGEDITOR_SOURCES} ${PGEDITOR_HEADERS_MOC})
target_link_libraries(pgedit ${QT_LIBRARIES})