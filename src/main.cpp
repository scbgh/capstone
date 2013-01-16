//
//  main.cpp
//  Main entry point for the game. Handles main SDL loop and setup code.
//

#include <cstdio>
#include <SDL.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(SDL_Quit);

    return 0;
}