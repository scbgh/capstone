//
//  circleshapeitem.h
//  A QGraphicsScene shape representing a circle in the map.
//

#ifndef _CIRCLESHAPEITEM_H_
#define _CIRCLESHAPEITEM_H_

#include <QGraphicsEllipseItem>
#include "shapeitem.h"
#include "sceneitem.h"

struct CircleShape;

class CircleShapeItem : public QGraphicsEllipseItem, public ShapeItem {  
public:
    explicit CircleShapeItem(QSharedPointer<CircleShape> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);  

    bool complete() const { return complete_; }
    void setComplete(bool complete) { complete_ = complete; }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void sync();
    virtual void commit();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QGraphicsItem *innerShape() const { return (QGraphicsItem *)this; }

    virtual int type() const { return kCircleShapeItem; }
private:
    bool complete_; // complete polygon, should be filled
};

#endif
