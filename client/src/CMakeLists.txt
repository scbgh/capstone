project(PHYSGAME)

if(NOT SDL_FOUND)
    message("SDL not found!")
endif(NOT SDL_FOUND)

set(PHYSGAME_SRCS
    animation.cpp
    app.cpp
    box2d/Render.cpp
    character.cpp
    characters/greeny.cpp
    characters/heavy.cpp
    characters/jump.cpp
    characters/ranger.cpp
    characters/strong.cpp
    file.cpp
    main.cpp
    mapfile.cpp
    math/box.cpp
    math/color.cpp
    math/matrix.cpp
    math/point.cpp
    math/polygon.cpp
    math/transform.cpp
    math/vector.cpp
    pack.cpp
    renderer.cpp
    script.cpp
    scriptstate.cpp
    sprite.cpp
    world.cpp
    ${THIRDPARTY_DIR}/LuaBox2D/LuaBox2D.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/lua_scriptstate.cpp
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lua_scriptstate.cpp
    COMMAND tolua++ ARGS
        -H ${CMAKE_CURRENT_BINARY_DIR}/lua_scriptstate.h
        -o ${CMAKE_CURRENT_BINARY_DIR}/lua_scriptstate.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/luabindings/scriptstate.pkg
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/luabindings/scriptstate.pkg
            ${CMAKE_CURRENT_SOURCE_DIR}/scriptstate.h
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating tolua binding"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${THIRDPARTY_DIR})

add_executable(physgame ${PHYSGAME_SRCS})
target_link_libraries(physgame ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${TOLUA++_LIBRARY} ${LUA_LIBRARIES} ${OPENGL_LIBRARIES} ${BOX2D_LIBRARY})

