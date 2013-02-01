//
// shapeitem.h
// Base class for items in the map scene
//

#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include <QtGui>

class MapScene;
class QAbstractGraphicsShapeItem;
struct Shape;

enum ShapeItemTypes {
    kPolygonShapeItem,
    kCircleShapeItem
};

class ShapeItem {
public:
    explicit ShapeItem(QSharedPointer<Shape> shape);

    QSharedPointer<Shape> underlyingShape() const { return shape_; }

    virtual void sync() = 0;
    virtual void commit() = 0;

    QPointF preMovePoint() const { return preMovePoint_; }
    void setPreMovePoint(const QPointF& preMovePoint) { preMovePoint_ = preMovePoint; }

    virtual int type() const = 0;

    virtual QAbstractGraphicsShapeItem *innerShape() const = 0;
protected:
    MapScene *mapScene() { return (MapScene *)innerShape()->scene(); }

    QSharedPointer<Shape> shape_;
    QPointF preMovePoint_;
};

#endif
