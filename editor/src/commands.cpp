//
//  commands.cpp
//

#include "commands.h"
#include "mapdata.h"
#include "mapscene.h"

//
//
CreatePolygonCommand::CreatePolygonCommand(MapScene *scene, QSharedPointer<PolygonShape> shape, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape)
{
    setText("Create Polygon");
}

//
//
void CreatePolygonCommand::undo()
{
    scene_->map()->shapes.pop_back();
    scene_->removeItem(shape_->shapeItem);
    shape_->shapeItem->sync();
}

//
//
void CreatePolygonCommand::redo()
{
    scene_->map()->shapes.append(shape_);
    if (!scene_->items().contains(shape_->shapeItem)) {
        scene_->addItem(shape_->shapeItem);
    }
    shape_->shapeItem->sync();
}

//
//
MoveShapeCommand::MoveShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QPointF pos, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape),
    pos_(pos),
    oldPos_(shape->position)
{
    setText("Move Shape");
}

//
//
void MoveShapeCommand::undo()
{
    shape_->position = oldPos_;
    shape_->shapeItem->sync();
}

//
//
void MoveShapeCommand::redo()
{
    shape_->position = pos_;
    shape_->shapeItem->sync();
}

//
//
bool MoveShapeCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) {
        return false;
    }
    const MoveShapeCommand *otherCmd = static_cast<const MoveShapeCommand *>(other);
    if (otherCmd->shape_ != shape_) {
        return false;
    }
    pos_ = otherCmd->pos_;
    return true;
}

//
//
ChangePolygonGeometryCommand::ChangePolygonGeometryCommand(MapScene *scene, QSharedPointer<PolygonShape> shape,
        QPolygonF polygon, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape),
    polygon_(polygon),
    oldPolygon_(shape->polygon)
{
    setText("Change Polygon Geometry");
}

//
//
void ChangePolygonGeometryCommand::undo()
{
    shape_->polygon = oldPolygon_;
    shape_->shapeItem->sync();
}

//
//
void ChangePolygonGeometryCommand::redo()
{
    shape_->polygon = polygon_;
    shape_->shapeItem->sync();
}

//
//
bool ChangePolygonGeometryCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) {
        return false;
    }
    const ChangePolygonGeometryCommand *otherCmd = static_cast<const ChangePolygonGeometryCommand *>(other);
    if (otherCmd->shape_ != shape_) {
        return false;
    }
    polygon_ = static_cast<const ChangePolygonGeometryCommand *>(other)->polygon_;
    return true;
}