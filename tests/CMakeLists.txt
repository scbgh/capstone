project(PHYSGAME_TESTS)


set(PHYSGAME_TESTS_SRCS
    test_physgame.cpp
    ../src/pack.cpp
    ../src/file.cpp
)

add_executable(physgame_tests ${PHYSGAME_TESTS_SRCS})

file(COPY test_archive DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
