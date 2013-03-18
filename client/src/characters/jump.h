/*
 * jump.h
 * Jump character
 */

#ifndef _JUMP_H_
#define _JUMP_H_

#include "character.h"

namespace pg {

//! Character-specific implementation information for the Agile character
class JumpCharacter : public Character {
public:
    JumpCharacter(App *app);
};

}

#endif

