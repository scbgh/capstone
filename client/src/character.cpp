/*
 * character.cpp
 */

#include "common.h"
#include "character.h"

namespace pg {

//
//
Character::Character(App *app) :
    app_(app),
    facing_left_(false)
{
}

//
//
void Character::Step(double time)
{
    animation_->Step(time);
}

//
//
void Character::Render() const
{
    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x + image_offset_.x, pos.y + image_offset_.y, 0);
    animation_->Render(image_size_.x, image_size_.y, facing_left_, false);
    glPopMatrix();
}

//
//
void Character::OnKeyDown(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            facing_left_ = true;
            break;
        case SDLK_RIGHT:
            facing_left_ = true;
            break;
        default:
            break;
    }
}

//
//
void Character::OnKeyUp(SDL_KeyboardEvent *evt)
{
}

}

