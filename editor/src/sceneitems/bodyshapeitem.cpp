//
// bodyshapeitem.cpp
//

#include <QtGui>
#include "mapdata.h"
#include "mapscene.h"
#include "bodyshapeitem.h"

//
//
BodyShapeItem::BodyShapeItem(QSharedPointer<Body> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    ShapeItem(shape),
    QGraphicsItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
//
void BodyShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush drawBrush = brush();
    QPen drawPen = pen();

    if (isSelected()) {
        drawBrush = QColor(128, 255, 128, 128);
        drawPen = QColor(128, 255, 128);
    }

    painter->setBrush(drawBrush);
    painter->setPen(drawPen);
    painter->setFont(QFont("Helvetica"));
    painter->drawEllipse(boundingRect());
}

//
//
void BodyShapeItem::sync()
{
    QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
    setPos(body->position);
}

//
// Commit changes to this Shape item to the underlying data object
void BodyShapeItem::commit()
{
    QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
    body->beginUpdate();
    body->position = pos();
    body->endUpdate();
}

//
//
QVariant BodyShapeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && mapScene()) {
        QPointF newPos = mapScene()->snapPoint(value.toPointF());
        return newPos;
    } else if (change == ItemPositionHasChanged) {
        syncConnections();
    }
    return QGraphicsItem::itemChange(change, value);
}

//
//
QGraphicsItem *BodyShapeItem::innerShape() const 
{
    return (QGraphicsItem *)this;
}

//
//
QRectF BodyShapeItem::boundingRect() const
{
    return QRectF(-0.25, -0.25, 0.5, 0.5);
}