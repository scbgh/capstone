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

//! A proxy object for communicating between the World and a Script
class ScriptState { //tolua_export
public:
    //! Create a new ScriptState
    //! \param world The underlying Box2D world
    //! \param app The App which owns this ScriptState
    ScriptState(b2World *world, App *app);

//tolua_begin
    //! The underlying Box2D world
    b2World *world() const { return world_; }
    //! Look up a joint with the specified tag
    //! \param tag The joint to look up
    b2Joint *joint(const std::string& tag);
    //! Look up a body with the specified tag
    //! \param tag The body to look up
    b2Body *body(const std::string& tag);
    //! Look up a fixture with the specified tag
    //! \param tag The fixture to look up
    b2Fixture *fixture(const std::string& tag);
//tolua_end
private:
    b2World *world_;
    App *app_;
}; //tolua_export

}

#endif

