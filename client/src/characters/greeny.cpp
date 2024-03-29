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
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->data.character_body = this;

    //b2PolygonShape *shape = new b2PolygonShape;
    //shape->SetAsBox(0.3, 0.45);
    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = 0.45;

    b2BodyDef body_def;
    body_def.fixedRotation = true;
    body_def.type = b2_dynamicBody;
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 20;
    fixture_def.shape = shape;
    body_->CreateFixture(&fixture_def);

    //shape->SetAsBox(0.25, 0.2, b2Vec2(0.0, -0.3), 0);
    shape->m_radius = 0.4;
    shape->m_p = b2Vec2(0.0, -0.1);
    fixture_def.isSensor = true;
    fixture_def.userData = (void *)kBottomFixture;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.5 };
    image_size_ = { 1, 1 };
    walk_speed_ = 2;
    jump_speed_ = 100;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("walk");
    animation_->SetState("stand");

    delete shape;
}

}

