//
//  polygonshapeitem.cpp
//

#include <QtGui>
#include "polygonshapeitem.h"

//
//
PolygonShapeItem::PolygonShapeItem(QSharedPointer<PolygonShape> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsPolygonItem(parent, scene),
    shape_(shape)
{
}

//
//
void PolygonShapeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(brush());
    painter->setPen(pen());
    if (complete_) {
        painter->drawPolygon(polygon());
    } else {
        painter->drawPolyline(polygon());
    }
}