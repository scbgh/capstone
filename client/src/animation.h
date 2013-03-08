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
//
class Animation {
public:
    Animation(App *app);

    void LoadAnimation(const std::string& animation_name);
    void Step(double time);
    void Render(double w, double h, bool flip_h, bool flip_v) const;
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

