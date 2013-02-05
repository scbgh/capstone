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
    QAbstractGraphicsShapeItem(parent, scene)
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
    painter->drawRect(boundingRect());
    painter->drawEllipse(boundingRect().adjusted(0.25, 0.25, -0.25, -0.25));
}

//
//
void BodyShapeItem::sync()
{
    QSharedPointer<Body> body = qSharedPointerCast<Body>(shape_);
    setPos(body->position);
}

//
// Commit changes to this Shape item to the underlying data object
void BodyShapeItem::commit()
{
    QSharedPointer<Body> body = qSharedPointerCast<Body>(shape_);
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
    }
    return QGraphicsItem::itemChange(change, value);
}

//
//
QAbstractGraphicsShapeItem *BodyShapeItem::innerShape() const 
{
    return (QAbstractGraphicsShapeItem *)this;
}

//
//
QRectF BodyShapeItem::boundingRect() const
{
    return QRectF(-0.5, -0.5, 1.0, 1.0);
}