project(PHYSGAME)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
link_directories(${SDL_LIBRARY_DIR})

set(PHYSGAME_SRCS
    main.cpp
    app.cpp
    renderer.cpp
    math/color.cpp
    math/matrix.cpp
    math/point.cpp
    math/transform.cpp
    math/vector.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(physgame ${PHYSGAME_SRCS})
target_link_libraries(physgame ${SDL_LIBRARY} ${OPENGL_LIBRARIES})
