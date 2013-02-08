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

    map<int, shared_ptr<Shape>> shapes;
    map<int, shared_ptr<Body>> bodies;

    auto root_object = root_value.get<picojson::object>();

    shared_ptr<MapFile> map_file = make_shared<MapFile>();
    map_file->width = root_object["width"].get<double>();
    map_file->height = root_object["height"].get<double>();

    // Do two passes from the shape array -- first to load the geometric shapes, then to load the bodies
    for (const auto& shape_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = shape_value.get<picojson::object>();

        string type = shape_object["type"].get<string>();
        shared_ptr<Shape> shape;
        if (type == "polygon") {
            shared_ptr<PolygonShape> poly = make_shared<PolygonShape>();
            shape = poly;

            // Read the points
            for (const auto& point_value : shape_object["polygon"].get<picojson::array>()) {
                poly->points.push_back(PointFromArray(point_value.get<picojson::array>()));
            }
        } else if (type == "circle") {
            shared_ptr<CircleShape> circle = make_shared<CircleShape>();
            shape = circle;
            circle->radius = shape_object["radius"].get<double>();
        } else if (type == "body") {
            continue;
        } else {
            Die("Unknown shape type '%s' while loading map", type.c_str());
        }

        shape->id = shape_object["id"].get<double>();
        shape->rotation = shape_object["rotation"].get<double>();
        shape->position = PointFromArray(shape_object["position"].get<picojson::array>());

        shapes[shape->id] = shape;
        map_file->shapes.push_back(shape);
    }

    // Second shapes pass to load bodies
    for (const auto& body_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = body_value.get<picojson::object>();

        string type = shape_object["type"].get<string>();
        if (type == "body") {
            shared_ptr<Body> body = make_shared<Body>();
            body->id = shape_object["id"].get<double>();
            body->rotation = shape_object["rotation"].get<double>();
            body->position = PointFromArray(shape_object["position"].get<picojson::array>());
            body->linear_velocity = PointFromArray(shape_object["linearVelocity"].get<picojson::array>());
            body->angular_velocity = shape_object["angularVelocity"].get<double>();
            body->linear_damping = shape_object["linearDamping"].get<double>();
            body->angular_damping = shape_object["angularDamping"].get<double>();
            body->fixed_rotation = shape_object["fixedRotation"].get<bool>();
            body->awake = shape_object["awake"].get<bool>();
            body->allow_sleep = shape_object["allowSleep"].get<bool>();
            body->active = shape_object["active"].get<bool>();
            bool isDynamic = shape_object["isDynamic"].get<bool>();

            if (!isDynamic) {
                body->type = kStatic;
            } else {
                body->type = kDynamic;
            }

            bodies[body->id] = body;
            map_file->bodies.push_back(body);
        }
    }    

    // Load the fixtures
    for (const auto& fixture_value : root_object["fixtures"].get<picojson::array>()) {
        auto fixture_object = fixture_value.get<picojson::object>();

        shared_ptr<Fixture> fixture = make_shared<Fixture>();
        fixture->id = fixture_object["id"].get<double>();
        fixture->friction = fixture_object["friction"].get<double>();
        fixture->restitution = fixture_object["restitution"].get<double>();
        fixture->density = fixture_object["density"].get<double>();
        fixture->is_sensor = fixture_object["isSensor"].get<bool>();

        int shape_id = fixture_object["shape"].get<double>();
        int body_id = fixture_object["body"].get<double>();
        fixture->shape = shapes[shape_id];
        fixture->body = bodies[body_id];

        map_file->fixtures.push_back(fixture);
    }

    return map_file;
}

}