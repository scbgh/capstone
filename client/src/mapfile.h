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

enum BodyType {
    kStatic, //!< Static body (not movable)
    kDynamic //!< Dynamic body (affected by other bodies)
};
enum ShapeType { kPolygon, kCircle };
enum JointType { kDistance, kGear, kLine, kPrismatic, kPulley, kRevolute, kRope, kWeld, kWheel };

//
//! Base structure for all map entities
struct Entity {
    //! Unique ID for this entity
    int id;
    //! User-defined string information
    std::string tag;
};

//
//! The root level file structure
struct MapFile : public Entity {
    //! Width of the level in world coordinates
    int width;
    //! Height of the level in world coordinates
    int height;
    //! Path in pack file of background image
    std::string back_image;
    //! Path in pack file of foreground image
    std::string fore_image;
    //! Shapes present in the level
    std::vector<std::unique_ptr<Shape>> shapes;
    //! Physical bodies present in the level
    std::vector<std::unique_ptr<Body>> bodies;
    //! Shape-to-body fixture present in the level
    std::vector<std::unique_ptr<Fixture>> fixtures;
    //! Body-to-body joints present in the level
    std::vector<std::unique_ptr<Joint>> joints;
};

//
//! A physical body (see Box2D documentation)
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

    //! Path in pack file of associated image
    std::string image;
    //! Offset from the body's origin where the image should be drawn
    math::Point image_offset;
    //! Stored sprite data for this body
    std::unique_ptr<Sprite> image_sprite;
    //! Stored reference to the Box2D userdata
    std::unique_ptr<BodyData> data;

    ~Body();
};

//
//! Fixture between a body and a shape
//! (see Box2D documentation)
struct Fixture : public Entity {
    //! Associated shape
    Shape *shape;
    //! Associated body
    Body *body;
    double friction;
    double restitution;
    double density;
    bool is_sensor;
};

//
//! Base structure for a shape
struct Shape : public Entity {
    math::Point position;
    double rotation;

    virtual math::Box bbox() const = 0;
    virtual ShapeType type() const = 0;
};

//
//! A simple polygon shape, decomposed into convex subpolygons
struct PolygonShape : public Shape {
    math::Polygon polygon;
    //! The subpolygons (triangles) of the convex decomposition of this polygon
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
//! A simple circle shape
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
//! Base structure for a joint
struct Joint : public Entity {
    //! The first associated body
    Body *body_a;
    //! The second associated
    Body *body_b;
    bool collide_connected;
    virtual JointType type() const = 0;
};

//
//! A joint that keeps two bodies a fixed distance from each other
//! (see Box2D documentation)
struct DistanceJoint : public Joint {
    math::Point anchor_a;
    math::Point anchor_b;
    double frequency_hz;
    double damping_ratio;
    virtual JointType type() const { return kDistance; }
};

//
//! A joint that keeps two bodies at a fixed relative position but with free rotation
//! (see Box2D documentation)
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

//! Load an entire map structure from a JSON representation
//! \param json The JSON source code of the map definition file
MapFile *LoadMapFromJSON(const std::string& json);

}

#endif
