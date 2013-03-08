/*
 * character.h
 * Holds information about a character including animation and physics.
 */

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <memory>
#include <Box2D/Box2D.h>
#include "mapfile.h"
#include "animation.h"
#include "math/math.h"

namespace pg {

class App;

enum CharacterState { kMoveLeft, kMoveRight, kJump, kIdle };
enum CharacterDirection { kLeft, kRight };

class Character {
public:
    Character(App *app);
    virtual void Step(double time);
    virtual void Render() const;

    void SetPosition(double x, double y) { body_->SetTransform(b2Vec2(x, y), 0); }
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);

    bool grounded() const { return grounded_; }
    void set_grounded(bool value) { grounded_ = value; }

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
protected:
    App *app_;
    b2Body *body_;
    std::unique_ptr<BodyData> data_;
    std::unique_ptr<Animation> animation_;
    
    double walk_speed_;
    double jump_speed_;
    math::Point image_offset_;
    math::Point image_size_;

    CharacterState state_;
    CharacterDirection direction_;
    bool grounded_;
};

}

#endif

