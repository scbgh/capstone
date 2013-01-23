project(PGEDITOR)

find_package(Qt4 REQUIRED)

set(PGEDITOR_SRCS
    main.cpp
    mainwindow.cpp
)

set (PGEDITOR_MOCS
    mainwindow.h
    propertybrowser.h
    mapscene.h
)

qt4_wrap_cpp(PGEDITOR_MOCS_SRCS ${PGEDITOR_MOCS})
qt4_add_resources(PGEDITOR_RCC_SRCS "resources.qrc")

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
add_executable(pgedit ${PGEDITOR_SRCS} ${PGEDITOR_MOCS_SRCS} ${PGEDITOR_RCC_SRCS})
target_link_libraries(pgedit ${QT_LIBRARIES})