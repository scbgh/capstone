//
// window.cpp
//

#include <cstdio>
#include <SDL.h>
#include "common.h"
#include "app.h"
#include "renderer.h"
#include "world.h"

namespace pg {

//
//
App::App(int argc, char *argv[], int width, int height) :
    has_quit(false),
    renderer(new Renderer(this)),
    world(new World(this))
{
    renderer->Init(width, height);
    ParseArgs(argc, argv);
}

//
//
App::~App()
{
    delete renderer;
    delete world;
}

//
// Start the SDL main loop
void App::Run()
{
    SDL_Event event;

    while (!has_quit) {
        // Process the SDL events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    has_quit = true;
                    break;
                case SDL_KEYDOWN:
                    OnKeyDown(&event.key);
                    break;
                case SDL_KEYUP:
                    OnKeyUp(&event.key);
                    break;
                case SDL_MOUSEMOTION:
                    OnMouseMove(&event.motion);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    OnMouseDown(&event.button);
                    break;
                case SDL_MOUSEBUTTONUP:
                    OnMouseUp(&event.button);
                    break;
            }
        }

        renderer->Render();
        SDL_GL_SwapBuffers();
    }
}

//
//
void App::OnKeyDown(SDL_KeyboardEvent *evt)
{
}

//
//
void App::OnKeyUp(SDL_KeyboardEvent *evt)
{
}

//
//
void App::OnMouseMove(SDL_MouseMotionEvent *evt)
{
}

//
//
void App::OnMouseDown(SDL_MouseButtonEvent *evt)
{
}

//
//
void App::OnMouseUp(SDL_MouseButtonEvent *evt)
{
}

//
//
void App::ParseArgs(int argc, char *argv[])
{
    // The first argument is the name of the map to warp to, if any
    if (argc > 1) {
        // TODO: make this work
    }
}


}
