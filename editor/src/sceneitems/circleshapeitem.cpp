//
//  circleshapeitem.cpp
//

#include <QtGui>
#include "mapdata.h"
#include "mapscene.h"
#include "circleshapeitem.h"

//
//
CircleShapeItem::CircleShapeItem(QSharedPointer<CircleShape> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    ShapeItem(shape),
    QGraphicsEllipseItem(parent, scene),
    complete_(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
//
void CircleShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush drawBrush = brush();
    QPen drawPen = pen();

    if (isSelected()) {
        drawBrush = QColor(128, 255, 128, 128);
        drawPen = QColor(128, 255, 128);
    }

    painter->setBrush(drawBrush);
    painter->setPen(drawPen);
    if (complete_) {
        painter->drawEllipse(boundingRect());
    } else {
        painter->setBrush(Qt::transparent);
        painter->drawEllipse(boundingRect());
    }
}

//
//
void CircleShapeItem::sync()
{
    QSharedPointer<CircleShape> circle = qSharedPointerCast<CircleShape>(shape_);
    setRect(-circle->radius, -circle->radius, 2*circle->radius, 2*circle->radius);
    setPos(circle->position);
}

//
// Commit changes to this Shape item to the underlying data object
void CircleShapeItem::commit()
{
    QSharedPointer<CircleShape> circle = qSharedPointerCast<CircleShape>(shape_);
    circle->beginUpdate();
    circle->radius = rect().width() / 2;
    circle->position = pos();
    circle->endUpdate();
}

//
//
QVariant CircleShapeItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && mapScene()) {
        QPointF newPos = mapScene()->snapPoint(value.toPointF());
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}
