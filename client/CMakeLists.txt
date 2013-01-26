cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (CMAKE_COMPILER_IS_CLANGXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif (CMAKE_COMPILER_IS_CLANGXX)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
link_directories(${SDL_LIBRARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(util)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

add_custom_command(
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat
    COMMAND packer ARGS ${CMAKE_CURRENT_SOURCE_DIR}/pack ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat
    DEPENDS packer
    VERBATIM
)

add_custom_target(gamedata ALL DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat)