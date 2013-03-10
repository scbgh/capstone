//
// renderer.h
// Handles all rendering functionality.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "math/math.h"
#include <SDL.h>
#include <memory>

namespace pg {

class App;
class Sprite;

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
    math::Point view_upper_left_; // the upper left corner of the camera
    math::Point view_lower_right_; // the lower right corner of the camera
    std::unique_ptr<Sprite> goal_sprite_;

    // Non-copyable
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

}

#endif

