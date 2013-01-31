//
//  polygonshapeitem.cpp
//

#include <QtGui>
#include "mapdata.h"
#include "mapscene.h"
#include "polygonshapeitem.h"

//
//
PolygonShapeItem::PolygonShapeItem(QSharedPointer<PolygonShape> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    ShapeItem(shape, parent, scene),
    complete_(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
//
void PolygonShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
        painter->drawPolygon(polygon());
    } else {
        painter->drawPolyline(polygon());
    }
}

//
//
void PolygonShapeItem::sync()
{
    QSharedPointer<PolygonShape> poly = qSharedPointerCast<PolygonShape>(shape_);
    setSuppressCommands(true);
    setPolygon(poly->polygon);
    setPos(poly->position);
    setSuppressCommands(false);
}

//
// Commit changes to this Shape item to the underlying data object
void PolygonShapeItem::commit()
{
    QSharedPointer<PolygonShape> poly = qSharedPointerCast<PolygonShape>(shape_);
    QRectF rect = polygon().boundingRect();
    QPointF center = mapScene()->snapPoint(rect.center());
    poly->polygon = polygon();
    poly->polygon.translate(-center.x(), -center.y());
    poly->position = center;
}