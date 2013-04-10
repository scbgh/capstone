//
//  renderer.cpp
//

#include <SDL_ttf.h>
#include "app.h"
#include "common.h"
#include "mapfile.h"
#include "math.h"
#include "pack.h"
#include "renderer.h"
#include "sprite.h"
#include "world.h"

namespace pg {

//
//
Renderer::Renderer(App *app) :
    app_(app),
    font_(nullptr),
    view_upper_left_(0.0, 22.0),
    view_lower_right_(40.0, 0.0)
{
}

//
//
Renderer::~Renderer()
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

    TTF_Init();

    surface_ = SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
    if (!surface_) {
        Die("Could not create SDL surface");
    }

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    width_ = width;
    height_ = height;

    Pack& pack = app_->pack();
    if (pack.contains("fonts/VeraBd.ttf")) {
        PackEntry entry = pack["fonts/VeraBd.ttf"];
        SDL_RWops *rwops = SDL_RWFromMem(entry.data, entry.length);
        font_ = TTF_OpenFontRW(rwops, 1, 16);
    }
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
    Sprite *fore_sprite = app_->GetSprite(world.map_file()->fore_image);
    Sprite *back_sprite = app_->GetSprite(world.map_file()->back_image);

    back_sprite->Render(40, 22);

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

    // Draw the goal
    b2Fixture *goal_fixture = world.goal_fixture();
    if (goal_fixture) {
        b2Vec2 pos = goal_fixture->GetBody()->GetPosition();
        glPushMatrix();
        glTranslated(pos.x, pos.y, 0);
        glRotated(fmod(world.time(), 5) / 5 * 360, 0, 0, 1);
        glTranslated(-0.5, -0.5, 0);
        app_->GetSprite("graphics/goal.png")->Render(1, 1);
        glPopMatrix();
    }


    fore_sprite->Render(40, 22);

    if (app_->ShouldRenderDebug()) {
        world.DrawDebug();
    }
    world.DrawCharacters();

    // Draw the character portraits
    glPushMatrix();
    glTranslated(0.5, 0.5, 0);
    if (!world.alt_control()) {
        string chars[] = { "heavy", "jump", "strong", "ranger" };
        for (auto& ch : chars) {
            Sprite *portrait = app_->GetSprite(string("graphics/") + ch + "portrait.png");
            portrait->Render(portrait->width() / 32.0, portrait->height() / 32.0);

            if (world.active_character() == ch) {
                glPushMatrix();
                Sprite *border = app_->GetSprite("graphics/portraitborder.png");
                glTranslated((portrait->width() - border->width()) * 0.5 / 32.0,
                    (portrait->height() - border->height()) * 0.5 / 32.0, 0);
                if ((int)(world.time() * 10) % 2 == 0) {
                    border->Render(border->width() / 32.0, border->height() / 32.0);
                }
                glPopMatrix();
            }
            
            glTranslated(portrait->width() / 32.0 + 0.5, 0, 0);
        }
    } else {
        int k = 0;
        string chars[] = { "strong", "ranger", "heavy", "jump" };
        for (auto& ch : chars) {
            Sprite *portrait = app_->GetSprite(string("graphics/") + ch + "portrait.png");
            portrait->Render(portrait->width() / 32.0, portrait->height() / 32.0);
            k++;
            if (k % 2 != 0) {
                glTranslated(portrait->width() / 32.0 + 0.5, 0, 0);
            } else {
                glTranslated(-portrait->width() / 32.0 - 0.5, portrait->height() / 32.0 + 0.5, 0);
            }
        }
    }
    glPopMatrix();

    if (world.complete()) {
        glColor4d(1.0, 1.0, 1.0, 0.5);
        glBegin(GL_QUADS);
            glVertex2d(0, 0);
            glVertex2d(40, 0);
            glVertex2d(40, 22);
            glVertex2d(0, 22);
        glEnd();
        glColor4d(1.0, 1.0, 1.0, 1.0);

        Sprite *complete = app_->GetSprite("graphics/complete.png");
        double w = complete->width() / 32.0;
        double h = complete->height() / 32.0;
        glTranslated(20 - w / 2.0, 11 - h / 2.0, 0);
        complete->Render(complete->width() / 32.0, complete->height() / 32.0);
    }

    glPopMatrix();
}

//
//
void Renderer::DrawText(const std::string& text, double x, double y)
{
    SDL_Surface *surface = TTF_RenderText_Blended(font_, text.c_str(), {0, 0, 0});
    unique_ptr<Sprite> sprite;
    
    sprite.reset(new Sprite(surface));

    // Draw drop shadow
    glPushMatrix();
    glTranslated(x+0.02, y-0.02, 0);
    sprite->Render(surface->w / 32.0, surface->h / 32.0);
    glPopMatrix();
    SDL_FreeSurface(surface);

    // Draw text
    surface = TTF_RenderText_Blended(font_, text.c_str(), {255,255,255});
    sprite.reset(new Sprite(surface));
    glPushMatrix();
    glTranslated(x, y, 0);
    sprite->Render(surface->w / 32.0, surface->h / 32.0);
    glPopMatrix();
    SDL_FreeSurface(surface);
}

}
