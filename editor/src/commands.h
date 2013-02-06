//
//  actions.h
//  Undo/redo actions for the map scene
//

#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "sceneitems/shapeitem.h"
#include "sceneitems/connectitem.h"
#include <QUndoCommand>

class HasProperties;
class MapScene;
struct Shape;
struct PolygonShape;

enum CommandType { kMoveShape, kChangePolygonGeometry };

//
//
class CreateShapeCommand : public QUndoCommand
{
public:
    CreateShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    MapScene *scene_;
    QSharedPointer<Shape> shape_;
};

//
//
class MoveShapeCommand : public QUndoCommand
{
public:
    MoveShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QPointF oldPos, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();
    virtual int id() const { return kMoveShape; }

private:
    MapScene *scene_;
    QSharedPointer<Shape> shape_;
    QPointF newPos_;
    QPointF oldPos_;
};

//
//
class DeleteShapeCommand : public QUndoCommand
{
public:
    DeleteShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    MapScene *scene_;
    QSharedPointer<Shape> shape_;
};

//
//
class DeleteFixtureCommand : public QUndoCommand
{
public:
    DeleteFixtureCommand(MapScene *scene, QSharedPointer<Fixture> fixture, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    MapScene *scene_;
    QSharedPointer<Fixture> fixture_;
    SceneItem *shape1_;
    SceneItem *shape2_;
};

//
//
class ChangePolygonGeometryCommand : public QUndoCommand
{
public:
    ChangePolygonGeometryCommand(MapScene *scene, QSharedPointer<PolygonShape> shape, QPolygonF polygon,
        QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();
    virtual int id() const { return kChangePolygonGeometry; }

private:
    MapScene *scene_;
    QSharedPointer<PolygonShape> shape_;
    QPolygonF polygon_;
    QPolygonF oldPolygon_;
};

//
//
class ChangePropertyCommand : public QUndoCommand
{
public:
    ChangePropertyCommand(QSharedPointer<HasProperties> target, const QString& name, QVariant value, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    QSharedPointer<HasProperties> target_;
    QString name_;
    QVariant value_;
    QVariant oldValue_;
private:
};

//
//
class CreateFixtureCommand : public QUndoCommand
{
public:
    CreateFixtureCommand(MapScene *scene, QSharedPointer<Fixture> fixture, QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();

private:
    MapScene *scene_;
    QSharedPointer<Fixture> fixture_;
};

#endif
