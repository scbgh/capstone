cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    message("Using Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)
add_subdirectory(src)