//
// script.h
// Infrastructure for embedding Lua scripts
//

#ifndef _SCRIPT_H_
#define _SCRIPT_H_

#include <string>

struct lua_State;

class Script {
public:
    Script(const std::string& source);
    ~Script();

    void Call(const std::string&);

    template <typename T, typename ...Args>
    void Call(T a, Args... args)
    {
        PushParameter(a);
        Call(args...);
    }
private:
    void PushParameter(double val);

    lua_State *lua;
};

#endif

