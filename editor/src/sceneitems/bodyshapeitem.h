//
// bodyshapeitem.h
// A QGraphicsScene shape representing a body in the map.
//

#ifndef _BODYSHAPEITEM_H_
#define _BODYSHAPEITEM_H_

#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include "shapeitem.h"

struct Body;

class BodyShapeItem : public QGraphicsItem, public ShapeItem {  
public:
    explicit BodyShapeItem(QSharedPointer<Body> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);  

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void sync();
    virtual void commit();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QGraphicsItem *innerShape() const;
    virtual QRectF boundingRect() const;

    QBrush brush() const { return brush_; }
    void setBrush(const QBrush& brush) { brush_ = brush; }

    QPen pen() const { return pen_; }
    void setPen(const QPen& pen) { pen_ = pen; }

    virtual int type() const { return kBodyShapeItem; }

private:
    QBrush brush_;
    QPen pen_;
};

#endif
