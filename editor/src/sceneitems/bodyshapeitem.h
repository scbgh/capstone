//
// bodyshapeitem.h
// A QGraphicsScene shape representing a body in the map.
//

#ifndef _BODYSHAPEITEM_H_
#define _BODYSHAPEITEM_H_

#include <QGraphicsItem>
#include "shapeitem.h"

struct Body;

class BodyShapeItem : public QAbstractGraphicsShapeItem, public ShapeItem {  
public:
    explicit BodyShapeItem(QSharedPointer<Body> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);  

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void sync();
    virtual void commit();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QAbstractGraphicsShapeItem *innerShape() const;
    virtual QRectF boundingRect() const;

    virtual int type() const { return kBodyShapeItem; }
};

#endif
