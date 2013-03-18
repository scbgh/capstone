/*
 * heavy.h
 * Heavy character
 */

#ifndef _HEAVY_H_
#define _HEAVY_H_

#include "character.h"

namespace pg {

//! Character-specific implementation information for the Heavy character
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

