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
    zoom_(1.f)
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    width_ = width;
    height_ = height;
    float aspect = (float)width_ / (float)height_;
    gluOrtho2D(-aspect, aspect, -1.f, 1.f);

    zoom_ = 0.05;
}

//
// Render the current game state
void Renderer::Render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(zoom_, zoom_, 0.f);
    glTranslatef(-view_center_.x, -view_center_.y, 0.f);

    app_->world().DrawDebug();
}

}
