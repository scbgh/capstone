//
//  polygonshapeitem.h
//  A QGraphicsScene shape representing a polygon in the map.
//

#ifndef _POLYGONSHAPEITEM_H_
#define _POLYGONSHAPEITEM_H_

#include <QGraphicsPolygonItem>
#include "shapeitem.h"

struct PolygonShape;

class PolygonShapeItem : public QGraphicsPolygonItem, public ShapeItem {  
public:
    explicit PolygonShapeItem(QSharedPointer<PolygonShape> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);  

    bool complete() const { return complete_; }
    void setComplete(bool complete) { complete_ = complete; }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void sync();
    virtual void commit();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QGraphicsItem *innerShape() const { return (QGraphicsItem *)this; }

    virtual int type() const { return kPolygonShapeItem; }
private:
    bool complete_; // complete polygon, should be filled
};

#endif
