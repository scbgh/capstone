//
// world.h
// Manages the state of the game world including the Box2D state
//

#ifndef _WORLD_H_
#define _WORLD_H_

#include "Box2D/Render.h"
#include "mapfile.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <string>

namespace pg {

class App;
class Sprite;
class Script;
class ScriptState;
class Character;
class World;

enum BodyDataType {
    kWorldBody, kCharacterBody
};

enum FixtureType {
    kBottomFixture = 1,
    kCharacterFixture,
    kGoalFixture
};

//
// User data for physical bodies
struct BodyData {
    BodyDataType type;
    bool cause_shake;
    union {
        Body *world_body;
        Character *character_body;
    } data;
};

//
//
class ContactListener : public b2ContactListener {
public:
    ContactListener(World *world);

    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
private:
    World *world_;
};

//
// Contains the state of the game world including the physics objects.
class World {
public:
    explicit World(App *app);
    ~World();

    void LoadMap(const std::string& map_name);
    void Dispose();
    void Step(float seconds);
    void DrawDebug();
    void DrawCharacters();
    void ShakeScreen(double magnitude, double frequency, double length, math::Vector dir);
    math::Vector ShakeMagnitude() const;

    bool initialized() const { return initialized_; }
    Sprite *back_sprite() { return back_sprite_.get(); }
    Sprite *fore_sprite() { return fore_sprite_.get(); }
    b2World *phys_world() { return phys_world_.get(); }
    double time() const { return time_; }
    b2Fixture *goal_fixture() const { return goal_fixture_; }
    
    b2Joint *joint(const std::string& tag);
    b2Body *body(const std::string& tag);
    b2Fixture *fixture(const std::string& tag);

    ScriptState *script_state() { return script_state_.get(); }
    Script *script() { return script_.get(); }

    void OnKeyDown(SDL_KeyboardEvent *evt);
    void OnKeyUp(SDL_KeyboardEvent *evt);
 
    World(const World&) = delete;
    World& operator=(const World&) = delete;
private:
    App *app_;
    std::unique_ptr<ScriptState> script_state_;
    std::unique_ptr<MapFile> map_;
    std::unique_ptr<b2World> phys_world_; // box2d world
    std::unique_ptr<Script> script_; // lua script
    bool initialized_; // is the world initialized?
    DebugDraw dbg_draw_; // debug drawer for box2d
    ContactListener contact_listener_;
    double time_;
    std::string active_character_;

    // screen shake
    double shake_;
    double shake_magnitude_;
    double shake_frequency_;
    double shake_length_;
    math::Vector shake_direction_;

    std::map<std::string, b2Fixture *> tagged_fixtures_;
    std::map<std::string, b2Joint *> tagged_joints_;
    std::map<std::string, b2Body *> tagged_bodies_;
    b2Fixture *goal_fixture_;

    std::map<std::string, std::unique_ptr<Character>> characters_;

    std::unique_ptr<Sprite> back_sprite_;
    std::unique_ptr<Sprite> fore_sprite_;
};

}

#endif
