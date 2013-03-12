/*
 * character.cpp
 */

#include "character.h"
#include "common.h"
#include "mapfile.h"
#include "world.h"

namespace pg {

//
//
Character::Character(App *app) :
    app_(app),
    direction_(kRight),
    foot_contacts_(0)
{
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->collide_player = false;
    data->data.character_body = this;
    state_ = kIdle;
}

//
//
void Character::Step(double time)
{
    animation_->Step(time);
    b2Vec2 vel = body_->GetLinearVelocity();
    double to_speed;

    if (state_ & kMoveLeft) {
        to_speed = -walk_speed_;
        animation_->SetState("walk");
        direction_ = kLeft;
    } else if (state_ & kMoveRight) {
        to_speed = walk_speed_;
        animation_->SetState("walk");
        direction_ = kRight;
    } else {
        to_speed = 0.0;
    }

    if (!grounded_) {
        animation_->SetState("jump");
    }

    double vel_change = to_speed - vel.x;
    double force = body_->GetMass() * vel_change / time;

    if (to_speed != 0.0) {
        body_->ApplyForce(b2Vec2(force, 0), body_->GetWorldCenter());
    }

    if (grounded_ && to_speed == 0.0) {
        animation_->SetState("stand");
    }
}

//
//
void Character::Render() const
{
    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x, pos.y, 0);
    glRotated(RAD_TO_DEG(body_->GetAngle()), 0, 0, 1);
    glTranslated(image_offset_.x, image_offset_.y, 0);
    animation_->Render(image_size_.x, image_size_.y, (direction_ == kLeft), false);
    glPopMatrix();
}

//
//
void Character::OnKeyDown(SDL_KeyboardEvent *evt)
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
        default:
            break;
    }
}

//
//
void Character::OnKeyUp(SDL_KeyboardEvent *evt)
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
        default:
            break;
    }

}

//
//
void Character::BeginFootContact()
{
    foot_contacts_++;
    grounded_ = true;
}

//
//
void Character::EndFootContact()
{
    foot_contacts_--;
    if (!foot_contacts_) {
        grounded_ = false;
    }
}

}

