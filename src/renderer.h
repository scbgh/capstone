//
// renderer.h
// Handles all rendering functionality.
//

#ifndef _RENDERER_H_
#define _RENDERER_H_

namespace pg {

//
// class Renderer
// Holds all the rendering state information
//
class Renderer {
public:
    Renderer();

private:


    // Non-copyable
    Renderer(const Renderer&);
    Renderer& operator=(const Renderer&);
};

}


#endif
