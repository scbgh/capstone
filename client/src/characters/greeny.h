/*
 * greeny.h
 * Test character
 */

#ifndef _GREENY_H_
#define _GREENY_H_

#include "character.h"

namespace pg {

class Greeny : public Character {
public:
    Greeny(App *app);
    
    virtual void Step(double time);
    virtual void OnKeyDown(SDL_KeyboardEvent *evt);
    virtual void OnKeyUp(SDL_KeyboardEvent *evt);
private:
    double x_movement_;
};

}

#endif

