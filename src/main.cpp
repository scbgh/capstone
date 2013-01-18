//
//  main.cpp
//  Main entry point for the game.
//

#include <cstdio>
#include <SDL.h>
#include "common.h"
#include "app.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Die("Failed to initialize SDL: %s\n", SDL_GetError());
    }
    atexit(SDL_Quit);

    // Execute the main game loop
    pg::App app(1280, 720);
    app.Run();

    return 0;
}
