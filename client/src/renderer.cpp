//
//  renderer.cpp
//

#include "app.h"
#include "common.h"
#include "mapfile.h"
#include "math.h"
#include "renderer.h"
#include "world.h"

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
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    surface_ = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    if (!surface_) {
        Die("Could not create SDL surface");
    }

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    glColor4d(1.0, 1.0, 1.0, 1.0);

    // Compute screen shake
    glPushMatrix();
    math::Vector vec = app_->world().ShakeMagnitude();
    glTranslated(vec.x, vec.y, 0);

    float aspect = (float)width_ / (float)height_;
    gluOrtho2D(view_upper_left_.x, view_lower_right_.x,
        view_lower_right_.y, view_upper_left_.y);

    // Draw the map image
    World& world = app_->world();
    if (world.back_sprite()) {
        world.back_sprite()->Render(40, 22);
    }

    // Draw the sprites for each fixture
    for (b2Body *phys_body = world.phys_world()->GetBodyList(); phys_body != NULL; phys_body = phys_body->GetNext()) {
        BodyData *body_data = (BodyData *)phys_body->GetUserData();
        if (!body_data || body_data->type != kWorldBody) continue;
        Body *body = body_data->data.world_body;
        if (!body || !body->image_sprite) continue;

        double w = body->image_sprite->width() / 32.0;
        double h = body->image_sprite->height() / 32.0;
        b2Vec2 pos = phys_body->GetPosition();
        glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glRotated(RAD_TO_DEG(phys_body->GetAngle()), 0, 0, 1);
        glTranslated(body->image_offset.x, body->image_offset.y, 0);
        glTranslated(-w / 2, -h / 2, 0);
        body->image_sprite->Render(w, h);
        glPopMatrix();
    }

    if (world.fore_sprite()) {
        world.fore_sprite()->Render(40, 22);
    }

    if (app_->ShouldRenderDebug()) {
        world.DrawDebug();
    }
    world.DrawCharacters();

    glPopMatrix();
}

}
