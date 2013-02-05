//
//  commands.cpp
//

#include "commands.h"
#include "hasproperties.h"
#include "mapdata.h"
#include "mapscene.h"

//
//
CreateShapeCommand::CreateShapeCommand(MapScene *scene, QSharedPointer<Shape> shape, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape_(shape)
{
    setText("Create Polygon");
}

//
//
void CreateShapeCommand::undo()
{
    scene_->map()->shapes.pop_back();
    scene_->removeItem(shape_->shapeItem->innerShape());
    shape_->shapeItem->sync();
}

//
//
void CreateShapeCommand::redo()
{
    scene_->map()->shapes.append(shape_);
    if (!scene_->items().contains(shape_->shapeItem->innerShape())) {
        scene_->addItem(shape_->shapeItem->innerShape());
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
    shape_->beginUpdate();
    shape_->position = oldPos_;
    shape_->endUpdate();
    shape_->shapeItem->sync();
}

//
//
void MoveShapeCommand::redo()
{
    shape_->beginUpdate();
    shape_->position = newPos_;
    shape_->endUpdate();
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
    if (!scene_->items().contains(shape_->shapeItem->innerShape())) {
        scene_->addItem(shape_->shapeItem->innerShape());
    }
    shape_->shapeItem->sync();
}

//
//
void DeleteShapeCommand::redo()
{
    scene_->map()->shapes.pop_back();
    scene_->removeItem(shape_->shapeItem->innerShape());
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
    shape_->beginUpdate();
    shape_->polygon = oldPolygon_;
    shape_->endUpdate();
    shape_->shapeItem->sync();
}

//
//
void ChangePolygonGeometryCommand::redo()
{
    shape_->beginUpdate();
    shape_->polygon = polygon_;
    shape_->endUpdate();
    shape_->shapeItem->sync();
}

//
//
ChangePropertyCommand::ChangePropertyCommand(QSharedPointer<HasProperties> target, const QString& name, QVariant value,
        QUndoCommand *parent) :
    QUndoCommand(parent),
    target_(target),
    name_(name),
    value_(value),
    oldValue_(target->getProperty(name))
{
    setText(QString("Change Property: %1").arg(name));
}

//
//
void ChangePropertyCommand::undo()
{
    target_->beginUpdate();
    target_->setProperty(name_, oldValue_);
    target_->endUpdate();
}

//
//
void ChangePropertyCommand::redo()
{
    target_->beginUpdate();
    target_->setProperty(name_, value_);
    target_->endUpdate();
}