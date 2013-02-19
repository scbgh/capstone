//
//  vertexitem.cpp
//

#include "mapscene.h"
#include "vertexitem.h"

//
//
VertexItem::VertexItem(std::function<QPointF()> syncFunc, std::function<void(QPointF)> commitFunc,
        QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsEllipseItem(parent, scene),
    syncFunc_(syncFunc),
    commitFunc_(commitFunc)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

//
void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush drawBrush = brush();
    QPen drawPen = pen();

    if (isSelected()) {
        drawBrush = QColor(255, 255, 255, 128);
        drawPen = QColor(255, 255, 255);
    }

    painter->setBrush(drawBrush);
    painter->setPen(drawPen);
    painter->drawEllipse(boundingRect());
}

//
//
void VertexItem::sync()
{
    setPos(syncFunc_());
}

//
// Commit changes to this Shape item to the underlying data object
void VertexItem::commit()
{
    commitFunc_(scenePos());
}

//
//
QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    MapScene *mapScene = (MapScene *)scene();
    if (change == ItemPositionChange && mapScene) {
        QPointF newPos = mapScene->snapPoint(value.toPointF());
        return newPos;
    } else if (change == ItemPositionHasChanged) {
        commit();
    }
    return QGraphicsItem::itemChange(change, value);
}
