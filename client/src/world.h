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
    kStaticBody, //!< Level geometry body
    kWorldBody, //!< A body that exists in the level data (e.g., was placed in the editor)
    kCharacterBody //!< A body that is attached to a character
};

enum FixtureType {
    kBottomFixture = 1,
    kCharacterFixture,
    kGoalFixture
};

//
//! User data for physical bodies stored internally
struct BodyData {
    //! What does this body represent?
    BodyDataType type;
    bool cause_shake;
    //! Should this body colide with the player?
    bool collide_player;
    //! The underlying data object for this body
    union {
        Body *world_body;
        Character *character_body;
    } data;
};

//
//! Used to react to collisions that happen in the Box2D world
//! (See Box2D documentation)
class ContactListener : public b2ContactListener {
public:
    ContactListener(World *world);

    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact);
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
private:
    World *world_;
};

//
//! Contains the entire state of the game world
class World {
public:
    //! Create a new World
    //! \param app The App to which this World belongs
    explicit World(App *app);
    ~World();

    //! Load a map from a JSON level definition
    //! \param map_name The path in the pack file of the level to load
    void LoadMap(const std::string& map_name);
    //! Clear the world state to get it ready for a new level
    void Dispose();
    //! Step the world simulation forward by the specified time
    //! \param seconds The number of seconds by which to step the simulation
    void Step(float seconds);
    //! Draw the Box2D debug information such as joints, fixtures, and bodies
    void DrawDebug();
    //! Make all the characters in the level draw themselves
    void DrawCharacters();

    void ShakeScreen(double magnitude, double frequency, double length, math::Vector dir);
    math::Vector ShakeMagnitude() const;

    //! Is the world initialized?
    bool initialized() const { return initialized_; }
    
    //! The underlying Box2D world object
    b2World *phys_world() { return phys_world_.get(); }
    //! The number of second elapsed since this World began
    double time() const { return time_; }
    //! The Box2D fixture representing the level goal
    b2Fixture *goal_fixture() const { return goal_fixture_; }
    //! The root object holding the map data
    MapFile *map_file() { return map_.get(); }
    
    //! Return the Box2D joint with the specified user tag as specified in the editor
    //! \param tag The tag of the joint to look up
    b2Joint *joint(const std::string& tag);
    //! Return the Box2D body with the specified user tag as specified in the editor
    //! \param tag The tag of the body to look up
    b2Body *body(const std::string& tag);
    //! Return the Box2D fixture with the specified user tag as specified in the editor
    //! \param tag The tag of the fixture to look up
    b2Fixture *fixture(const std::string& tag);

    //! Return the script proxy object for this world
    ScriptState *script_state() { return script_state_.get(); }
    //! Return the active script for the current level
    Script *script() { return script_.get(); }

    //! Handle an SDL key down event
    //! \param evt The event as caught by SDL
    void OnKeyDown(SDL_KeyboardEvent *evt);
    //! Handle an SDL key up event
    //! \param evt The event as caught by SDL
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

    friend class ContactListener;
};

}

#endif
