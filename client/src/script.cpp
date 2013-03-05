//
// script.cpp
//

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <tolua++.h>
#include "common.h"
#include "script.h"
#include "LuaBox2D.h"

//
//
Script::Script(const string& source)
{
    lua = luaL_newstate();
    luaL_openlibs(lua);
    tolua_LuaBox2D_open(lua);
    luaL_dostring(lua, source.c_str());
}

//
//
Script::~Script()
{
    lua_close(lua);
}

