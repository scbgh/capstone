//
// world.cpp
//

#include "app.h"
#include "common.h"
#include "mapfile.h"
#include "world.h"
#include "json/picojson.h"
#include <Box2D/Box2D.h>
#include <sstream>
#include <tuple>

namespace pg {

namespace {

//
// Convert a world Point into a Box2D Vector
b2Vec2 PointToVec(Point p) { return b2Vec2(p.x, p.y); }

}

///////////

//
//
World::World(App *app) :
    app_(app)
{
}

//
// Reinitialize the world with a given map name
void World::LoadMap(const string& map_name)
{
    Dispose();

    string map_path = "maps/" + map_name + ".json";
    if (!app_->pack().contains(map_path)) {
        Die("Could not load map '%s'", map_name.c_str());
    }
    string json = app_->pack()[map_path].ToString();

    b2Vec2 gravity(0.0f, -10.0f);
    phys_world_ = unique_ptr<b2World>(new b2World(gravity));

    // Create a single static body at the origin for the static world geometry
    b2BodyDef static_body_def;
    b2Body *static_body = phys_world_->CreateBody(&static_body_def);

    // Load the map from disk and turn it into a Box2D world
    shared_ptr<MapFile> map_file = LoadMapFromJSON(json);

    // Map from shapes to their associated fixture
    map<shared_ptr<Shape>, shared_ptr<Fixture>> fixtures;
    for (const auto& fixture : map_file->fixtures) {
        fixtures[fixture->shape] = fixture;
    }

    // Maps from map bodies to their associated Box2D bodies
    map<shared_ptr<Body>, b2Body *> phys_bodies;

    // Create bodies in the world
    for (const auto& body : map_file->bodies) {
        b2BodyDef def;
        def.position = PointToVec(body->position);
        def.angle = body->rotation;
        def.linearVelocity = PointToVec(body->linear_velocity);
        def.angularVelocity = body->angular_velocity;
        def.linearDamping = body->linear_damping;
        def.angularDamping = body->angular_damping;
        def.allowSleep = body->allow_sleep;
        def.awake = body->awake;
        def.fixedRotation = body->fixed_rotation;
        def.bullet = body->bullet;
        def.active = body->active;

        phys_bodies[body] = phys_world_->CreateBody(&def);
    }

    // Create shapes and their associated fixtures
    for (const auto& shape : map_file->shapes) {
        bool has_fixture = (fixtures.find(shape) != fixtures.end());
        shared_ptr<Fixture> fixture;
        shared_ptr<Body> fix_body;
        vector<shared_ptr<b2Shape>> shapes_to_fix;

        if (has_fixture) {
            fixture = fixtures[shape];
            fix_body = fixture->body;
        }     

        switch (shape->type()) {
            case kPolygon: {
                PolygonShape *poly = static_cast<PolygonShape *>(shape.get());

                // Construct a shape for each triangle
                for (const auto& tri : poly->triangles) {
                    shared_ptr<b2PolygonShape> bpoly = make_shared<b2PolygonShape>();
                    b2Vec2 verts[3];
                    Point points[3] = { tri.p[0], tri.p[1], tri.p[2] };

                    for (int i = 0; i < 3; i++) {
                        // If this isn't part of a dynamic fixture, we make the vertex coordinates absolute,
                        // otherwise we make them relative to the body position
                        if (!has_fixture) {
                            points[i].x += shape->position.x;
                            points[i].y += shape->position.y;
                        } else {
                            points[i].x += shape->position.x - fix_body->position.x; 
                            points[i].y += shape->position.y - fix_body->position.y;
                        }
                        verts[i] = PointToVec(points[i]);
                    }
                    bpoly->Set(verts, 3);
                    shapes_to_fix.push_back(bpoly);
                }
                break;
            }
            case kCircle: {
                CircleShape *circle = static_cast<CircleShape *>(shape.get());
                shared_ptr<b2CircleShape> bcircle = make_shared<b2CircleShape>();
                bcircle->m_p = PointToVec(circle->position);
                bcircle->m_radius = circle->radius;
                shapes_to_fix.push_back(bcircle);

                break;
            }
        }

        // Create fixtures for this shape
        b2FixtureDef def;
        b2Body *target_body;
        if (has_fixture) {
            def.friction = fixture->friction;
            def.restitution = fixture->restitution;
            def.density = fixture->density;
            def.isSensor = fixture->is_sensor;
            b2Body *body = phys_bodies[fixture->body];
            target_body = body;
        } else {
            target_body = static_body;
        }

        for (const auto& fix_shape : shapes_to_fix) {
            def.shape = fix_shape.get();
            target_body->CreateFixture(&def);
        }
    }

    initialized_ = true;
}

//
// Destroy the current world state to make way for a new state
void World::Dispose()
{
}

//
// Step the world state forward by a given number of seconds
void World::Step(float seconds)
{
    const float time_step = 1.f / 60.f;
    const int vel_iter = 10;
    const int pos_iter = 8;
    float accum = 0;

    // Perform as many time steps as necessary to get to the next state
    while (accum < seconds) {
        phys_world_->Step(time_step, vel_iter, pos_iter);
        accum += time_step;
    }
}

}
