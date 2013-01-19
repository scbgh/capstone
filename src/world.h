//
// world.h
// Manages the state of the game world including the Box2D state
//

#include <Box2D/Box2D.h>

namespace pg {

//
// class World
// Contains the state of the game world including the physics objects.
//
class World {
public:

private:
    b2World *phys_world; // box2d world

    // Non-copyable
    World(const World&);
    World& operator=(const World&);
};

}
