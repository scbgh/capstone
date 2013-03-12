/*
 * ranger.cpp
 */

#include "app.h"
#include "common.h"
#include "ranger.h"
#include "world.h"
#include "mapfile.h"

namespace pg {

//
//
RangerCharacter::RangerCharacter(App *app) :
    Character(app)
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
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 20;
    fixture_def.shape = shape;
    fixture_def.userData = (void *)kCharacterFixture;
    body_->CreateFixture(&fixture_def);

    shape->m_radius = 0.4;
    shape->m_p = b2Vec2(0.0, -0.1);
    fixture_def.isSensor = true;
    fixture_def.userData = (void *)kBottomFixture;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.5 };
    image_size_ = { 1, 1 };
    walk_speed_ = 1.5;
    jump_speed_ = 50;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("ranger");
    animation_->SetState("stand");

    delete shape;
}

//
//
void RangerCharacter::OnKeyDown(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            state_ |= kMoveLeft;
            break;
        case SDLK_RIGHT:
            state_ |= kMoveRight;
            break;
        case SDLK_SPACE:
            if (grounded_) {
                state_ |= kJump;
                body_->ApplyLinearImpulse(b2Vec2(0, jump_speed_), body_->GetWorldCenter());
            }
        case SDLK_a:
            if (grounded_) {
                launch_ = true;
                launch_angle_ = 0;
            }
        default:
            break;
    }
}

//
//
void RangerCharacter::OnKeyUp(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            state_ &= ~kMoveLeft;
            break;
        case SDLK_RIGHT:
            state_ &= ~kMoveRight;
            break;
        case SDLK_SPACE:
            state_ &= ~kJump;
            break;
        case SDLK_a:
            if (launch_) {
                b2BodyDef body_def;
                body_def.type = b2_dynamicBody;
                b2CircleShape circle;
                circle.m_radius = 0.125;
                b2FixtureDef fixture_def;
                fixture_def.shape = &circle;
                b2Body *body = app_->world().phys_world()->CreateBody(&body_def);
                b2Fixture *fixture = body->CreateFixture(&fixture_def);
                Body *world_body = new Body;
                BodyData *data = new BodyData;
                data->cause_shake = false;
                data->type = kCharacterBody;
                data->data.world_body = world_body;
                body->SetUserData((void *)data);
                app_->world().map()->bodies.push_back(std::unique_ptr<Body>(world_body));
            }
            break;
        default:
            break;
    }

}


}

