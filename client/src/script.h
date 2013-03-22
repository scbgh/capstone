//
// script.h
// Infrastructure for embedding Lua scripts
//

#ifndef _SCRIPT_H_
#define _SCRIPT_H_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <iostream>
#include <string>
#include <tolua++.h>
#include "common.h"
#include "scriptstate.h"
#include "LuaBox2D/LuaBox2D.h"
#include "lua_scriptstate.h"

struct b2Contact;

namespace pg {

struct ScriptState;

//
//! A wrapper class for interacting with Lua scripts
class Script {
public:
    //! Load a Lua script into a new Script wrapper
    //! \param stdlib The Lua source code of the standard library
    //! \param source The Lua source code to load
    Script(const std::string& stdlib, const std::string& source);
    ~Script();

    //! Call a nullary function in this Lua script
    //! \param name The name of the function to call
    //! \tparam R The return type of the function
    template <typename R> R Call(const std::string& name);

    //! Call a function in this Lua script
    //! \param name The name of the function to call
    //! \param args The arguments to the function
    //! \tparam R The return type of the function
    //! \tparam ...Args The types of the arguments
    template <typename R, typename ...Args> R Call(const std::string& name, Args... args);
    
private:
    void PushParameter(double val);
    void PushParameter(ScriptState *val);
    void PushParameter(const std::string& val);
    void PushParameter(b2Contact *val);

    void GetFunction(const std::string& val);

    template <typename R> R InnerCall();
    template <typename R, typename T, typename ...Args> R InnerCall(T a, Args... args);

    template <typename R> R GetResult();
  
    lua_State *lua_;
    int params_;
};

//
//
template <typename R, typename T, typename ...Args>
R Script::InnerCall(T a, Args... args)
{
    PushParameter(a);
    params_++;
    InnerCall<R>(args...);
}
//
//
template <typename R, typename ...Args>
R Script::Call(const std::string& name, Args... args)
{
    GetFunction(name);
    return InnerCall<R>(args...);
}

//
//
template <>
inline double Script::GetResult<double>()
{
    double d = lua_tonumber(lua_, -1);
    lua_remove(lua_, -1);
    return d;
}

//
//
template <>
inline std::string Script::GetResult<std::string>()
{
    const char *c = lua_tostring(lua_, -1);
    lua_remove(lua_, -1);
    if (!c) return ""; 
    return std::string(c);
}

//
//
template <typename R>
R Script::GetResult()
{
    static_assert(false || std::is_void<R>::value, "Unknown result type");
}

//
//
template <typename R>
R Script::InnerCall()
{
    int r = lua_pcall(lua_, params_, 1, 0);
    params_ = 0;
    if (r) {
        Debug("Lua error: %s\n", GetResult<std::string>().c_str());
    } else {
        return GetResult<R>();
    }
}

//
//
template <>
inline void Script::InnerCall<void>()
{
    int r = lua_pcall(lua_, params_, 0, 0);
    params_ = 0;
    if (r) {
        Debug("Lua error: %s\n", GetResult<std::string>().c_str());
    }
}


}

#endif

