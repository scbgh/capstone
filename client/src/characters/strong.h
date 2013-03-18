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
};

}

#endif

