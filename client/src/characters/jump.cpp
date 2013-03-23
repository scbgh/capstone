/*
 * jump.cpp
 */

#include "app.h"
#include "common.h"
#include "jump.h"
#include "world.h"

namespace pg {

//
//
JumpCharacter::JumpCharacter(App *app) :
    Character(app)
{
    b2World *phys_world = app->world().phys_world();
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->cause_shake = true;
    data->data.character_body = this;

    b2PolygonShape *shape = new b2PolygonShape;
    shape->SetAsBox(0.3, 0.4);

    b2BodyDef body_def;
    body_def.fixedRotation = true;
    body_def.type = b2_dynamicBody;
    body_def.bullet = true;
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 140;
    fixture_def.shape = shape;
    fixture_def.userData = (void *)kCharacterFixture;
    body_->CreateFixture(&fixture_def);

    shape->SetAsBox(0.25, 0.4, b2Vec2(0.0, -0.1), 0);
    fixture_def.isSensor = true;
    fixture_def.density = 0;
    fixture_def.userData = (void *)kBottomFixture;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.5 };
    image_size_ = { 1, 1 };
    walk_speed_ = 1.5;
    jump_speed_ = 250;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("jump");
    animation_->SetState("stand");

    delete shape;
}

//
//
void JumpCharacter::OnKeyDown(SDL_KeyboardEvent *evt)
{
    Character::OnKeyDown(evt);

    switch (evt->keysym.sym) {
        case SDLK_a:
            if (IsGrounded()) {
                state_ |= kJump;
                body_->ApplyLinearImpulse(b2Vec2(0, 2 * jump_speed_), body_->GetWorldCenter());
            }
        default:
            break;
    }
}

}

