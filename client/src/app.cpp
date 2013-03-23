//
// window.cpp
//

#include "animation.h"
#include "app.h"
#include "character.h"
#include "common.h"
#include "file.h"
#include "pack.h"
#include "renderer.h"
#include "script.h"
#include "scriptstate.h"
#include "sprite.h"
#include "world.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cstdio>

namespace pg {

//
//
App::App(int argc, char *argv[], int width, int height) :
    has_quit_(false),
    renderer_(unique_ptr<Renderer>(new Renderer(this))),
    world_(unique_ptr<World>(new World(this))),
    last_tick_(SDL_GetTicks()),
    running_(false),
    should_render_debug_(false)
{
    Debug("Initializing App");

    // Load game packfile
    string pack_filename = AppendPath(GetExecutableDirectory(), "game.dat");
    pack_ = unique_ptr<Pack>(new Pack(pack_filename));
    renderer_->Init(width, height);
    ParseArgs(argc, argv);
}

//
//
App::~App()
{
    SDL_Quit();
    TTF_Quit();
}

//
// Start the SDL main loop
void App::Run()
{
    SDL_Event event;

    while (!has_quit_) {
        // Process the SDL events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    has_quit_ = true;
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

        // Clear the screen
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Step the world state forward
        int tick = SDL_GetTicks();
        if (running_) {
            int delta = tick - last_tick_;
            if (world_->initialized()) {
                world_->Step(delta / 1000.f);
            }
        } else {
            running_ = true;
        }
        last_tick_ = tick;

        renderer_->Render();
        SDL_GL_SwapBuffers();
    }
}

//
//
Sprite *App::GetSprite(const string& path)
{
    if (sprites_.find(path) == sprites_.end()) {
        if (pack_->contains(path)) {
            PackEntry entry = (*pack_)[path];
            sprites_[path] = unique_ptr<Sprite>(new Sprite(entry.data, entry.length));
        } else {
            Debug("Failed to load sprite: %s", path.c_str());
            sprites_[path] = unique_ptr<Sprite>(new Sprite);
        }
    }
    return sprites_[path].get();
}

//
//
void App::OnKeyDown(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_d:
            should_render_debug_ = !should_render_debug_;
            break;
        case SDLK_r:
            if (evt->keysym.mod == KMOD_LSHIFT) {
                world_->LoadMap(map_name_);
            }
            break;
        default:
            break;
    }
    world_->OnKeyDown(evt);
}

//
//
void App::OnKeyUp(SDL_KeyboardEvent *evt)
{
    world_->OnKeyUp(evt);
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
        Debug("Loading map '%s'...\n", argv[1]);
        world_->LoadMap(argv[1]);
        map_name_ = argv[1];
    }
}


}
