/*
 * strong.h
 * Strong character
 */

#ifndef _STRONG_H_
#define _STRONG_H_

#include "character.h"

namespace pg {

//! Character-specific implementation information for the Strong character
class StrongCharacter : public Character {
public:
    StrongCharacter(App *app);
private:
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);
    virtual void Step(double time);
    virtual void Render() const;

    Character *GetOverlapping() const;
    void CreateGrabJoint(b2Body *other);

    double launch_angle_;
    double launch_force_;
    b2Joint *grab_joint_;
    bool grab_;

    Character *grabbed_character_;
};

}

#endif

