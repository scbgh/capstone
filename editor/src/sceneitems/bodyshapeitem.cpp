//
// bodyshapeitem.cpp
//

#include <QtGui>
#include "mapdata.h"
#include "mapscene.h"
#include "bodyshapeitem.h"
#include "vertexitem.h"
#include "util.h"

//
//
BodyShapeItem::BodyShapeItem(QSharedPointer<Body> shape, QGraphicsItem *parent, QGraphicsScene *scene) :
    ShapeItem(shape),
    QGraphicsItem(parent, scene)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    auto commitFunc = [&](QPointF pt)
    {
        QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
        body->beginUpdate();
        body->imageOffset = mapFromScene(pt);
        body->endUpdate();
    };
    auto syncFunc = [&]()
    {
        QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
        return body->imageOffset;
    };
    vertex_ = new VertexItem(syncFunc, commitFunc, shape);
    vertex_->setBrush(QColor(255, 100, 100));
    vertex_->setPen(QColor(255, 255, 255));
    vertex_->setParentItem(this);
    vertex_->setShouldSnap(true);
    childItems().append(vertex_);
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

    QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
    QSizeF scaledSize = QSizeF(image_.size()) / 32; //HACK: TODO: remove magic number
    painter->setOpacity(0.5);
    painter->drawImage(QRectF(body->imageOffset.x() - scaledSize.width() / 2,
        body->imageOffset.y() - scaledSize.height() / 2, scaledSize.width(), scaledSize.height()), image_);
}

//
//
void BodyShapeItem::sync()
{
    QSharedPointer<Body> body = qSharedPointerCast<Body>(underlyingShape());
    setPos(body->position);

    if (imageName_ != body->image) {
        image_ = QImage(resourceFileName(body->image)).mirrored();
        imageName_ = body->image;
        update();
    }

    vertex_->sync();
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
