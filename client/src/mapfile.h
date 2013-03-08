//
// mapfile.h
// Objects representing a game map as it is represented on disk, along with code to read them from disk and
// deal with the objects.
//

#ifndef _MAPFILE_H_
#define _MAPFILE_H_

#include <memory>
#include <string>
#include <vector>
#include <list>
#include "sprite.h"
#include "math/math.h"

namespace pg {

struct Entity;
struct MapFile;
struct Body;
struct Fixture;
struct Shape;
struct Joint;
struct PolygonShape;
struct CircleShape;
struct BodyData;

enum BodyType { kStatic, kDynamic };
enum ShapeType { kPolygon, kCircle };
enum JointType { kDistance, kGear, kLine, kPrismatic, kPulley, kRevolute, kRope, kWeld, kWheel };

//
// Base structure for all map entities
struct Entity {
    int id;
    std::string tag;
};

//
// Root map file
struct MapFile : public Entity {
    int width;
    int height;
    std::string back_image;
    std::string fore_image;
    std::vector<std::unique_ptr<Shape>> shapes;
    std::vector<std::unique_ptr<Body>> bodies;
    std::vector<std::unique_ptr<Fixture>> fixtures;
    std::vector<std::unique_ptr<Joint>> joints;
};

//
// Physical body
struct Body : public Entity {
    BodyType type;
    math::Point position;
    double rotation;
    math::Point linear_velocity;
    double angular_velocity;
    double linear_damping;
    double angular_damping;
    bool fixed_rotation;
    bool bullet;
    bool awake;
    bool allow_sleep;
    bool active;
    std::string image;
    math::Point image_offset;
    std::unique_ptr<Sprite> image_sprite;
    std::unique_ptr<BodyData> data;

    ~Body();
};

//
// Fixture between a body and a shape
struct Fixture : public Entity {
    Shape *shape;
    Body *body;
    double friction;
    double restitution;
    double density;
    bool is_sensor;
};

//
// Base structure for a shape
struct Shape : public Entity {
    math::Point position;
    double rotation;

    virtual math::Box bbox() const = 0;
    virtual ShapeType type() const = 0;
};

//
// Polygon shape made up of many triangles
struct PolygonShape : public Shape {
    math::Polygon polygon;
    std::list<math::Polygon> subpolygons;

    virtual math::Box bbox() const
    {
        math::Box aabb;
        for (math::Point p : polygon.points) {
            aabb = math::AddPoint(aabb, p + math::Vector(position.x, position.y)); 
        }
        return aabb;
    }
    virtual ShapeType type() const { return kPolygon; }
};

//
// Simple circle shape
struct CircleShape : public Shape {
    double radius;
    virtual ShapeType type() const { return kCircle; }
    virtual math::Box bbox() const
    {
        math::Box aabb;
        aabb = math::AddPoint(aabb, { position.x - radius, position.y - radius });
        aabb = math::AddPoint(aabb, { position.x + radius, position.y + radius });
        return aabb;
    }
};

//
// Base structure for a joint
struct Joint : public Entity {
    Body *body_a;
    Body *body_b;
    bool collide_connected;
    virtual JointType type() const = 0;
};

//
//
struct DistanceJoint : public Joint {
    math::Point anchor_a;
    math::Point anchor_b;
    double frequency_hz;
    double damping_ratio;
    virtual JointType type() const { return kDistance; }
};

//
//
struct RevoluteJoint : public Joint {
    math::Point anchor;
    bool enable_motor;
    bool enable_limit;
    bool max_motor_torque;
    double motor_speed;
    double lower_angle;
    double upper_angle;
    double reference_angle;
    virtual JointType type() const { return kRevolute; }
};

MapFile *LoadMapFromJSON(const std::string& json);

}

#endif
