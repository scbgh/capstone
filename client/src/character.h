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

//! A bit flag representing the current movement states that a character is in
enum CharacterState { kIdle = 0, kMoveLeft = 1, kMoveRight = 2, kJump = 4, kAction = 8 };

//! A value representing the direction that a character is facing
enum CharacterDirection { kLeft, kRight };

//! Represents a generic player-controlled character
class Character {
public:
    //! Create a new Character
    //! \param app The App in which this character exists
    Character(App *app);

    //! Steps this character forward in time
    //! \param time The number of seconds by which to step forward
    virtual void Step(double time);
    //! Render this character on the screen
    virtual void Render() const;

    //! Move this character to an absolute position in world coordinates
    void SetPosition(double x, double y) { body_->SetTransform(b2Vec2(x, y), 0); }

    //! Handle an SDL key down event
    //! \param evt The event as generated by SDL
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);

    //! Handle an SDL key up event
    //! \param evt The event as generated by SDL
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);

    //! Returns true if the character is standing on the ground
    bool IsGrounded() const;

    //! Returns true if the character is touching the goal
    bool AtGoal() const;

    //! Prevent the character from moving
    void Freeze() { fixed_ = true; }

    //! Resume character movement
    void Unfreeze() { fixed_ = false; }

    //! Explode
    void Explode();
    
    //! The Box2D body
    b2Body *body() const { return body_; }

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
protected:
    void DoWalk(double time);

    App *app_;
    b2Body *body_;
    std::unique_ptr<BodyData> data_;
    std::unique_ptr<Animation> animation_;
    
    bool fixed_;
    bool hidden_;
    double walk_speed_;
    double jump_speed_;
    math::Point image_offset_;
    math::Point image_size_;

    int state_;
    CharacterDirection direction_;
};

}

#endif

