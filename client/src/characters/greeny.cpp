/*
 * greeny.cpp
 */

#include "app.h"
#include "common.h"
#include "greeny.h"

namespace pg {

//
//
Greeny::Greeny(App *app) :
    Character(app),
    x_movement_(0)
{
    b2World *phys_world = app->world().phys_world();

    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = 0.5;

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

//
//
void Greeny::OnKeyDown(SDL_KeyboardEvent *evt)
{
    Character::OnKeyDown(evt);
    
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            x_movement_ = -15;
            animation_->SetState("walk");
            break;
        case SDLK_RIGHT:
            x_movement_ = 15;
            animation_->SetState("walk");
            break;
        default:
            break;
    }

}

//
//
void Greeny::OnKeyUp(SDL_KeyboardEvent *evt)
{
    Character::OnKeyUp(evt);
    x_movement_ = 0;
    animation_->SetState("stand");
}

//
//
void Greeny::Step(double time)
{
    Character::Step(time);
    if (x_movement_ != 0) {
        body_->ApplyLinearImpulse(b2Vec2(x_movement_, 0), body_->GetWorldCenter());
    }
}

}

