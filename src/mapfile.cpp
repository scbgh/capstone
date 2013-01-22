//
// mapfile.cpp
//

#include "common.h"
#include "mapfile.h"
#include "json/picojson.h"

namespace pg {

namespace {

//
// Convert a two-element JSON-value array (STL vector) into a Point
Point PointFromArray(const vector<picojson::value>& elems)
{
    return { elems[0].get<double>(), elems[1].get<double>() };
}

}

///////////

//
// Load an entire map structure from a JSON representation
shared_ptr<MapFile> LoadMapFromJSON(const string& json)
{
    // Load the root object of the JSON map representation
    picojson::value root_value;
    string error;
    picojson::parse(root_value, json.begin(), json.end(), &error);
    if (!error.empty()) {
        Die("Failed to parse JSON: %s", error.c_str());
    }

    map<string, shared_ptr<Shape>> shapes;
    map<string, shared_ptr<Body>> bodies;

    auto root_object = root_value.get<picojson::object>();

    shared_ptr<MapFile> map_file = make_shared<MapFile>();
    map_file->pixels_per_unit = (int)root_object["pixels_per_unit"].get<double>();
    map_file->width = root_object["width"].get<double>();
    map_file->height = root_object["height"].get<double>();

    // Load the shapes into a map by UUID
    for (const auto& shape_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = shape_value.get<picojson::object>();

        string type = shape_object["type"].get<string>();
        shared_ptr<Shape> shape;
        if (type == "polygon") {
            shared_ptr<PolygonShape> poly = make_shared<PolygonShape>();
            shape = poly;

            // Read the triangles
            for (const auto& triangle_value : shape_object["triangles"].get<picojson::array>()) {
                Triangle t;
                auto triangle_array = triangle_value.get<picojson::array>();                
                for (int i = 0; i < 3; i++) {
                    t.p[i] = PointFromArray(triangle_array[i].get<picojson::array>());
                }
                poly->triangles.push_back(t);
            }

            // Read the points
            for (const auto& point_value : shape_object["points"].get<picojson::array>()) {
                poly->points.push_back(PointFromArray(point_value.get<picojson::array>()));
            }
        } else if (type == "circle") {
            shared_ptr<CircleShape> circle = make_shared<CircleShape>();
            shape = circle;
            circle->radius = shape_object["radius"].get<double>();
        } else {
            Die("Unknown polygon type '%s' while loading map", type.c_str());
        }

        shape->uuid = shape_object["uuid"].get<string>();
        shape->rotation = shape_object["rotation"].get<double>();
        shape->position = PointFromArray(shape_object["position"].get<picojson::array>());

        shapes[shape->uuid] = shape;
        map_file->shapes.push_back(shape);
    }

    // Load the bodies into a map by UUID
    for (const auto& body_value : root_object["bodies"].get<picojson::array>()) {
        auto body_object = body_value.get<picojson::object>();

        shared_ptr<Body> body = make_shared<Body>();
        body->uuid = body_object["uuid"].get<string>();
        body->rotation = body_object["rotation"].get<double>();
        body->position = PointFromArray(body_object["position"].get<picojson::array>());
        body->linear_velocity = PointFromArray(body_object["linear_velocity"].get<picojson::array>());
        body->angular_velocity = body_object["angular_velocity"].get<double>();
        body->linear_damping = body_object["linear_damping"].get<double>();
        body->angular_damping = body_object["angular_damping"].get<double>();
        body->fixed_rotation = body_object["fixed_rotation"].get<bool>();
        body->awake = body_object["awake"].get<bool>();
        body->allow_sleep = body_object["allow_sleep"].get<bool>();
        body->active = body_object["active"].get<bool>();
        string type = body_object["type"].get<string>();

        if (type == "static") {
            body->type = kStatic;
        } else if (type == "dynamic") {
            body->type = kDynamic;
        } else {
            Die("Unknown body type '%s' while loading map", type.c_str());
        }

        bodies[body->uuid] = body;
        map_file->bodies.push_back(body);
    }    

    // Load the fixtures
    for (const auto& fixture_value : root_object["fixtures"].get<picojson::array>()) {
        auto fixture_object = fixture_value.get<picojson::object>();

        shared_ptr<Fixture> fixture = make_shared<Fixture>();
        fixture->uuid = fixture_object["uuid"].get<string>();
        fixture->friction = fixture_object["friction"].get<double>();
        fixture->restitution = fixture_object["restitution"].get<double>();
        fixture->density = fixture_object["density"].get<double>();
        fixture->is_sensor = fixture_object["is_sensor"].get<bool>();

        string shape_uuid = fixture_object["shape"].get<string>();
        string body_uuid = fixture_object["body"].get<string>();
        fixture->shape = shapes[shape_uuid];
        fixture->body = bodies[body_uuid];

        map_file->fixtures.push_back(fixture);
    }

    return map_file;
}

}