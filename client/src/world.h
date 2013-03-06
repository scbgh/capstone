//
// world.h
// Manages the state of the game world including the Box2D state
//

#ifndef _WORLD_H_
#define _WORLD_H_

#include "Box2D/Render.h"
#include "mapfile.h"
#include "sprite.h"
#include "script.h"
#include "scriptstate.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <string>

namespace pg {

class App;

//
// Contains the state of the game world including the physics objects.
class World {
public:
    explicit World(App *app);

    void LoadMap(const std::string& map_name);
    void Dispose();
    void Step(float seconds);
    void DrawDebug();

    bool initialized() const { return initialized_; }
    Sprite *back_sprite() { return back_sprite_.get(); }
    Sprite *fore_sprite() { return fore_sprite_.get(); }
    b2World *phys_world() { return phys_world_.get(); }
    
    b2Joint *joint(const std::string& tag);
    b2Body *body(const std::string& tag);
    b2Fixture *fixture(const std::string& tag);

    ScriptState *script_state() { return script_state_.get(); }
    Script *script() { return script_.get(); }
private:
    App *app_;
    std::unique_ptr<ScriptState> script_state_;
    std::unique_ptr<MapFile> map_;
    std::unique_ptr<b2World> phys_world_; // box2d world
    std::unique_ptr<Script> script_; // lua script
    bool initialized_; // is the world initialized?
    DebugDraw dbg_draw_; // debug drawer for box2d

    std::map<std::string, b2Fixture *> tagged_fixtures_;
    std::map<std::string, b2Joint *> tagged_joints_;
    std::map<std::string, b2Body *> tagged_bodies_;

    std::unique_ptr<Sprite> back_sprite_;
    std::unique_ptr<Sprite> fore_sprite_;

    // Non-copyable
    World(const World&);
    World& operator=(const World&);
};

}

#endif
