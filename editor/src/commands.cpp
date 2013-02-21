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
    setText("Create Shape");
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
CreateFixtureCommand::CreateFixtureCommand(MapScene *scene, QSharedPointer<Fixture> fixture, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    fixture_(fixture)
{
    setText("Create Fixture");
}

//
//
void CreateFixtureCommand::undo()
{
    scene_->map()->fixtures.pop_back();
    scene_->removeItem(fixture_->connectItem->innerShape());
    fixture_->connectItem->sync();
}

//
//
void CreateFixtureCommand::redo()
{
    scene_->map()->fixtures.append(fixture_);
    if (!scene_->items().contains(fixture_->connectItem->innerShape())) {
        scene_->addItem(fixture_->connectItem->innerShape());
    }
    fixture_->connectItem->sync();
}

//
//
CreateJointCommand::CreateJointCommand(MapScene *scene, QSharedPointer<Joint> joint, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    joint_(joint)
{
    setText("Create Joint");
}

//
//
void CreateJointCommand::undo()
{
    scene_->map()->joints.pop_back();
    scene_->removeItem(joint_->connectItem->innerShape());
    joint_->connectItem->sync();
}

//
//
void CreateJointCommand::redo()
{
    scene_->map()->joints.append(joint_);
    if (!scene_->items().contains(joint_->connectItem->innerShape())) {
        scene_->addItem(joint_->connectItem->innerShape());
    }
    joint_->connectItem->sync();
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
    scene_->map()->shapes.remove(scene_->map()->shapes.indexOf(shape_));
    scene_->removeItem(shape_->shapeItem->innerShape());
    shape_->shapeItem->sync();
}

//
//
DeleteFixtureCommand::DeleteFixtureCommand(MapScene *scene, QSharedPointer<Fixture> fixture, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape1_(fixture_->connectItem->shape1()),
    shape2_(fixture_->connectItem->shape2()),
    fixture_(fixture)
{
    setText("Delete Fixture");
}

//
//
void DeleteFixtureCommand::undo()
{
    scene_->map()->fixtures.append(fixture_);
    if (!scene_->items().contains(fixture_->connectItem->innerShape())) {
        scene_->addItem(fixture_->connectItem->innerShape());
        fixture_->connectItem->setShape1(shape1_);
        fixture_->connectItem->setShape2(shape2_);
    }
    fixture_->connectItem->sync();
}

//
//
void DeleteFixtureCommand::redo()
{
    scene_->map()->fixtures.remove(scene_->map()->fixtures.indexOf(fixture_));
    fixture_->connectItem->setShape1(NULL);
    fixture_->connectItem->setShape2(NULL);
    scene_->removeItem(fixture_->connectItem->innerShape());
    fixture_->connectItem->sync();
}

//
//
DeleteJointCommand::DeleteJointCommand(MapScene *scene, QSharedPointer<Joint> joint, QUndoCommand *parent) :
    QUndoCommand(parent),
    scene_(scene),
    shape1_(joint_->connectItem->shape1()),
    shape2_(joint_->connectItem->shape2()),
    joint_(joint)
{
    setText("Delete Joint");
}

//
//
void DeleteJointCommand::undo()
{
    scene_->map()->joints.append(joint_);
    if (!scene_->items().contains(joint_->connectItem->innerShape())) {
        scene_->addItem(joint_->connectItem->innerShape());
        joint_->connectItem->setShape1(shape1_);
        joint_->connectItem->setShape2(shape2_);
    }
    joint_->connectItem->sync();
}

//
//
void DeleteJointCommand::redo()
{
    scene_->map()->joints.remove(scene_->map()->joints.indexOf(joint_));
    joint_->connectItem->setShape1(NULL);
    joint_->connectItem->setShape2(NULL);
    scene_->removeItem(joint_->connectItem->innerShape());
    joint_->connectItem->sync();
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