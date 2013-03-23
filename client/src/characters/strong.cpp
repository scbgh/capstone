/*
 * strong.cpp
 */

#include "app.h"
#include "common.h"
#include "strong.h"
#include "world.h"

namespace pg {

//
//
StrongCharacter::StrongCharacter(App *app) :
    Character(app),
    launch_force_(1.0),
    launch_angle_(45),
    grab_(false),
    grab_joint_(nullptr)
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
    body_def.bullet = true;
    body_def.userData = data;
    body_ = phys_world->CreateBody(&body_def);

    b2FixtureDef fixture_def;
    b2Fixture *fixture;
    fixture_def.friction = 1;
    fixture_def.density = 145;
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
    jump_speed_ = 320;

    animation_.reset(new Animation(app));
    animation_->LoadAnimation("strong");
    animation_->SetState("stand");

    delete shape;
}

//
//
void StrongCharacter::OnKeyDown(SDL_KeyboardEvent *evt)
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
                Character *overlap = GetOverlapping();
                if (overlap) {
                    grab_ = true;
                    grabbed_character_ = overlap;
                    grabbed_character_->Freeze();
                    CreateGrabJoint(grabbed_character_->body());
                }
            }
        default:
            break;
    }
}

//
//
void StrongCharacter::OnKeyUp(SDL_KeyboardEvent *evt)
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
            if (grab_ && !fixed_) {
                if (grab_joint_) {
                    app_->world().phys_world()->DestroyJoint(grab_joint_);
                    grab_joint_ = nullptr;
                }
                if (grabbed_character_) {
                    float dir = direction_ == kLeft ? -1 : 1;
                    b2Vec2 vel = 600 * b2Vec2(dir * cos(DEG_TO_RAD(launch_angle_)), sin(DEG_TO_RAD(launch_angle_)));

                    grabbed_character_->Unfreeze();
                    grabbed_character_->body()->ApplyLinearImpulse(launch_force_ * vel,
                        grabbed_character_->body()->GetPosition());
                    grabbed_character_ = nullptr;
                }
            }
            grab_ = false;
            break;
        default:
            break;
    }
}

//
//
void StrongCharacter::Step(double time)
{
    animation_->Step(time);

    if (!grab_ && !(state_ & kAction)) {
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

    if (state_ & kAction) {
        animation_->SetState("grab");
    } else if (IsGrounded() && !(state_ & (kMoveLeft | kMoveRight))) {
        animation_->SetState("stand");
    }

}

//
//
void StrongCharacter::Render() const
{
    Character::Render();
    
    // Draw the launch arrow if necessary
    if (!grab_ || fixed_) return;
    double dir = direction_ == kLeft ? -1 : 1; 
    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x + dir * 0.5, pos.y + 0.25, 0);
    glScaled(dir, 1, 1);
    glRotated(launch_angle_, 0, 0, 1);
    glColor4d(1.0, 1.0, 0.0, 1.0);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
        glVertex2d(0.0, 0.0);
        glVertex2d(launch_force_, 0.0);
    glEnd();
    glLineWidth(1.0f);
    glColor4d(1.0, 1.0, 1.0, 1.0);
    glPopMatrix();
}

//
//
Character *StrongCharacter::GetOverlapping() const
{
    for (b2ContactEdge *edge = body_->GetContactList(); edge; edge = edge->next) {
        BodyData *data = (BodyData *)edge->other->GetUserData();

        if (!edge->contact->IsTouching()) continue;
        if (!data || data->type != kCharacterBody) continue;

        return data->data.character_body;
    }

    return nullptr;
}

//
//
void StrongCharacter::CreateGrabJoint(b2Body *other)
{
    b2RevoluteJointDef joint_def;
    float dir = direction_ == kLeft ? -1 : 1;
    b2Vec2 pos(dir * 0.5, 0.25);
    joint_def.bodyA = body_;
    joint_def.bodyB = other;
    joint_def.localAnchorA = pos;
    joint_def.localAnchorB = b2Vec2(0, 0);
    grab_joint_ = app_->world().phys_world()->CreateJoint(&joint_def);
}

}

