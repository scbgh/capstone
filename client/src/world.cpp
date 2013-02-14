//
// world.cpp
//

#include "app.h"
#include "common.h"
#include "mapfile.h"
#include "world.h"
#include "json/picojson.h"
#include "math/math.h"
#include <Box2D/Box2D.h>
#include <sstream>
#include <tuple>

namespace pg {

namespace {

//
// Convert a world Point into a Box2D Vector
b2Vec2 PointToVec(math::Point p) { return b2Vec2(p.x, p.y); }

}

///////////

//
//
World::World(App *app) :
    app_(app)
{
    dbg_draw_.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit);
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
    phys_world_->SetDebugDraw(&dbg_draw_);

    // Create a single static body at the origin for the static world geometry
    b2BodyDef static_body_def;
    b2Body *static_body = phys_world_->CreateBody(&static_body_def);

    // Load the map from disk and turn it into a Box2D world
    unique_ptr<MapFile> map_file = unique_ptr<MapFile>(LoadMapFromJSON(json));

    // Map from shapes to their associated fixture
    map<Shape *, Fixture *> fixtures;
    for (const auto& fixture : map_file->fixtures) {
        fixtures[fixture->shape] = fixture.get();
    }

    // Maps from map bodies to their associated Box2D bodies
    map<Body *, b2Body *> phys_bodies;

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
        if (body->type == kStatic) {
            def.type = b2_staticBody;
        } else if (body->type == kDynamic) {
            def.type = b2_dynamicBody;
        }

        phys_bodies[body.get()] = phys_world_->CreateBody(&def);
    }

    // Create shapes and their associated fixtures
    for (const auto& shape : map_file->shapes) {
        bool has_fixture = (fixtures.find(shape.get()) != fixtures.end());
        Fixture *fixture;
        Body *fix_body;

        if (has_fixture) {
            fixture = fixtures[shape.get()];
            fix_body = fixture->body;
        }

        // Lambda to create the fixture for a shape
        auto fix_shape = [=](b2Shape *sh) mutable {
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

            def.shape = sh;
            target_body->CreateFixture(&def);
        };

        switch (shape->type()) {
            case kPolygon: {
                PolygonShape *poly = static_cast<PolygonShape *>(shape.get());

                for (const auto& subpoly : poly->subpolygons) {
                    unique_ptr<b2PolygonShape> bpoly = unique_ptr<b2PolygonShape>(new b2PolygonShape);
                    b2Vec2 *verts = new b2Vec2[poly->polygon.points.size()];
                    int i = 0;

                    for (const auto& pt : subpoly.points) {
                        math::Point p = { pt.x, pt.y };
                        math::Transform rot = math::Rotate(RAD_TO_DEG(shape->rotation));
                        p = rot.Apply(p);

                        if (!has_fixture) {
                            p.x += shape->position.x;
                            p.y += shape->position.y;
                        } else {
                            p.x += shape->position.x - fix_body->position.x; 
                            p.y += shape->position.y - fix_body->position.y;
                        }
                        verts[i] = PointToVec(p);
                        i++;
                    }

                    bpoly->Set(verts, subpoly.points.size());
                    fix_shape(bpoly.get());
                    delete [] verts;
                }

                break;
            }
            case kCircle: {
                CircleShape *circle = static_cast<CircleShape *>(shape.get());
                unique_ptr<b2CircleShape> bcircle = unique_ptr<b2CircleShape>(new b2CircleShape);
                b2Vec2 offset;
                if (has_fixture) {
                    offset = -b2Vec2(fix_body->position.x, fix_body->position.y);
                }
                bcircle->m_p = PointToVec(circle->position) + offset;
                bcircle->m_radius = circle->radius;
                fix_shape(bcircle.get());

                break;
            }
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

    if (seconds < time_step) {
        phys_world_->Step(seconds, vel_iter, pos_iter);
    } else {
        // Perform as many time steps as necessary to get to the next state
        while (accum < seconds) {
            phys_world_->Step(time_step, vel_iter, pos_iter);
            accum += time_step;
        }
    }
}

//
// Call the Box2D debug renderer
void World::DrawDebug()
{
    phys_world_->DrawDebugData();
}

}
