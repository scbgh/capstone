//
// script.cpp
//

#include "script.h"
#include "common.h"

namespace pg {

//
//
Script::Script(const string& stdlib, const string& source) :
    params_(0)
{
    lua_ = luaL_newstate();
    luaL_openlibs(lua_);
    tolua_Box2D_open(lua_);
    tolua_scriptstate_open(lua_);
    if (luaL_dostring(lua_, stdlib.c_str())) {
        Debug("Lua error (stdlib): %s\n", lua_tostring(lua_, -1));
        lua_remove(lua_, -1);
    }
    if (luaL_dostring(lua_, source.c_str())) {
        Debug("Lua error: %s\n", lua_tostring(lua_, -1));
        lua_remove(lua_, -1);
    }
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
void Script::PushParameter(b2Contact *val)
{
    tolua_pushusertype(lua_, val, "b2Contact");
}

//
//
void Script::GetFunction(const string& val)
{
    lua_getfield(lua_, LUA_GLOBALSINDEX, val.c_str());
}

}
