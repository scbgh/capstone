//
// scriptstate.cpp
//

#include "app.h"
#include "scriptstate.h"
#include "world.h"

namespace pg {

//
//
ScriptState::ScriptState(b2World *world, App *app) :
    world_(world),
    app_(app)
{ }

//
//
b2Joint *ScriptState::joint(const std::string& tag)
{
    return app_->world().joint(tag);
}

//
//
b2Body *ScriptState::body(const std::string& tag)
{
    return app_->world().body(tag);
}

//
//
b2Fixture *ScriptState::fixture(const std::string& tag)
{
    return app_->world().fixture(tag);
}

}

