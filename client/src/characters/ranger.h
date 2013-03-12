/*
 * ranger.h
 * Ranger character
 */

#ifndef _RANGER_H_
#define _RANGER_H_

#include "character.h"

namespace pg {

class RangerCharacter : public Character {
public:
    RangerCharacter(App *app);
    
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);
    virtual void Step(double time);
    virtual void Render() const;
private:
    double launch_angle_;
    double launch_force_;
    double launch_time_;
    double time_;

    bool launch_;
};

}

#endif

