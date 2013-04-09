/*
 * character.cpp
 */

#include "character.h"
#include "common.h"
#include "mapfile.h"
#include "world.h"
#include "app.h"

namespace pg {

//
//
Character::Character(App *app) :
    app_(app),
    direction_(kRight),
    fixed_(false),
    hidden_(false)
{
    BodyData *data = new BodyData;
    data->type = kCharacterBody;
    data->collide_player = false;
    data->data.character_body = this;
    state_ = kIdle;
}

//
//
bool Character::IsGrounded() const
{
    for (b2ContactEdge *edge = body_->GetContactList(); edge; edge = edge->next) {
        BodyData *data = (BodyData *)edge->other->GetUserData();

        if (!edge->contact->IsTouching()) continue;
        if (!data || !data->collide_player) continue;

        if (edge->contact->GetFixtureA()->GetUserData() == (void *)kBottomFixture) {
            return true;
        } else if (edge->contact->GetFixtureB()->GetUserData() == (void *)kBottomFixture) {
            return true;
        }
    }

    return false;
}

//
//
bool Character::AtGoal() const
{
    for (b2ContactEdge *edge = body_->GetContactList(); edge; edge = edge->next) {
        if (!edge->contact->IsTouching()) continue;

        if (edge->contact->GetFixtureA()->GetUserData() == (void *)kGoalFixture) {
            return true;
        } else if (edge->contact->GetFixtureB()->GetUserData() == (void *)kGoalFixture) {
            return true;
        }
    }

    return false;
}

//
//
void Character::Step(double time)
{
    animation_->Step(time);
    DoWalk(time);
    
    if (IsGrounded() && !(state_ & (kMoveLeft | kMoveRight))) {
        animation_->SetState("stand");
    }
}

//
//
void Character::DoWalk(double time)
{
    b2Vec2 vel = body_->GetLinearVelocity();
    double to_speed;

    if (state_ & kMoveLeft) {
        to_speed = -walk_speed_;
        animation_->SetState("walk");
        direction_ = kLeft;
    } else if (state_ & kMoveRight) {
        to_speed = walk_speed_;
        animation_->SetState("walk");
        direction_ = kRight;
    } else {
        to_speed = 0.0;
    }

    if (!IsGrounded()) {
        animation_->SetState("jump");
    }

    double vel_change = to_speed - vel.x;
    double force = body_->GetMass() * vel_change / time;

    if (to_speed != 0.0 && IsGrounded() && !fixed_) {
        body_->ApplyForce(b2Vec2(force, 0), body_->GetWorldCenter());
    }

}

//
//
void Character::Render() const
{
    if (hidden_) return;

    b2Vec2 pos = body_->GetPosition();
    glPushMatrix();
    glTranslated(pos.x, pos.y, 0);
    glRotated(RAD_TO_DEG(body_->GetAngle()), 0, 0, 1);
    glTranslated(image_offset_.x, image_offset_.y, 0);
    animation_->Render(image_size_.x, image_size_.y, (direction_ == kLeft), false);
    glPopMatrix();
}

//
//
void Character::OnKeyDown(SDL_KeyboardEvent *evt)
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
        default:
            break;
    }
}

//
//
void Character::OnKeyUp(SDL_KeyboardEvent *evt)
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
        default:
            break;
    }
}

//
//
void Character::Explode()
{
    Freeze();
    hidden_ = true;

    b2World *phys_world = app_->world().phys_world();

    b2BodyDef body_def;
    b2PolygonShape shape;
    b2FixtureDef fixture_def;

    body_def.type = b2_dynamicBody;
    body_def.angularDamping = 1.0;
    shape.SetAsBox(0.2, 0.02);
    fixture_def.shape = &shape;
    fixture_def.friction = 0.5;
    fixture_def.restitution = 0.4;
    fixture_def.density = 50;

    for (int i = 0; i < 5; i++) {
        b2Body *body = app_->world().phys_world()->CreateBody(&body_def);
        b2Fixture *fixture = body->CreateFixture(&fixture_def);

        Body *body_body = new Body;
        float angle = ((float)rand() / RAND_MAX) * M_PI * 2;
        b2Vec2 vec = 5 * b2Vec2(cos(angle), sin(angle));
        body_body->image_sprite.reset(app_->GetSprite("graphics/bone.png", true));
        BodyData *data = new BodyData;
        data->cause_shake = false;
        data->collide_player = false;
        data->type = kWorldBody;
        data->data.world_body = body_body;
        body->SetUserData((void *)data);
        body->SetTransform(body_->GetPosition(), 0);
        body->ApplyLinearImpulse(20 * vec, body->GetPosition());
        body->ApplyAngularImpulse(10);
        app_->world().map_file()->bodies.push_back(std::unique_ptr<Body>(body_body));
    }

    shape.SetAsBox(0.2, 0.2);
    
    b2Body *body = app_->world().phys_world()->CreateBody(&body_def);
    b2Fixture *fixture = body->CreateFixture(&fixture_def);

    Body *body_body = new Body;
    float angle = ((float)rand() / RAND_MAX) * M_PI * 2;
    b2Vec2 vec = 5 * b2Vec2(cos(angle), sin(angle));
    body_body->image_sprite.reset(app_->GetSprite("graphics/skull.png", true));
    BodyData *data = new BodyData;
    data->cause_shake = false;
    data->collide_player = false;
    data->type = kWorldBody;
    data->data.world_body = body_body;
    body->SetUserData((void *)data);
    body->SetTransform(body_->GetPosition(), 0);
    body->ApplyLinearImpulse(20 * vec, body->GetPosition());
    body->ApplyAngularImpulse(10);
    app_->world().map_file()->bodies.push_back(std::unique_ptr<Body>(body_body));

    phys_world->DestroyBody(body_);
}


}

