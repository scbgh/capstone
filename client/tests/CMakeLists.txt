project(PHYSGAME_TESTS)


set(PHYSGAME_TESTS_SRCS
    test_physgame.cpp
    ../src/file.cpp
    ../src/mapfile.cpp
    ../src/math/box.cpp
    ../src/math/color.cpp
    ../src/math/matrix.cpp
    ../src/math/point.cpp
    ../src/math/polygon.cpp
    ../src/math/transform.cpp
    ../src/math/vector.cpp
    ../src/pack.cpp
)

add_executable(physgame_tests ${PHYSGAME_TESTS_SRCS})
target_link_libraries(physgame_tests ${SDL_LIBRARY} ${OPENGL_LIBRARIES} ${BOX2D_LIBRARY})

file(COPY test_archive DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
