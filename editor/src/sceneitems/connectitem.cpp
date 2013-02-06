//
// connectitem.cpp
//

#include <QtGui>
#include "connectitem.h"
#include "sceneitems.h"

//
//
ConnectItem::ConnectItem(SceneItem *shape1, SceneItem *shape2,
        QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsLineItem(parent, scene),
    shape1_(shape1),
    shape2_(shape2)
{
}

//
//
void ConnectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    if (isSelected()) {
        painter->setPen(QColor(0, 255, 0));
    }
    painter->drawLine(line());
}

//
//
void ConnectItem::setShape1(SceneItem *shape1)
{
    if (shape1_) {
        shape1_->removeConnection(this);
    }
    shape1_ = shape1;
    shape1_->addConnection(this);
}

//
//
void ConnectItem::setShape2(SceneItem *shape2)
{
    if (shape2_) {
        shape2_->removeConnection(this);
    }
    shape2_ = shape2;
    shape2_->addConnection(this);
}

//
//
void ConnectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QVector2D dist1 = QVector2D(line().p1() - event->scenePos());
    QVector2D dist2 = QVector2D(line().p2() - event->scenePos());
    bool isShape1 = dist1.lengthSquared() < dist2.lengthSquared();
    emit tryingToDetach(isShape1);
}

//
//
void ConnectItem::sync()
{
    QLineF l = line();
    if (shape1_) {
        l.setP1(shape1_->innerShape()->pos());
    }
    if (shape2_) {
        l.setP2(shape2_->innerShape()->pos());
    }
    setLine(l);
}