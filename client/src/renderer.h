//
// renderer.h
// Handles all rendering functionality.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "math/math.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>

namespace pg {

class App;
class Sprite;

//
//! Holds rendering state information and handles all application-level rendering tasks
class Renderer {
public:
    //! Create a new renderer
    //! \param app The App to which this renderer belongs
    explicit Renderer(App *app);
    ~Renderer();

    //! Initialize the SDL window and rendering surface
    //! \param width The desired window width
    //! \param height The desired window height
    void Init(int width, int height);
    //! Render the current application state
    void Render();
    //! Draw some text on the screen
    //! \param text The text to draw
    //! \param x The x coordinate at which to draw
    //! \param y The y coordinate at which to draw
    void DrawText(const std::string& text, double x, double y);
private:
    App *app_; // the global app
    SDL_Surface *surface_; // the main window SDL surface
    int width_, height_; // the width and height of the main window
    math::Point view_upper_left_; // the upper left corner of the camera
    math::Point view_lower_right_; // the lower right corner of the camera
    TTF_Font *font_; // font

    // Non-copyable
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

}

#endif

