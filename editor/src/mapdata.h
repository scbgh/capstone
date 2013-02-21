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
class ConnectItem;
struct Entity;
struct GameMap;
struct Body;
struct Fixture;
struct Shape;
struct PolygonShape;
struct CircleShape;
struct Joint;

enum ShapeType { kPolygon, kCircle, kBody };
enum JointType { kDistance, kGear, kLine, kPrismatic, kPulley, kRevolute, kRope, kWeld, kWheel };

Q_DECLARE_METATYPE(ShapeType);
Q_DECLARE_METATYPE(JointType);

//
// Base structure for all map entities
struct Entity : public HasProperties {
    Entity() :
        id(0)
    { }

    int id;
};

//
// Root map file
struct GameMap : public Entity {
    GameMap() :
        width(1280./32.),
        height(720./32.)
    { }

    PROPERTY(GameMap, qreal, width);
    PROPERTY(GameMap, qreal, height);
    QVector<QSharedPointer<Shape>> shapes;
    QVector<QSharedPointer<Fixture>> fixtures;
    QVector<QSharedPointer<Joint>> joints;
};

//
// Fixture between a body and a shape
struct Fixture : public Entity {
    Fixture() :
        friction(0),
        restitution(0),
        density(0),
        isSensor(false)
    { }

    QSharedPointer<Shape> shape;
    QSharedPointer<Body> body;
    PROPERTY(Fixture, qreal, friction);
    PROPERTY(Fixture, qreal, restitution);
    PROPERTY(Fixture, qreal, density);
    PROPERTY(Fixture, bool, isSensor);

    ConnectItem *connectItem;
};

//
// Base structure for a shape
struct Shape : public Entity {
    Shape() :
        rotation(0)
    { }

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
// Physical body (subclass of Shape for simplicity)
struct Body : public Shape {
private:
    Q_OBJECT
    Q_ENUMS(BodyType)

public:
    enum BodyType { kStatic, kDynamic };

    Body() :
        bodyType(kDynamic),
        angularVelocity(0),
        linearDamping(0),
        angularDamping(0),
        fixedRotation(false),
        bullet(false),
        awake(true),
        allowSleep(true),
        active(true)
    { }

    PROPERTY(Body, BodyType, bodyType);
    PROPERTY(Body, QPointF, linearVelocity);
    PROPERTY(Body, qreal, angularVelocity);
    PROPERTY(Body, qreal, linearDamping);
    PROPERTY(Body, qreal, angularDamping);
    PROPERTY(Body, bool, fixedRotation);
    PROPERTY(Body, bool, bullet);
    PROPERTY(Body, bool, awake);
    PROPERTY(Body, bool, allowSleep);
    PROPERTY(Body, bool, active);

    virtual ShapeType type() const { return kBody; }
};

Q_DECLARE_METATYPE(Body::BodyType);

//
// Base structure for a joint
struct Joint : public Entity {
    Joint() :
        collideConnected(false)
    { }

    QSharedPointer<Body> bodyA;
    QSharedPointer<Body> bodyB;
    PROPERTY(Joint, bool, collideConnected);

    ConnectItem *connectItem;
    virtual JointType type() const = 0;
};

//
//
struct DistanceJoint : public Joint {
    DistanceJoint() :
        frequencyHz(0.0),
        dampingRatio(0.0)
    { }

    PROPERTY(DistanceJoint, QPointF, anchorA);
    PROPERTY(DistanceJoint, QPointF, anchorB);
    PROPERTY(DistanceJoint, qreal, frequencyHz);
    PROPERTY(DistanceJoint, qreal, dampingRatio);

    virtual JointType type() const { return kDistance; }
};

#endif
