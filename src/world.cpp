//
// world.cpp
//

#include <sstream>
#include <Box2D/Box2D.h>
#include "json/picojson.h"
#include "common.h"
#include "world.h"
#include "app.h"

namespace pg {

//
//
World::World(App *app) :
    app(app)
{
}

//
// Reinitialize the world with a given map name
void World::LoadMap(const string& map_name)
{
    b2Vec2 gravity(0.0f, -10.0f);
    phys_world = new b2World(gravity);

}

//
// Destroy the current world state to make way for a new state
void World::Dispose()
{
}


}
