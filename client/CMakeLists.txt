cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../modules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message("Using Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Box2D REQUIRED)
find_package(Lua51 REQUIRED)
find_package(Tolua++ REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(${BOX2D_INCLUDE_DIR})
include_directories(${LUA_INCLUDE_DIR})
include_directories(${TOLUA++_INCLUDE_DIR})
link_directories(${SDL_LIBRARY_DIR})

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif(DOXYGEN_FOUND)

set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${THIRDPARTY_DIR})
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(util)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

add_custom_command(
    OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat dummy
    COMMAND packer ARGS ${CMAKE_CURRENT_SOURCE_DIR}/pack ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat
    DEPENDS packer
    VERBATIM
)

add_custom_target(gamedata ALL DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/game.dat dummy)
