//
// window.cpp
//

#include <cstdio>
#include <SDL.h>
#include "common.h"
#include "app.h"

namespace pg {

//
//
App::App(int width, int height) :
    has_quit(false),
    renderer(new Renderer())
{
    renderer->Init(width, height);
}

//
//
App::~App()
{
    delete renderer;
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


}
