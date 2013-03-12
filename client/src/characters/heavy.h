/*
 * heavy.h
 * Heavy character
 */

#ifndef _HEAVY_H_
#define _HEAVY_H_

#include "character.h"

namespace pg {

class HeavyCharacter : public Character {
public:
    HeavyCharacter(App *app);
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);
    virtual void Step(double time);
private:
    bool rolling_;
};

}

#endif

