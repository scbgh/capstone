project(PHYSGAME)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
link_directories(${SDL_LIBRARY_DIR})

if(NOT SDL_FOUND)
    message("SDL not found!")
endif(NOT SDL_FOUND)

set(PHYSGAME_SRCS
    app.cpp
    file.cpp
    main.cpp
    math/color.cpp
    math/matrix.cpp
    math/point.cpp
    math/transform.cpp
    math/vector.cpp
    pack.cpp
    renderer.cpp
    world.cpp
)

if (MSVC)
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
	set(CMAKE_C_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_DEBUG_INIT} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS_DEBUG_INIT} /MD")
endif (MSVC)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(physgame ${PHYSGAME_SRCS})
target_link_libraries(physgame ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${BOX2D_LIBRARY})
