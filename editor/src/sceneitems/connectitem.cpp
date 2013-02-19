//
// connectitem.cpp
//

#include <QtGui>
#include "connectitem.h"
#include "sceneitems.h"
#include "vertexitem.h"

//
//
ConnectItem::ConnectItem(QSharedPointer<Entity> entity, SceneItem *shape1, SceneItem *shape2,
        QGraphicsItem *parent, QGraphicsScene *scene) :
    SceneItem(entity),
    QGraphicsLineItem(parent, scene),
    connectionType_(kFixtureConnection),
    shape1_(shape1),
    shape2_(shape2)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
//
void ConnectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen());
    if (isSelected()) {
        painter->setPen(QColor(0, 255, 0));
        painter->drawPath(shape());
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
    if (shape1_) {
        shape1_->addConnection(this);
    }
}

//
//
void ConnectItem::setShape2(SceneItem *shape2)
{
    if (shape2_) {
        shape2_->removeConnection(this);
    }
    shape2_ = shape2;
    if (shape2_) {
        shape2_->addConnection(this);
    }
}

//
//
void ConnectItem::addVertexItem(VertexItem *vertexItem)
{
    childItems().append(vertexItem);
    vertices_.append(vertexItem);
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

    for (auto& vert : vertices_) {
        vert->sync();
    }
}

//
//
QPainterPath ConnectItem::shape() const
{
    // Construct a slightly thicker shape than a single line
    QLineF normal = line().normalVector().unitVector();
    QLineF dir = line().unitVector();
    qreal threshold = scene()->views().takeFirst()->mapToScene(QRect(0, 0, 16, 16)).boundingRect().width();
    QPointF perp = QPointF(normal.dx(), normal.dy()) * threshold;
    QPointF par = QPointF(dir.dx(), dir.dy()) * threshold;
    QPainterPath path;
    path.moveTo(line().p1() - perp / 2 - par / 2);
    path.lineTo(line().p1() + perp / 2 - par / 2);
    path.lineTo(line().p2() + perp / 2 + par / 2);
    path.lineTo(line().p2() - perp / 2 + par / 2);
    path.closeSubpath();
    return path;
}

//
//
QVariant ConnectItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    // Hide children if not selected
    if (change == ItemSelectedHasChanged) {
        for (auto& child : childItems()) {
            child->setVisible(isSelected());
        }
    }
    return QGraphicsItem::itemChange(change, value);
}
