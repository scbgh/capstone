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

enum CharacterState { kIdle = 0, kMoveLeft = 1, kMoveRight = 2, kJump = 4 };
enum CharacterDirection { kLeft, kRight };

class Character {
public:
    Character(App *app);
    virtual void Step(double time);
    virtual void Render() const;

    void SetPosition(double x, double y) { body_->SetTransform(b2Vec2(x, y), 0); }
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);

    void BeginFootContact();
    void EndFootContact();
    void BeginGoalContact() { at_goal_ = true; }
    void EndGoalContact() { at_goal_ = false; }
    
    bool at_goal() const { return at_goal_; }

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
protected:
    App *app_;
    b2Body *body_;
    b2Body *bottom_body_;
    std::unique_ptr<BodyData> data_;
    std::unique_ptr<Animation> animation_;
    
    double walk_speed_;
    double jump_speed_;
    math::Point image_offset_;
    math::Point image_size_;

    int foot_contacts_;
    int state_;
    CharacterDirection direction_;
    bool grounded_;
    bool at_goal_;
};

}

#endif

