/*
 * heavy.cpp
 */

#include "app.h"
#include "common.h"
#include "heavy.h"
#include "world.h"

namespace pg {

//
//
HeavyCharacter::HeavyCharacter(App *app) :
    Character(app),
    rolling_(false)
{
    b2World *phys_world = app->world().phys_world();
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->cause_shake = true;
    data->data.character_body = this;

    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = 0.45;

    b2BodyDef body_def;
    body_def.fixedRotation = true;
    body_def.type = b2_dynamicBody;
    body_def.bullet = true;
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 335;
    fixture_def.shape = shape;
    fixture_def.userData = (void *)kCharacterFixture;
    body_->CreateFixture(&fixture_def);

    shape->m_radius = 0.4;
    shape->m_p = b2Vec2(0.0, -0.1);
    fixture_def.isSensor = true;
    fixture_def.density = 0;
    fixture_def.userData = (void *)kBottomFixture;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.45 };
    image_size_ = { 1, 1 };
    walk_speed_ = 1.5;
    jump_speed_ = 650;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("heavy");
    animation_->SetState("stand");

    delete shape;
}

//
//
void HeavyCharacter::OnKeyDown(SDL_KeyboardEvent *evt)
{
    if (!rolling_) {
        Character::OnKeyDown(evt);
    }

    switch (evt->keysym.sym) {
        case SDLK_a:
            if (IsGrounded()) {
                rolling_ = true;
                body_->SetFixedRotation(false);
            }
        default:
            break;
    }
}

//
//
void HeavyCharacter::OnKeyUp(SDL_KeyboardEvent *evt)
{
    Character::OnKeyUp(evt);

    switch (evt->keysym.sym) {
        case SDLK_a:
            rolling_ = false;
            body_->SetFixedRotation(true);
            body_->SetAngularVelocity(0);
            body_->SetTransform(body_->GetPosition(), 0);
            break;
        default:
            break;
    }
}

//
//
void HeavyCharacter::Step(double time)
{
    Character::Step(time);
    if (rolling_) {
        animation_->SetState("stand");
    }
}

}

