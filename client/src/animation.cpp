/*
 * animation.cpp
 */

#include "app.h"
#include "pack.h"
#include "common.h"
#include "animation.h"
#include "json/picojson.h"
#include <cmath>
#include <sstream>

namespace pg {

//
//
Animation::Animation(App *app) :
    app_(app)
{
}

//
//
void Animation::LoadAnimation(const string& animation_name)
{
    Pack& pack = app_->pack();
    string anim_path = "animations/" + animation_name + ".json";
    if (!pack.contains(anim_path)) {
        Die("Could not load animation '%s'", anim_path.c_str());
    }
    string json = pack[anim_path].ToString(); 

    picojson::value root_value;
    string error;
    picojson::parse(root_value, json.begin(), json.end(), &error);
    if (!error.empty()) {
        Die("Failed to parse JSON: %s", error.c_str());
    }

    auto root_object = root_value.get<picojson::object>();
    
    cell_width_ = (int)root_object["cell_width"].get<double>();
    cell_height_ = (int)root_object["cell_height"].get<double>();
    string sprite = root_object["sprite"].get<string>();
    
    if (!pack.contains(sprite)) {
        Die("Could not load animation sprite '%s'", sprite.c_str());
    }
    sprite_sheet_.reset(new Sprite(pack[sprite].data, pack[sprite].length));

    for (auto& state_value : root_object["states"].get<picojson::array>()) {
        auto state_object = state_value.get<picojson::object>();
        AnimationState state;
        string name = state_object["name"].get<string>();
        state.repeat = state_object["repeat"].is<bool>() ? state_object["repeat"].get<bool>() : false;
    
        for (auto& frame_value : state_object["frames"].get<picojson::array>()) {
            auto frame_object = frame_value.get<picojson::object>();
            AnimationFrame f;
            f.index = (int)frame_object["index"].get<double>();
            f.duration = frame_object["duration"].get<double>();
            state.frames.push_back(f);
        }

        states_[name] = state;
    }
}

//
//
void Animation::Step(double time)
{
    time_ += fmod(time, state_duration_);
    frame_ = 0;

    double stepped;
    while (stepped + cur_state_.frames[frame_].duration < time_) {
        stepped += cur_state_.frames[frame_].duration;
        frame_ = (frame_ + 1) % cur_state_.frames.size();
    }
}

//
//
void Animation::Render(double w, double h, bool flip_h, bool flip_v) const
{
    if (!sprite_sheet_) return;

    int num_cells_x = sprite_sheet_->width() / cell_width_;
    AnimationFrame cur_frame = cur_state_.frames[frame_];
    double sw = (double)cell_width_ / sprite_sheet_->width();
    double sh = (double)cell_height_ / sprite_sheet_->height();
    double sx = (cur_frame.index % num_cells_x) * sw;
    double sy = (cur_frame.index / num_cells_x) * sh;
    if (flip_h) {
        sx += sw;
        sw = -sw;
    }
    if (flip_v) {
        sy += sh;
        sh = -sh;
    }
    sprite_sheet_->Render(sx, sy, sw, sh, w, h);
}

//
//
void Animation::SetState(const std::string& name)
{
    if (cur_state_name_ == name) return;

    cur_state_name_ = name;
    cur_state_ = states_[name];
    time_ = 0;
    frame_ = 0;
    state_duration_ = 0;
    for (auto& frame : cur_state_.frames) {
        state_duration_ += frame.duration;
    }
}

}
