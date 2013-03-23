//
// sprite.cpp
//

#include "common.h"
#include "sprite.h"
#include <SDL_image.h>

namespace pg {

//
//
Sprite::Sprite() :
    valid_(0)
{
}

//
//
Sprite::Sprite(SDL_Surface *surface)
{
    InitFromSurface(surface);
}

//
//
Sprite::Sprite(void *data, int size) :
    valid_(0)
{
    SDL_RWops *rwops = SDL_RWFromMem(data, size);
    SDL_Surface *surface = IMG_Load_RW(rwops, 1);

    if (surface) {
        InitFromSurface(surface);
        SDL_FreeSurface(surface);
    }
}

//
//
Sprite::~Sprite()
{
    if (valid_) glDeleteTextures(1, &texture_);
}

//
//
void Sprite::InitFromSurface(SDL_Surface *surface)
{
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    width_ = surface->w;
    height_ = surface->h;
    glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    valid_ = 1;
}

//
//
void Sprite::Render(double w, double h) const
{
    if (!valid_) return;

    glBindTexture(GL_TEXTURE_2D, texture_);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(0, 0);
        glTexCoord2f(1, 1); glVertex2f(w, 0);
        glTexCoord2f(1, 0); glVertex2f(w, h);
        glTexCoord2f(0, 0); glVertex2f(0, h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//
//
void Sprite::Render(double sx, double sy, double sw, double sh, double w, double h) const
{
    if (!valid_) return;

    glBindTexture(GL_TEXTURE_2D, texture_);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
        glTexCoord2f(sx, sy+sh); glVertex2f(0, 0);
        glTexCoord2f(sx+sw, sy+sh); glVertex2f(w, 0);
        glTexCoord2f(sx+sw, sy); glVertex2f(w, h);
        glTexCoord2f(sx, sy); glVertex2f(0, h);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

}

