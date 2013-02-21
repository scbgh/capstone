//
// mapfile.cpp
//

#include <string>
#include <map>
#include "mapfile.h"
#include "json/picojson.h"

namespace j = picojson;

//
//
int freshId()
{
    static int id = 0;
    return ++id;
}

//
// Convert a two-element JSON-value array (STL vector) into a Point
QPointF pointFromArray(const std::vector<picojson::value>& elems)
{
    return { elems[0].get<double>(), elems[1].get<double>() };
}

//
//
j::value pointToValue(QPointF point)
{
    return j::value(j::array { j::value(point.x()), j::value(point.y()) });
}

//
//
j::value polygonToValue(QPolygonF polygon)
{
    j::array arr;
    for (auto pt : polygon) {
        arr.push_back(pointToValue(pt));
    }
    return j::value(arr);
}

//
//
j::value shapeToValue(QSharedPointer<Shape> shape)
{
    if (shape->id == 0) {
        shape->id = freshId();
    }

    j::object subObject;    
    j::object shapeObject = {
        { "id", j::value((double)shape->id) },
        { "position", pointToValue(shape->position) },
        { "rotation", j::value(shape->rotation) },
    };

    switch (shape->type()) {
        case kPolygon: {
            QSharedPointer<PolygonShape> poly = qSharedPointerCast<PolygonShape>(shape);
            subObject = {
                { "polygon", polygonToValue(poly->polygon) },
                { "type", j::value("polygon") }
            };
            break;
        }
        case kCircle: {
            QSharedPointer<CircleShape> circle = qSharedPointerCast<CircleShape>(shape);
            subObject = {
                { "radius", j::value(circle->radius) },
                { "type", j::value("circle") }
            };
            break;
        }
        case kBody: {
            QSharedPointer<Body> body = qSharedPointerCast<Body>(shape);
            subObject = {
                { "isDynamic", j::value(body->bodyType == Body::kDynamic) },
                { "linearVelocity", pointToValue(body->linearVelocity) },
                { "angularVelocity", j::value(body->angularVelocity) },
                { "linearDamping", j::value(body->linearDamping) },
                { "angularDamping", j::value(body->angularDamping) },
                { "fixedRotation", j::value(body->fixedRotation) },
                { "bullet", j::value(body->bullet) },
                { "awake", j::value(body->awake) },
                { "allowSleep", j::value(body->allowSleep) },
                { "active", j::value(body->active) },
                { "type", j::value("body") }
            };
            break;
        }
    }
    shapeObject.insert(begin(subObject), end(subObject));

    return j::value(shapeObject);
}

//
//
j::value fixtureToValue(QSharedPointer<Fixture> fixture)
{
    j::object shapeObject = {
        { "id", j::value((double)fixture->id) },
        { "shape", j::value((double)fixture->shape->id) },
        { "body", j::value((double)fixture->body->id) },
        { "friction", j::value(fixture->friction) },
        { "restitution", j::value(fixture->restitution) },
        { "density", j::value(fixture->density) },
        { "isSensor", j::value(fixture->isSensor) }
    };

    return j::value(shapeObject);
}

//
//
j::value jointToValue(QSharedPointer<Joint> joint)
{
    j::object subObject;
    j::object jointObject = {
        { "id", j::value((double)joint->id) },
        { "bodyA", j::value((double)joint->bodyA->id) },
        { "bodyB", j::value((double)joint->bodyB->id) }
    };

    switch (joint->type()) {
        case kDistance: {
            QSharedPointer<DistanceJoint> realJoint = qSharedPointerCast<DistanceJoint>(joint);
            subObject = {
                { "type", j::value("distance") },
                { "localAnchorA", pointToValue(realJoint->anchorA - realJoint->bodyA->position - QPointF(0.25, -0.25)) },
                { "localAnchorB", pointToValue(realJoint->anchorB - realJoint->bodyB->position - QPointF(0.25, -0.25)) },
                { "length", j::value(QVector2D(realJoint->anchorA - realJoint->anchorB).length()) },
                { "frequencyHz", j::value(realJoint->frequencyHz) }, 
                { "dampingRatio", j::value(realJoint->dampingRatio) }
            };
            break;
        }
        default:
            qFatal("Unknown joint type");
            break;
    }
    jointObject.insert(begin(subObject), end(subObject));

    return j::value(jointObject);
}

//
//
QString mapToJson(QSharedPointer<GameMap> map)
{
    j::array shapes;
    for (auto shape : map->shapes) {
        shapes.push_back(shapeToValue(shape));
    }

    j::array fixtures;
    for (auto fixture : map->fixtures) {
        fixtures.push_back(fixtureToValue(fixture));
    }

    j::array joints;
    for (auto joint : map->joints) {
        joints.push_back(jointToValue(joint));
    }

    j::object rootObject = {
        { "width", j::value(map->width) },
        { "height", j::value(map->height) },
        { "shapes", j::value(shapes) },
        { "fixtures", j::value(fixtures) },
        { "joints", j::value(joints) }
    };

    return QString::fromStdString(j::value(rootObject).serialize());
}

//
//
QSharedPointer<GameMap> jsonToMap(const QString& json)
{
        // Load the root object of the JSON map representation
    picojson::value root_value;
    std::string error;
    std::string stdJson = json.toStdString();
    picojson::parse(root_value, stdJson.begin(), stdJson.end(), &error);
    if (!error.empty()) {
        qFatal("Failed to parse JSON: %s", error.c_str());
    }

    QMap<int, QSharedPointer<Shape>> shapes;
    QMap<int, QSharedPointer<Body>> bodies;

    auto root_object = root_value.get<picojson::object>();

    QSharedPointer<GameMap> game_map = QSharedPointer<GameMap>(new GameMap);
    game_map->width = root_object["width"].get<double>();
    game_map->height = root_object["height"].get<double>();

    // Do two passes from the shape array -- first to load the geometric shapes, then to load the bodies
    for (const auto& shape_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = shape_value.get<picojson::object>();

        QSharedPointer<Shape> shape;
        std::string type = shape_object["type"].get<std::string>();
        if (type == "polygon") {
            QSharedPointer<PolygonShape> poly = QSharedPointer<PolygonShape>(new PolygonShape);
            shape = qSharedPointerCast<Shape>(poly);

            // Read the points
            for (const auto& point_value : shape_object["polygon"].get<picojson::array>()) {
                poly->polygon << pointFromArray(point_value.get<picojson::array>());
            }
        } else if (type == "circle") {
            QSharedPointer<CircleShape> circle = QSharedPointer<CircleShape>(new CircleShape);
            shape = qSharedPointerCast<Shape>(circle);
            circle->radius = shape_object["radius"].get<double>();
        } else if (type == "body") {
            continue;
        } else {
            qFatal("Unknown shape type '%s' while loading map", type.c_str());
        }

        shape->id = shape_object["id"].get<double>();
        shape->rotation = shape_object["rotation"].get<double>();
        shape->position = pointFromArray(shape_object["position"].get<picojson::array>());
        shapes[shape->id] = shape;
        game_map->shapes.append(shape);
    }

    // Second shapes pass to load bodies
    for (const auto& body_value : root_object["shapes"].get<picojson::array>()) {
        auto shape_object = body_value.get<picojson::object>();

        std::string type = shape_object["type"].get<std::string>();
        if (type == "body") {
            QSharedPointer<Body> body = QSharedPointer<Body>(new Body);
            body->id = shape_object["id"].get<double>();
            body->rotation = shape_object["rotation"].get<double>();
            body->position = pointFromArray(shape_object["position"].get<picojson::array>());
            body->linearVelocity = pointFromArray(shape_object["linearVelocity"].get<picojson::array>());
            body->angularVelocity = shape_object["angularVelocity"].get<double>();
            body->linearDamping = shape_object["linearDamping"].get<double>();
            body->angularDamping = shape_object["angularDamping"].get<double>();
            body->fixedRotation = shape_object["fixedRotation"].get<bool>();
            body->awake = shape_object["awake"].get<bool>();
            body->allowSleep = shape_object["allowSleep"].get<bool>();
            body->active = shape_object["active"].get<bool>();
            bool isDynamic = shape_object["isDynamic"].get<bool>();

            if (!isDynamic) {
                body->bodyType = Body::kStatic;
            } else {
                body->bodyType = Body::kDynamic;
            }

            bodies[body->id] = body;
            game_map->shapes.append(body);
        }
    }    

    // Load the fixtures
    for (const auto& fixture_value : root_object["fixtures"].get<picojson::array>()) {
        auto fixture_object = fixture_value.get<picojson::object>();

        QSharedPointer<Fixture> fixture = QSharedPointer<Fixture>(new Fixture);
        fixture->id = fixture_object["id"].get<double>();
        fixture->friction = fixture_object["friction"].get<double>();
        fixture->restitution = fixture_object["restitution"].get<double>();
        fixture->density = fixture_object["density"].get<double>();
        fixture->isSensor = fixture_object["isSensor"].get<bool>();

        int shape_id = fixture_object["shape"].get<double>();
        int body_id = fixture_object["body"].get<double>();
        fixture->shape = shapes[shape_id];
        fixture->body = bodies[body_id];

        game_map->fixtures.append(fixture);
    }

    // Load joints
    for (const auto& joint_value : root_object["joints"].get<picojson::array>()) {
        auto joint_object = joint_value.get<picojson::object>();

        QSharedPointer<Joint> joint;
        int body_a_id = joint_object["bodyA"].get<double>();
        int body_b_id = joint_object["bodyB"].get<double>();
        QSharedPointer<Body> bodyA = bodies[body_a_id];
        QSharedPointer<Body> bodyB = bodies[body_b_id];
        std::string type = joint_object["type"].get<std::string>();
        if (type == "distance") {
            DistanceJoint *distance_joint = new DistanceJoint;
            joint = QSharedPointer<Joint>(distance_joint);
            distance_joint->anchorA = pointFromArray(joint_object["localAnchorA"].get<picojson::array>())
                + QPointF(0.25, -0.25);
            distance_joint->anchorB = pointFromArray(joint_object["localAnchorB"].get<picojson::array>())
                + QPointF(0.25, -0.25);
            distance_joint->frequencyHz = joint_object["frequencyHz"].get<double>();
            distance_joint->dampingRatio = joint_object["dampingRatio"].get<double>();
        }

        joint->bodyA = bodyA;
        joint->bodyB = bodyB;
        joint->id = joint_object["id"].get<double>();

        game_map->joints.append(joint);
    }

    return game_map;
}