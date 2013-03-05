//
// script.cpp
//

#include "script.h"
#include "common.h"

namespace pg {

//
//
Script::Script(const string& source) :
    params_(0)
{
    lua_ = luaL_newstate();
    luaL_openlibs(lua_);
    tolua_LuaBox2D_open(lua_);
    tolua_scriptstate_open(lua_);
    luaL_dostring(lua_, source.c_str());
}

//
//
Script::~Script()
{
    lua_close(lua_);
}

//
//
void Script::PushParameter(double val)
{
    lua_pushnumber(lua_, val);
}

//
//
void Script::PushParameter(const string& val)
{
    lua_pushstring(lua_, val.c_str());
}

//
//
void Script::PushParameter(ScriptState *val)
{
    tolua_pushusertype(lua_, val, "ScriptState");
}

//
//
void Script::GetFunction(const string& val)
{
    lua_getfield(lua_, LUA_GLOBALSINDEX, val.c_str());
}

}
