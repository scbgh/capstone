/*
 * ranger.cpp
 */

#include "app.h"
#include "common.h"
#include "ranger.h"
#include "world.h"
#include "mapfile.h"
#include "pack.h"

namespace pg {

//
//
RangerCharacter::RangerCharacter(App *app) :
    Character(app),
    launch_(false),
    launch_force_(1.0),
    launch_angle_(45),
    time_(0.0)
{
    b2World *phys_world = app->world().phys_world();
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->cause_shake = true;
    data->data.character_body = this;

    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = 0.45;

    b2BodyDef body_def;
    body_def.fixedRotation = true;
    body_def.type = b2_dynamicBody;
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 120;
    fixture_def.shape = shape;
    fixture_def.userData = (void *)kCharacterFixture;
    body_->CreateFixture(&fixture_def);

    shape->m_radius = 0.4;
    shape->m_p = b2Vec2(0.0, -0.1);
    fixture_def.isSensor = true;
    fixture_def.density = 0;
    fixture_def.userData = (void *)kBottomFixture;
    body_->CreateFixture(&fixture_def);

    image_offset_ = { -0.5, -0.5 };
    image_size_ = { 1, 1 };
    walk_speed_ = 1.5;
    jump_speed_ = 120;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("ranger");
    animation_->SetState("stand");

    delete shape;
}

//
//
void RangerCharacter::OnKeyDown(SDL_KeyboardEvent *evt)
{
    if (fixed_) return;

    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            state_ |= kMoveLeft;
            break;
        case SDLK_RIGHT:
            state_ |= kMoveRight;
            break;
        case SDLK_SPACE:
            if (IsGrounded()) {
                state_ |= kJump;
                body_->ApplyLinearImpulse(b2Vec2(0, jump_speed_), body_->GetWorldCenter());
            }
            break;
        case SDLK_a:
            if (IsGrounded()) {
                state_ |= kAction;
                launch_ = true;
                launch_time_ = time_;
            }
        default:
            break;
    }
}

//
//
void RangerCharacter::OnKeyUp(SDL_KeyboardEvent *evt)
{
    switch (evt->keysym.sym) {
        case SDLK_LEFT:
            state_ &= ~kMoveLeft;
            break;
        case SDLK_RIGHT:
            state_ &= ~kMoveRight;
            break;
        case SDLK_SPACE:
            state_ &= ~kJump;
            break;
        case SDLK_a:
            state_ &= ~kAction;
            if (launch_ && !fixed_) {
                b2BodyDef body_def;
                b2CircleShape circle;
                b2FixtureDef fixture_def;
                double dir = direction_ == kLeft ? -1 : 1; 
                b2Vec2 offset = b2Vec2(dir * 0.5, 0);
                b2Vec2 vel = 50 * b2Vec2(dir * cos(DEG_TO_RAD(launch_angle_)), sin(DEG_TO_RAD(launch_angle_)));

                body_def.type = b2_dynamicBody;
                body_def.angularDamping = 1.0;
                circle.m_radius = 0.125;
                fixture_def.shape = &circle;
                fixture_def.friction = 0.5;
                fixture_def.restitution = 0.4;
                fixture_def.density = 101;

                b2Body *body = app_->world().phys_world()->CreateBody(&body_def);
                b2Fixture *fixture = body->CreateFixture(&fixture_def);

                Body *world_body = new Body;
                PackEntry entry = app_->pack()[std::string("graphics/ball.png")];
                world_body->image_sprite = unique_ptr<Sprite>(new Sprite(entry.data, entry.length));
                BodyData *data = new BodyData;
                data->cause_shake = false;
                data->collide_player = false;
                data->type = kWorldBody;
                data->data.world_body = world_body;
                body->SetUserData((void *)data);
                body->SetTransform(body_->GetPosition() + offset, 0);
                body->ApplyLinearImpulse(launch_force_ * vel, body->GetPosition());
                app_->world().map_file()->bodies.push_back(std::unique_ptr<Body>(world_body));
                
            }
            launch_ = false;
            break;
        default:
            break;
    }
}

//
//
void RangerCharacter::Step(double time)
{
    animation_->Step(time);
    time_ += time;

    launch_force_ = (sin(2 * M_PI * (time_ - launch_time_)) + 1.5) / 2.0;

    if (!launch_) {
        DoWalk(time);
    } else {
        double dir = direction_ == kLeft ? -1 : 1; 
        if (state_ & kMoveLeft) {
            launch_angle_ += dir * time / 2 * 90;
        } else if (state_ & kMoveRight) {
            launch_angle_ -= dir * time / 2 * 90;
        }

        if (launch_angle_ < 0) {
            launch_angle_ = 0;
        } else if (launch_angle_ > 80) {
            launch_angle_ = 80;
        }
    }

    if (IsGrounded() && !(state_ & (kMoveLeft | kMoveRight))) {
        animation_->SetState("stand");
    }
}

//
//
void RangerCharacter::Render() const
{
    Character::Render();
    
    // Draw the launch arrow if necessary
    if (!launch_ || fixed_) return;
    double dir = direction_ == kLeft ? -1 : 1; 
    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x + dir * 0.5, pos.y, 0);
    glScaled(dir, 1, 1);
    glRotated(launch_angle_, 0, 0, 1);
    glColor4d(1.0, 0.0, 0.0, 1.0);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2d(0.0, 0.0);
        glVertex2d(launch_force_, 0.0);
    glEnd();
    glLineWidth(1.0f);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    glPopMatrix();
}


}

