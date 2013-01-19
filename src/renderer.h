//
// renderer.h
// Handles all rendering functionality.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL.h>
#include "math/math.h"

namespace pg {

//
// class Renderer
// Holds all the rendering state information
//
class Renderer {
public:
    Renderer();

    void Init(int width, int height);
    void Render();
private:
    SDL_Surface *surface; // the main window SDL surface

    // Non-copyable
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

}

#endif

