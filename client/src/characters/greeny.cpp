/*
 * greeny.cpp
 */

#include "app.h"
#include "common.h"
#include "greeny.h"
#include "world.h"

namespace pg {

//
//
Greeny::Greeny(App *app) :
    Character(app)
{
    b2World *phys_world = app->world().phys_world();

    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = 0.45;

    b2BodyDef body_def;
    body_def.fixedRotation = true;
    body_def.type = b2_dynamicBody;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 20;
    fixture_def.shape = shape;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.5 };
    image_size_ = { 1, 1 };

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("walk");
    animation_->SetState("stand");
}

}

