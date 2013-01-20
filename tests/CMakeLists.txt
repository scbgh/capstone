project(PHYSGAME_TESTS)


set(PHYSGAME_TESTS_SRCS
    test_physgame.cpp
    ../src/pack.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(physgame_tests ${PHYSGAME_TESTS_SRCS})