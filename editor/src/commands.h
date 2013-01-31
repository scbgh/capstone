//
//  actions.h
//  Undo/redo actions for the map scene
//

#ifndef _ACTIONS_H_
#define _ACTIONS_H_

#include "sceneitems/shapeitem.h"
#include <QUndoCommand>

class MapScene;
struct Shape;
struct PolygonShape;

enum CommandType { kMoveShape, kChangePolygonGeometry };

//
//
class CreatePolygonCommand : public QUndoCommand
{
public:
    CreatePolygonCommand(MapScene *scene, QSharedPointer<PolygonShape> shape);
    virtual void undo();
    virtual void redo();

private:
    MapScene *scene_;
    QSharedPointer<PolygonShape> shape_;
};

//
//
class MoveShapeCommand : public QUndoCommand
{
public:
    MoveShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QPointF pos);
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *other);
    virtual int id() { return kMoveShape; }

private:
    MapScene *scene_;
    QSharedPointer<Shape> shape_;
    QPointF pos_;
};

//
//
class ChangePolygonGeometryCommand : public QUndoCommand
{
public:
    ChangePolygonGeometryCommand(MapScene *scene, QSharedPointer<PolygonShape> shape, QPolygonF polygon);
    virtual void undo();
    virtual void redo();
    virtual bool mergeWith(const QUndoCommand *other);
    virtual int id() { return kChangePolygonGeometry; }

private:
    MapScene *scene_;
    QSharedPointer<PolygonShape> shape_;
    QPolygonF polygon_;
};

#endif
