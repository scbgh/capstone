//
//  shapeitem.cpp
//

#include <QtGui>
#include "commands.h"
#include "mapscene.h"
#include "shapeitem.h"

//
//
ShapeItem::ShapeItem(QSharedPointer<Shape> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsPolygonItem(parent, scene),
    shape_(shape),
    suppressCommands_(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
//
void ShapeItem::sync()
{
}

//
//
void ShapeItem::commit()
{
}

//
//
QVariant ShapeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && mapScene()) {
        QPointF newPos = mapScene()->snapPoint(value.toPointF());
        return newPos;
    } else if (change == ItemPositionHasChanged && mapScene()) {
        if (!suppressCommands_) {
            mapScene()->undoStack()->push(new MoveShapeCommand(mapScene(), shape_, value.toPointF()));
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
