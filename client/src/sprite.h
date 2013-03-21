//
// sprite.h
// Functionality for loading and rendering images using OpenGL.
//

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
#include "common.h"

namespace pg {

//! Wraps and draws a single image 
class Sprite { 
public:
    //! Create a new Sprite from image data
    //! \param data The raw bytes of the image in a format that SDL_image can read
    //! \param size The size in bytes of the image
    Sprite(SDL_Surface *surface);
    Sprite(void *data, int size);
    ~Sprite();
    
    Sprite(const Sprite&) = delete;
    Sprite& operator=(const Sprite&) = delete;

    //! Render the sprite at the origin
    //! \param w The width in world coordinates to render the sprite
    //! \param h The height in world coordinates to render the sprite
    void Render(double w, double h) const;
    //! Render the sprite at the origin
    //! \param sx The x-coordinate in texture coordinates [0.0, 1.0] of the source rectangle
    //! \param sy The y-coordinate in texture coordinates [0.0, 1.0] of the source rectangle
    //! \param sw The width in texture coordinates [0.0, 1.0] of the source rectangle
    //! \param sh The height in texture coordinates [0.0, 1.0] of the source rectangle
    //! \param w The width in world coordinates to render the sprite
    //! \param h The height in world coordinates to render the sprite
    void Render(double sx, double sy, double sw, double sh, double w, double h) const;

    //! The width of this sprite in pixels
    int width() const { return width_; }
    //! The height of this sprite in pixels
    int height() const { return height_; }
private:
    void InitFromSurface(SDL_Surface *surface);

    int valid_;
    GLuint texture_;
    double width_;
    double height_;
};

}

#endif

