//
// world.h
// Manages the state of the game world including the Box2D state
//

#ifndef _WORLD_H_
#define _WORLD_H_

#include <Box2D/Box2D.h>
#include <string>

namespace pg {

class App;

//
// class World
// Contains the state of the game world including the physics objects.
//
class World {
public:
    explicit World(App *app);

    void LoadMap(const std::string& map_name);
    void Dispose();
private:
    App *app;
    b2World *phys_world; // box2d world

    // Non-copyable
    World(const World&);
    World& operator=(const World&);
};

}

#endif
