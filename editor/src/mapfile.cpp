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
QString mapToJson(QSharedPointer<GameMap> map)
{
    j::array shapes;
    for (auto shape : map->shapes) {
        shapes.push_back(shapeToValue(shape));
    }

    j::array fixtures;
    for (auto fixture : map->fixtures) {
        shapes.push_back(fixtureToValue(fixture));
    }

    j::object rootObject = {
        { "width", j::value(map->width) },
        { "height", j::value(map->height) },
        { "shapes", j::value(shapes) },
        { "fixtures", j::value(fixtures) }
    };

    return QString::fromStdString(j::value(rootObject).serialize());
}

//
//
QSharedPointer<GameMap> jsonToMap(const QString& json)
{
    return QSharedPointer<GameMap>();
}