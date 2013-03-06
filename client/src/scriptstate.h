//
// scriptstate.h
// An object that acts as a marshal between C++ and Lua
//

#ifndef _SCRIPTSTATE_H_
#define _SCRIPTSTATE_H_

#include <string>

class b2World;
class b2Joint;
class b2Body;
class b2Fixture;

namespace pg {

class App;

class ScriptState { //tolua_export
public:
    ScriptState(b2World *world, App *app);

//tolua_begin
    b2World *world() const { return world_; }
    b2Joint *joint(const std::string& tag);
    b2Body *body(const std::string& tag);
    b2Fixture *fixture(const std::string& tag);
//tolua_end
private:
    b2World *world_;
    App *app_;
}; //tolua_export

}

#endif

