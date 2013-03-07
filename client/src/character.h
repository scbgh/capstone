/*
 * character.h
 * Holds information about a character including animation and physics.
 */

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <memory>
#include <Box2D/Box2D.h>
#include "animation.h"
#include "math/math.h"

namespace pg {

class App;

class Character {
public:
    Character(App *app);
    virtual void Step(double time);
    virtual void Render() const;

    void SetPosition(double x, double y) { body_->SetTransform(b2Vec2(x, y), 0); }
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
protected:
    App *app_;
    b2Body *body_;
    math::Point image_offset_;
    math::Point image_size_;
    std::unique_ptr<Animation> animation_;
    bool facing_left_;
};

}

#endif

