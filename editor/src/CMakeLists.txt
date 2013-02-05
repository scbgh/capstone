project(PGEDITOR)

find_package(Qt4 REQUIRED)

set(PGEDITOR_SRCS
    commands.cpp
    hasproperties.cpp
    main.cpp
    mainwindow.cpp
    mapscene.cpp
    propertyitemmodel.cpp
    editorwidgets/pointeditor.cpp
    sceneitems/circleshapeitem.cpp
    sceneitems/polygonshapeitem.cpp
    sceneitems/shapeitem.cpp
)

set (PGEDITOR_MOCS
    hasproperties.h
    mainwindow.h
    mapscene.h
    editorwidgets/pointeditor.h
    propertyitemmodel.h
    sceneitems/shapeitem.h
)

qt4_wrap_cpp(PGEDITOR_MOCS_SRCS ${PGEDITOR_MOCS})
qt4_add_resources(PGEDITOR_RCC_SRCS "resources.qrc")

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
add_executable(pgedit ${PGEDITOR_SRCS} ${PGEDITOR_MOCS_SRCS} ${PGEDITOR_RCC_SRCS})
target_link_libraries(pgedit ${QT_LIBRARIES})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})