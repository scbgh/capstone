//
//  renderer.cpp
//

#include "common.h"
#include "renderer.h"
#include "app.h"

namespace pg {

//
//
Renderer::Renderer(App *app) :
    app(app)
{
}

//
// Initialize the game state
void Renderer::Init(int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    surface = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    if (!surface) {
        Die("Could not create SDL surface");
    }
}

//
// Render the current game state
void Renderer::Render()
{
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

}
