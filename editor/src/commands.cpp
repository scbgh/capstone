//
//  commands.cpp
//

#include "commands.h"
#include "mapdata.h"
#include "mapscene.h"

//
//
CreatePolygonCommand::CreatePolygonCommand(MapScene *scene, QSharedPointer<PolygonShape> shape) :
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
MoveShapeCommand::MoveShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QPointF pos) :
    scene_(scene),
    shape_(shape),
    pos_(pos)
{
    setText("Move Shape");
}

//
//
void MoveShapeCommand::undo()
{
    qSwap(pos_, shape_->position);
    shape_->shapeItem->sync();
}

//
//
void MoveShapeCommand::redo()
{
    qSwap(pos_, shape_->position);
    shape_->shapeItem->sync();
}

//
//
bool MoveShapeCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) {
        return false;
    }
    pos_ = static_cast<const MoveShapeCommand *>(other)->pos_;
    return true;
}

//
//
ChangePolygonGeometryCommand::ChangePolygonGeometryCommand(MapScene *scene, QSharedPointer<PolygonShape> shape, QPolygonF polygon) :
    scene_(scene),
    shape_(shape),
    polygon_(polygon)
{
    setText("Change Polygon Geometry");
}

//
//
void ChangePolygonGeometryCommand::undo()
{
    qSwap(polygon_, shape_->polygon);
    shape_->shapeItem->sync();
}

//
//
void ChangePolygonGeometryCommand::redo()
{
    qSwap(polygon_, shape_->polygon);
    shape_->shapeItem->sync();
}

//
//
bool ChangePolygonGeometryCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id()) {
        return false;
    }
    polygon_ = static_cast<const ChangePolygonGeometryCommand *>(other)->polygon_;
    return true;
}