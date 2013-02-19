//
//  vertexitem.h
//  An item representing a vertex in the map scene
//

#ifndef _VERTEXITEM_H_
#define _VERTEXITEM_H_

#include "sceneitem.h"
#include <functional>
#include <QGraphicsEllipseItem>

class VertexItem : public QGraphicsEllipseItem {
public:
    explicit VertexItem(std::function<QPointF()> syncFunc, std::function<void(QPointF)> commitFunc,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void sync();
    void commit();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    std::function<QPointF()> syncFunc_;
    std::function<void(QPointF)> commitFunc_;
};

#endif
