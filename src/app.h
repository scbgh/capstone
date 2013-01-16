//
// app.h
// Handles the main game window, including the main SDL loop.
//

#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>

namespace pg {

//
// class App
// Abstracts the basic SDL game loop and main window.
//
class App {
public:
    App(int width, int height);

    // Start the SDL game loop
    void Run();
private:
    SDL_Surface *surface; // the main SDL window surface
    bool has_quit; // should we quit the program?

    // Paint the window
    void Paint();

    // Event handling
    void OnKeyDown(SDL_KeyboardEvent *evt);
    void OnKeyUp(SDL_KeyboardEvent *evt);
    void OnMouseMove(SDL_MouseMotionEvent *evt);
    void OnMouseDown(SDL_MouseButtonEvent *evt);
    void OnMouseUp(SDL_MouseButtonEvent *evt);

    // Non-copyable
    App(const App&);
    App& operator=(const App&);
};

};

#endif
