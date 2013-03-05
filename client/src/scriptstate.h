//
// scriptstate.h
// An object that acts as a marshal between C++ and Lua
//

#ifndef _SCRIPTSTATE_H_
#define _SCRIPTSTATE_H_

class b2World;

namespace pg {

class App;

class ScriptState { //tolua_export
public:
    ScriptState(b2World *world, App *app);

//tolua_begin
    b2World *world() const { return world_; }
    int test() { return 5; }
//tolua_end
private:
    b2World *world_;
    App *app_;
}; //tolua_export

}

#endif

