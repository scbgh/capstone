//
// world.cpp
//

#include "app.h"
#include "characters/heavy.h"
#include "common.h"
#include "json/picojson.h"
#include "mapfile.h"
#include "math/math.h"
#include "pack.h"
#include "script.h"
#include "world.h"
#include <Box2D/Box2D.h>
#include <sstream>
#include <tuple>

namespace pg {

namespace {

//
// Convert a world Point into a Box2D Vector
b2Vec2 PointToVec(math::Point p) { return b2Vec2(p.x, p.y); }

}

//
//
ContactListener::ContactListener(World *world) :
    world_(world)
{ }

//
//
void ContactListener::BeginContact(b2Contact *contact)
{
    BodyData *a, *b;
    a = (BodyData *)contact->GetFixtureA()->GetBody()->GetUserData();
    b = (BodyData *)contact->GetFixtureB()->GetBody()->GetUserData();

    if (contact->GetFixtureA()->GetUserData() == (void *)kBottomFixture) {
        a->data.character_body->BeginFootContact();
    }
    if (contact->GetFixtureB()->GetUserData() == (void *)kBottomFixture) {
        b->data.character_body->BeginFootContact();
    }
}

//
//
void ContactListener::EndContact(b2Contact *contact)
{
    BodyData *a, *b;
    a = (BodyData *)contact->GetFixtureA()->GetBody()->GetUserData();
    b = (BodyData *)contact->GetFixtureB()->GetBody()->GetUserData();

    if (contact->GetFixtureA()->GetUserData() == (void *)kBottomFixture) {
        a->data.character_body->EndFootContact();
    }
    if (contact->GetFixtureB()->GetUserData() == (void *)kBottomFixture) {
        b->data.character_body->EndFootContact();
    }
}

//
//
void ContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
    b2Body *ba = contact->GetFixtureA()->GetBody();
    b2Body *bb = contact->GetFixtureB()->GetBody();
    BodyData *a, *b;
    a = (BodyData *)ba->GetUserData();
    b = (BodyData *)bb->GetUserData();

    b2WorldManifold manifold;
    contact->GetWorldManifold(&manifold);
    int max_index = 0;
    double max_force = 0;
    
    for (int i = 0; i < contact->GetManifold()->pointCount; i++) {
        if (impulse->normalImpulses[i] > max_force) {
            max_force = impulse->normalImpulses[i];
            max_index = i;
        }
    }

    b2Vec2 vel1 = ba->GetLinearVelocity();
    b2Vec2 vel2 = bb->GetLinearVelocity();
    b2Vec2 impact_velocity = vel1 - vel2;
    if (max_force > 2.0 && impact_velocity.Length() > 0.0 && ((a && a->cause_shake) || (b && b->cause_shake))) { 
        // disable screen shake for now
        //world_->ShakeScreen(max_force / 5000, 0.5, 0.5, math::Vector(impact_velocity.x, impact_velocity.y));
    }
}

///////////

//
//
World::World(App *app) :
    app_(app),
    shake_magnitude_(0),
    shake_(0),
    shake_length_(0),
    shake_direction_(1, 1),
    contact_listener_(this)
{
    dbg_draw_.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit);
}

//
//
World::~World()
{ }

//
// Reinitialize the world with a given map name
void World::LoadMap(const string& map_name)
{
    Dispose();
    
    Pack& pack = app_->pack();
    string map_path = "maps/" + map_name + ".json";
    string script_path = "scripts/" + map_name + ".lua";
    if (!pack.contains(map_path)) {
        Die("Could not load map '%s'", map_name.c_str());
    }
    string json = pack[map_path].ToString();
    string src = pack[script_path].ToString();

    b2Vec2 gravity(0.0f, -10.0f);
    phys_world_ = unique_ptr<b2World>(new b2World(gravity));
    phys_world_->SetDebugDraw(&dbg_draw_);
    phys_world_->SetContactListener(&contact_listener_);

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
    tagged_bodies_.clear();

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

        // Load the sprite for the body if applicable
        if (pack.contains(body->image)) {
            PackEntry entry = pack[body->image];
            body->image_sprite = unique_ptr<Sprite>(new Sprite(entry.data, entry.length));
        }
        BodyData *data = new BodyData;
        data->type = kWorldBody;
        data->cause_shake = false;
        data->data.world_body = body.get();
        body->data.reset(data);
        def.userData = data;
        phys_bodies[body.get()] = phys_world_->CreateBody(&def);

        if (body->tag != "") {
            tagged_bodies_[body->tag] = phys_bodies[body.get()];
        }
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
        auto fix_shape = [&](b2Shape *sh) mutable {
            b2FixtureDef def;
            b2Fixture *fix;
            b2Body *target_body;
            string tag;

            if (has_fixture) {
                def.friction = fixture->friction;
                def.restitution = fixture->restitution;
                def.density = fixture->density;
                def.isSensor = fixture->is_sensor;
                tag = fixture->tag;
                b2Body *body = phys_bodies[fixture->body];
                target_body = body;
            } else {
                def.friction = 0.5;
                target_body = static_body;
            }

            def.shape = sh;
            fix = target_body->CreateFixture(&def);
            if (tag != "") {
                tagged_fixtures_[tag] = fix;
            }
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

    // Create joints
    for (const auto& joint : map_file->joints) {
        b2Joint *j;
        b2JointDef *def;
        switch (joint->type()) {
            case kDistance: {
                DistanceJoint *distance_joint = (DistanceJoint *)joint.get();
                b2DistanceJointDef *inner_def = new b2DistanceJointDef;
                def = inner_def;
                inner_def->Initialize(phys_bodies[joint->body_a], phys_bodies[joint->body_b], 
                    PointToVec(distance_joint->anchor_a), PointToVec(distance_joint->anchor_b));
                inner_def->frequencyHz = distance_joint->frequency_hz;
                inner_def->dampingRatio = distance_joint->damping_ratio;
                break;
            }
            case kRevolute: {
                RevoluteJoint *revolute_joint = (RevoluteJoint *)joint.get();
                b2RevoluteJointDef *inner_def = new b2RevoluteJointDef;
                def = inner_def;
                inner_def->Initialize(phys_bodies[joint->body_a], phys_bodies[joint->body_b],
                    PointToVec(revolute_joint->anchor));
                inner_def->enableMotor = revolute_joint->enable_motor;
                inner_def->maxMotorTorque = revolute_joint->max_motor_torque;
                inner_def->motorSpeed = revolute_joint->motor_speed;
                inner_def->upperAngle = revolute_joint->upper_angle;
                inner_def->lowerAngle = revolute_joint->lower_angle;
                inner_def->referenceAngle = revolute_joint->reference_angle;
                inner_def->enableLimit = revolute_joint->enable_limit;
                break;
            }
            default:
                Die("Invalid joint type");
                break;
        }
        def->collideConnected = joint->collide_connected;
        j = phys_world_->CreateJoint(def);
        if (joint->tag != "") {
            tagged_joints_[joint->tag] = j;
        }
        delete def;
    }

    // Load the sprites for the background and foreground images
    if (pack.contains(map_file->back_image)) {
        PackEntry entry = pack[map_file->back_image];
        back_sprite_ = unique_ptr<Sprite>(new Sprite(entry.data, entry.length));
    } else {
        Debug("Failed to load back image: %s", map_file->back_image.c_str());
    }

    if (pack.contains(map_file->fore_image)) {
        PackEntry entry = pack[map_file->fore_image];
        fore_sprite_ = unique_ptr<Sprite>(new Sprite(entry.data, entry.length));
    } else {
        Debug("Failed to load fore image: %s", map_file->fore_image.c_str());
    }

    initialized_ = true;
    map_ = std::move(map_file);

    // Create characters
    if (tagged_bodies_.find("heavy_start") != tagged_bodies_.end()) {
        HeavyCharacter *heavy = new HeavyCharacter(app_);
        b2Vec2 pos = tagged_bodies_["heavy_start"]->GetPosition();
        heavy->SetPosition(pos.x, pos.y);
        characters_["heavy"] = std::unique_ptr<Character>(std::move(heavy));
    }

    // Load the lua script
    script_state_.reset(new ScriptState(phys_world_.get(), app_));
    script_.reset(new Script(src));
    script_->Call<void>("init", script_state_.get());
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

    for (auto& it : characters_) {
        it.second->Step(seconds);
    }

    shake_ -= seconds;
    if (shake_ < 0.0) shake_ = 0.0;
}

//
//
void World::ShakeScreen(double magnitude, double frequency, double length, math::Vector dir)
{
    shake_ = length;
    shake_magnitude_ = magnitude;
    shake_frequency_ = frequency;
    shake_length_ = length;
    shake_direction_ = dir;
}

//
//
math::Vector World::ShakeMagnitude() const
{
    double sx = shake_length_ - shake_;
    double amt;
    if (sx == 0.0) return math::Vector(0, 0);
    amt = shake_magnitude_ * sin(M_PI * 4 * sx * shake_frequency_) / (M_PI * 4 * sx * shake_frequency_);
    return Normalized(shake_direction_) * amt;
}

//
//
b2Joint *World::joint(const std::string& tag)
{
    if (tagged_joints_.find(tag) != tagged_joints_.end()) {
        return tagged_joints_[tag];
    }
    return NULL;
}

//
//
b2Body *World::body(const std::string& tag)
{
    if (tagged_bodies_.find(tag) != tagged_bodies_.end()) {
        return tagged_bodies_[tag];
    }
    return NULL;
}

//
//
b2Fixture *World::fixture(const std::string& tag)
{
    if (tagged_fixtures_.find(tag) != tagged_fixtures_.end()) {
        return tagged_fixtures_[tag];
    }
    return NULL;
}

//
// Call the Box2D debug renderer
void World::DrawDebug()
{
    phys_world_->DrawDebugData();
}

//
//
void World::DrawCharacters()
{
    for (auto& it : characters_) {
        it.second->Render();
    }
}

//
//
void World::OnKeyDown(SDL_KeyboardEvent *evt)
{
    script_->Call<void>("key_down", script_state_.get(),
        std::string(SDL_GetKeyName(evt->keysym.sym)));
    for (auto& it : characters_) {
        it.second->OnKeyDown(evt);
    }
}

//
//
void World::OnKeyUp(SDL_KeyboardEvent *evt)
{
    script_->Call<void>("key_up", script_state_.get(),
        std::string(SDL_GetKeyName(evt->keysym.sym)));
    for (auto& it : characters_) {
        it.second->OnKeyUp(evt);
    }
}

}
