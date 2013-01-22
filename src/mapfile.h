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

namespace pg {

struct Entity;
struct MapFile;
struct Body;
struct Fixture;
struct Shape;
struct PolygonShape;
struct CircleShape;

//
//
struct Point {
    double x;
    double y;
};

//
//
struct Triangle {
    Point p[3];
};

enum BodyType { kStatic, kDynamic };

//
// Base structure for all map entities
struct Entity {
    std::string uuid;
};

//
// Root map file
struct MapFile : public Entity {
    int pixels_per_unit;
    int width;
    int height;
    std::vector<std::shared_ptr<Shape>> shapes;
    std::vector<std::shared_ptr<Body>> bodies;
    std::vector<std::shared_ptr<Fixture>> fixtures;
};

//
// Physical body
struct Body : public Entity {
    BodyType type;
    Point position;
    double rotation;
    Point linear_velocity;
    double angular_velocity;
    double linear_damping;
    double angular_damping;
    bool fixed_rotation;
    bool bullet;
    bool awake;
    bool allow_sleep;
    bool active;
};

//
// Fixture between a body and a shape
struct Fixture : public Entity {
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Body> body;
    double friction;
    double restitution;
    double density;
    bool is_sensor;
};

//
// Base structure for a shape
struct Shape : public Entity {
    Point position;
    double rotation;
};

//
// Polygon shape made up of many triangles
struct PolygonShape : public Shape {
    std::vector<Triangle> triangles;
    std::vector<Point> points;
    Point position;
};

//
// Simple circle shape
struct CircleShape : public Shape {
    double radius;
};

std::shared_ptr<MapFile> LoadMapFromJSON(const std::string& json);

}

#endif