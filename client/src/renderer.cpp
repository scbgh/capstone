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
    app_(app),
    view_upper_left_(0.0, 22.0),
    view_lower_right_(40.0, 0.0)
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

    surface_ = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    if (!surface_) {
        Die("Could not create SDL surface");
    }

    glViewport(0, 0, width, height);

    width_ = width;
    height_ = height;
}

//
// Render the current game state
void Renderer::Render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width_ / (float)height_;
    gluOrtho2D(view_upper_left_.x, view_lower_right_.x,
        view_lower_right_.y, view_upper_left_.y);

    app_->world().DrawDebug();
}

}
