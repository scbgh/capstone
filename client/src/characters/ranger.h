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
private:
    float launch_angle_;
    bool launch_;
};

}

#endif

