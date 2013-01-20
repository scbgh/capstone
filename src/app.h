//
// app.h
// Handles the main game window, including the main SDL loop.
//

#ifndef _APP_H_
#define _APP_H_

#include <SDL.h>
#include <string>

namespace pg {

class Renderer;
class World;

//
// class App
// Abstracts the basic SDL game loop and main window.
//
class App {
public:
    App(int argc, char *argv[], int width, int height);
    ~App();

    // Start the SDL game loop
    void Run();
private:
    bool has_quit; // should we quit the program?

    Renderer *renderer; // the renderer for the game state
    World *world; // the current state of the game world

    // Handle command line args
    void ParseArgs(int argc, char *argv[]);

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
