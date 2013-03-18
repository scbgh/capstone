/*
 * animation.h
 * Holds definition information for animated sprites.
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "sprite.h"
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace pg {

class App;

//
//
struct AnimationFrame {
    int index;
    double duration;
    bool flip_h;
    bool flip_v;
};

//
//
struct AnimationState {
    std::vector<AnimationFrame> frames;
    bool repeat;
};

//
//! Holds information about an animated sprite.
class Animation {
public:
    //! Construct a new animation
    //! \param app The App to which this animation belongs
    Animation(App *app);

    //! Load this animation from a JSON definition file in the application's pack file
    //! \param animation_name The name of the animation within the pack file's "pack" directory
    void LoadAnimation(const std::string& animation_name);
    //! Advance the animation forward by a specified number of seconds
    //! \param time The number of seconds to move the animation.
    void Step(double time);
    //! Render this animation at the origin
    //! \param w The width in world units of the drawn sprite
    //! \param h The height in world units of the drawn sprite
    //! \param flip_h If true, the sprite will be drawn flipped horizontally
    //! \param flip_v If true, the sprite will be drawn flipped vertically
    void Render(double w, double h, bool flip_h, bool flip_v) const;
    //! Set the state of the animation
    //! \param name The name of the state as defined in the JSON definition file
    void SetState(const std::string& name);

    Animation(const Animation&) = delete;
    Animation& operator=(const Animation&) = delete;
private:
    App *app_;
    std::string cur_state_name_;
    std::unique_ptr<Sprite> sprite_sheet_;
    std::map<std::string, AnimationState> states_;
    int cell_width_;
    int cell_height_;
    double time_;
    double state_duration_;
    int frame_;
    AnimationState cur_state_;
};

}

#endif

