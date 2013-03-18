//
// app.h
// Handles the main game window, including the main SDL loop.
//

#ifndef _APP_H_
#define _APP_H_

#include "animation.h"
#include "common.h"
#include <memory>
#include <string>

namespace pg {

class Renderer;
class World;
class Pack;

//
//! Contains the main SDL game loop.
class App {
public:
    //! Create and initialize a new App
    //! \param argc The number of command line arguments to pass in
    //! \param argv The command line arguments from the main function
    //! \param width The width of the game window
    //! \param height The height of the game window
    App(int argc, char *argv[], int width, int height);
    ~App();

    //! The renderer that holds the current SDL window
    Renderer& renderer() { return *renderer_; }
    //! The current world state
    World& world() { return *world_; }
    //! The pack file as loaded from disk
    Pack& pack() { return *pack_; }

    //! Start the SDL game loop and show the window
    void Run();

    //! If true, extra lines will be drawn displaying the Box2D bodies
    bool ShouldRenderDebug() const { return should_render_debug_; }
private:
    bool has_quit_; // should we quit the program?

    std::unique_ptr<Renderer> renderer_; // the renderer for the game state
    std::unique_ptr<World> world_; // the current state of the game world
    std::unique_ptr<Pack> pack_; // pack file containing the game resources

    int last_tick_; // the last recorded tick count
    bool running_; // is the game running yet?
    bool should_render_debug_; // should render debug?

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

}

#endif
