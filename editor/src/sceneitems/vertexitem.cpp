//
//  vertexitem.cpp
//

#include "mapscene.h"
#include "vertexitem.h"

//
//
VertexItem::VertexItem(std::function<QPointF()> syncFunc, std::function<void(QPointF)> commitFunc,
        QSharedPointer<Entity> parentEntity, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsEllipseItem(parent, scene),
    syncFunc_(syncFunc),
    commitFunc_(commitFunc),
    parentEntity_(parentEntity),
    shouldSnap_(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    setRect(QRectF(-0.125, -0.125, 0.25, 0.25));
    setZValue(10000);
    setPen(QColor(255, 255, 255));
    setBrush(QColor(255, 255, 255, 128));
}

//
void VertexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush drawBrush = brush();
    QPen drawPen = pen();

    if (isSelected()) {
        drawBrush = QColor(128, 255, 128, 128);
        drawPen = QColor(128, 255, 128);
    }

    painter->setBrush(drawBrush);
    painter->setPen(drawPen);
    painter->drawEllipse(boundingRect());
}

//
//
void VertexItem::sync()
{
    //setPos(mapToParent(syncFunc_() - QPointF(0.25, -0.25)));
    QPointF pos = syncFunc_();
    setPos(pos);
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
    if (change == ItemPositionChange && mapScene && shouldSnap_) {
        QPointF newPos = mapScene->snapPoint(value.toPointF());
        return newPos;
    } else if (change == ItemPositionHasChanged) {
        commit();
    }
    return QGraphicsItem::itemChange(change, value);
}
