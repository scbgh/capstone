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
MoveShapeCommand::MoveShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QPointF oldPos, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape),
    oldPos_(oldPos),
    newPos_(shape->position)
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
    shape_->position = newPos_;
    shape_->shapeItem->sync();
}

//
//
DeleteShapeCommand::DeleteShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape)
{
    setText("Delete Shape");
}

//
//
void DeleteShapeCommand::undo()
{
    scene_->map()->shapes.append(shape_);
    if (!scene_->items().contains(shape_->shapeItem)) {
        scene_->addItem(shape_->shapeItem);
    }
    shape_->shapeItem->sync();
}

//
//
void DeleteShapeCommand::redo()
{
    scene_->map()->shapes.pop_back();
    scene_->removeItem(shape_->shapeItem);
    shape_->shapeItem->sync();
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
