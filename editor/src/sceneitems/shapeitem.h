//
// shapeitem.h
// Base class for items in the map scene
//

#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include <QGraphicsPolygonItem> 

class MapScene;
struct Shape;

enum ShapeItemTypes {
    kPolygonShapeItem = QGraphicsPolygonItem::UserType + 1,
    kCircleShapeItem
};

class ShapeItem : public QGraphicsPolygonItem {
public:
    explicit ShapeItem(QSharedPointer<Shape> shape, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    QSharedPointer<Shape> underlyingShape() const { return shape_; }

    virtual void sync();
    virtual void commit();

    QPointF preMovePoint() const { return preMovePoint_; }
    void setPreMovePoint(const QPointF& preMovePoint) { preMovePoint_ = preMovePoint; }

    virtual int type() const { return Type; }
protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    MapScene *mapScene() { return (MapScene *)scene(); }

    QSharedPointer<Shape> shape_;
    QPointF preMovePoint_;
};

#endif
