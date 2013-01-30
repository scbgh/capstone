//
//  polygonshapeitem.h
//  A QGraphicsScene shape representing a polygon in the map.
//

#ifndef _POLYGONSHAPEITEM_H_
#define _POLYGONSHAPEITEM_H_

#include <QGraphicsPolygonItem> 

struct PolygonShape;

class PolygonShapeItem : public QGraphicsPolygonItem {
public:
    enum { Type = UserType + 1 };

    explicit PolygonShapeItem(QSharedPointer<PolygonShape> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    int type() const { return Type; }

    bool complete() const { return complete_; }
    void setComplete(bool complete) { complete_ = complete; }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    bool complete_; // complete polygon, should be filled
    QSharedPointer<PolygonShape> shape_;
};

#endif
