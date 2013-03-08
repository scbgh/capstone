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
    direction_(kRight)
{
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->data.character_body = this;
}

//
//
void Character::Step(double time)
{
    animation_->Step(time);
    b2Vec2 vel = body_->GetLinearVelocity();
    double vel_change = walk_speed_ - vel.x;
    double force = body_->GetMass() * vel_change / time;
}

//
//
void Character::Render() const
{
    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x + image_offset_.x, pos.y + image_offset_.y, 0);
    animation_->Render(image_size_.x, image_size_.y, (direction_ == kLeft), false);
    glPopMatrix();
}

//
//
void Character::OnKeyDown(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            if (grounded_) {
                state_ = kMoveLeft;
            }
            direction_ = kLeft;
            break;
        case SDLK_RIGHT:
            if (grounded_) {
                state_ = kMoveRight;
            }
            direction_ = kRight;
            break;
        case SDLK_SPACE:
            if (grounded_) {
                state_ = kJump;
            }
        default:
            break;
    }
}

//
//
void Character::OnKeyUp(SDL_KeyboardEvent *evt)
{
    state_ = kIdle;
}

}

