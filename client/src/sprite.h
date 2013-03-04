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

    int width() const { return width_; }
    int height() const { return height_; }
private:
    int valid_;
    GLuint texture_;
    double width_;
    double height_;
};

}

#endif

