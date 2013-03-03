//
// sprite.cpp
//

#include <SDL_image.h>
#include "common.h"
#include "sprite.h"

namespace pg {

//
//
Sprite::Sprite(void *data, int size) :
    valid_(0)
{
    SDL_RWops *rwops = SDL_RWFromMem(data, size);
    SDL_Surface *surface = IMG_Load_RW(rwops, 1);

    if (surface) {
        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);

        glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_FreeSurface(surface);
        valid_ = 1;
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

}

