//
// sprite.h
// Functionality for loading and rendering images using OpenGL.
//

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>

namespace pg {

class Sprite { 
public:
    Sprite(void *data, int size);
    ~Sprite();
    
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    void Render(double w, double h) const;
private:
    int valid_;
    GLuint texture_;
    GLfloat texcoord_[4];

    GLuint LoadTexture(SDL_Surface *surface);
};

}

#endif

