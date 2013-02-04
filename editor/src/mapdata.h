//
// mapdata.h
// Contains all the data structures for representing a game map.
//

#include "hasproperties.h"
#include <QtCore>
#include <QtGui>

#ifndef _MAPDATA_H_
#define _MAPDATA_H_

class ShapeItem;
struct Entity;
struct GameMap;
struct Body;
struct Fixture;
struct Shape;
struct PolygonShape;
struct CircleShape;
struct Joint;

enum BodyType { kStatic, kDynamic };
enum ShapeType { kPolygon, kCircle };
enum JointType { kDistance, kGear, kLine, kMouse, kPrismatic, kPulley, kRevolute };

Q_DECLARE_METATYPE(BodyType);
Q_DECLARE_METATYPE(ShapeType);
Q_DECLARE_METATYPE(JointType);

//
// Base structure for all map entities
struct Entity : public HasProperties {
    int id;
};

//
// Root map file
struct GameMap : public Entity {
    PROPERTY(GameMap, int, width);
    PROPERTY(GameMap, int, height);
    QVector<QSharedPointer<Shape>> shapes;
    QVector<QSharedPointer<Body>> bodies;
    QVector<QSharedPointer<Fixture>> fixtures;
    QVector<QSharedPointer<Joint>> joints;
};

//
// Physical body
struct Body : public Entity {
    PROPERTY(Body, BodyType, type);
    PROPERTY(Body, QPointF, position);
    PROPERTY(Body, qreal, rotation);
    PROPERTY(Body, QPointF, linearVelocity);
    PROPERTY(Body, qreal, angularVelocity);
    PROPERTY(Body, qreal, linearDamping);
    PROPERTY(Body, qreal, angularDamping);
    PROPERTY(Body, bool, fixedRotation);
    PROPERTY(Body, bool, bullet);
    PROPERTY(Body, bool, awake);
    PROPERTY(Body, bool, allowSleep);
    PROPERTY(Body, bool, active);
};

//
// Fixture between a body and a shape
struct Fixture : public Entity {
    QSharedPointer<Shape> shape;
    QSharedPointer<Body> body;
    PROPERTY(Fixture, qreal, friction);
    PROPERTY(Fixture, qreal, restitution);
    PROPERTY(Fixture, qreal, density);
    PROPERTY(Fixture, bool, isSensor);
};

//
// Base structure for a shape
struct Shape : public Entity {
    PROPERTY(Shape, QPointF, position);
    qreal rotation;
    ShapeItem *shapeItem;

    virtual ShapeType type() const = 0;
};

//
// Polygon shape made up of many triangles
struct PolygonShape : public Shape {
    QPolygonF polygon;
    virtual ShapeType type() const { return kPolygon; }
};

//
// Simple circle shape
struct CircleShape : public Shape {
    PROPERTY(CircleShape, qreal, radius);
    virtual ShapeType type() const { return kCircle; }
};

//
// Base structure for a joint
struct Joint : public Entity {
    QSharedPointer<Body> body1;
    QSharedPointer<Body> body2;
    PROPERTY(Joint, bool, collideConnected);
    virtual JointType type() const = 0;
};

#endif
