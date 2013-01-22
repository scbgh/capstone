//
// renderer.h
// Handles all rendering functionality.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <SDL.h>
#include "math/math.h"

namespace pg {

class App;

//
// Holds all the rendering state information
class Renderer {
public:
    explicit Renderer(App *app);

    void Init(int width, int height);
    void Render();
private:
	App *app_; // the global app
    SDL_Surface *surface_; // the main window SDL surface
    int width_, height_; // the width and height of the main window
    math::Point view_center_; // the center of the camera
    float zoom_; // the zoom factor

    // Non-copyable
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

}

#endif

