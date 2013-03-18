//
// mapfile.cpp
//

#include "common.h"
#include "mapfile.h"
#include "sprite.h"
#include "json/picojson.h"
#include "math/math.h"
#include "world.h"

namespace pg {

namespace {

//
// Convert a two-element JSON-value array (STL vector) into a Point
math::Point PointFromArray(const vector<picojson::value>& elems)
{
    return { elems[0].get<double>(), elems[1].get<double>() };
}

}

//
//
Body::~Body()
{ }

///////////

//
//
MapFile *LoadMapFromJSON(const string& json)
{
    // Load the root object of the JSON map representation
    picojson::value root_value;
    string error;
    picojson::parse(root_value, json.begin(), json.end(), &error);
    if (!error.empty()) {
        Die("Failed to parse JSON: %s", error.c_str());
    }

    map<int, Shape *> shapes;
    map<int, Body *> bodies;

    auto root_object = root_value.get<picojson::object>();

    MapFile *map_file = new MapFile;
    map_file->width = root_object["width"].get<double>();
    map_file->height = root_object["height"].get<double>();
    map_file->back_image = root_object["backImage"].get<std::string>();
    map_file->fore_image = root_object["foreImage"].get<std::string>();

    // Do two passes from the shape array -- first to load the geometric shapes, then to load the bodies
    for (const auto& shape_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = shape_value.get<picojson::object>();


        Shape *shape;
        string type = shape_object["type"].get<string>();
        if (type == "polygon") {
            PolygonShape *poly = new PolygonShape;
            shape = poly;

            // Read the points
            for (const auto& point_value : shape_object["polygon"].get<picojson::array>()) {
                poly->polygon.points.push_back(PointFromArray(point_value.get<picojson::array>()));
            }

            // Triangulate and partition the polygon
            poly->polygon.SetWindingDirection(math::kCCW);
            poly->subpolygons = math::Triangulate(poly->polygon);
        } else if (type == "circle") {
            CircleShape *circle = new CircleShape;
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
        shape->tag = shape_object["tag"].get<string>();
        shapes[shape->id] = shape;
        map_file->shapes.push_back(unique_ptr<Shape>(shape));
    }

    // Second shapes pass to load bodies
    for (const auto& body_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = body_value.get<picojson::object>();

        string type = shape_object["type"].get<string>();
        if (type == "body") {
            Body *body = new Body;
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
            body->image = shape_object["image"].get<string>();
            body->tag = shape_object["tag"].get<string>();
            body->image_offset = PointFromArray(shape_object["imageOffset"].get<picojson::array>());
            bool isDynamic = shape_object["isDynamic"].get<bool>();

            if (!isDynamic) {
                body->type = kStatic;
            } else {
                body->type = kDynamic;
            }

            bodies[body->id] = body;
            map_file->bodies.push_back(unique_ptr<Body>(body));
        }
    }    

    // Load the fixtures
    for (const auto& fixture_value : root_object["fixtures"].get<picojson::array>()) {
        auto fixture_object = fixture_value.get<picojson::object>();

        Fixture *fixture = new Fixture;
        fixture->id = fixture_object["id"].get<double>();
        fixture->friction = fixture_object["friction"].get<double>();
        fixture->restitution = fixture_object["restitution"].get<double>();
        fixture->density = fixture_object["density"].get<double>();
        fixture->is_sensor = fixture_object["isSensor"].get<bool>();
        fixture->tag = fixture_object["tag"].get<string>();

        int shape_id = fixture_object["shape"].get<double>();
        int body_id = fixture_object["body"].get<double>();
        fixture->shape = shapes[shape_id];
        fixture->body = bodies[body_id];

        map_file->fixtures.push_back(unique_ptr<Fixture>(fixture));
    }

    // Load joints
    for (const auto& joint_value : root_object["joints"].get<picojson::array>()) {
        auto joint_object = joint_value.get<picojson::object>();

        Joint *joint;
        string type = joint_object["type"].get<string>();
        if (type == "distance") {
            DistanceJoint *distance_joint = new DistanceJoint;
            joint = distance_joint;
            distance_joint->anchor_a = PointFromArray(joint_object["anchorA"].get<picojson::array>());
            distance_joint->anchor_b = PointFromArray(joint_object["anchorB"].get<picojson::array>());
            distance_joint->frequency_hz = joint_object["frequencyHz"].get<double>();
            distance_joint->damping_ratio = joint_object["dampingRatio"].get<double>();
        } else if (type == "revolute") {
            RevoluteJoint *revolute_joint = new RevoluteJoint;
            joint = revolute_joint;
            revolute_joint->anchor = PointFromArray(joint_object["anchor"].get<picojson::array>());
            revolute_joint->enable_motor = joint_object["enableMotor"].get<bool>();
            revolute_joint->enable_limit = joint_object["enableLimit"].get<bool>();
            revolute_joint->lower_angle = joint_object["lowerAngle"].get<double>();
            revolute_joint->reference_angle = joint_object["referenceAngle"].get<double>();
            revolute_joint->upper_angle = joint_object["upperAngle"].get<double>();
            revolute_joint->max_motor_torque = joint_object["maxMotorTorque"].get<double>();
            revolute_joint->motor_speed = joint_object["motorSpeed"].get<double>();
        }
        int body_a_id = joint_object["bodyA"].get<double>();
        int body_b_id = joint_object["bodyB"].get<double>();
        joint->body_a = bodies[body_a_id];
        joint->body_b = bodies[body_b_id];
        joint->collide_connected = joint_object["collideConnected"].get<bool>();
        joint->tag = joint_object["tag"].get<string>();
        joint->id = joint_object["id"].get<double>();

        map_file->joints.push_back(unique_ptr<Joint>(joint));
    }

    return map_file;
}

}
