//
// window.cpp
//

#include <SDL.h>
#include "common.h"
#include "window.h"

namespace pg {

//
//
Window::Window(int width, int height)
{
    // Create the window
    surface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE);
    if (!surface) {
        die("Could not create SDL surface");
    }
}

//
// Start the SDL main loop
void Window::Run()
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
    }
}

//
//
void Window::OnKeyDown(SDL_KeyboardEvent *evt)
{
}

//
//
void Window::OnKeyUp(SDL_KeyboardEvent *evt)
{
}

//
//
void Window::OnMouseMove(SDL_MouseMotionEvent *evt)
{
}

//
//
void Window::OnMouseDown(SDL_MouseButtonEvent *evt)
{
}

//
//
void Window::OnMouseUp(SDL_MouseButtonEvent *evt)
{
}


}