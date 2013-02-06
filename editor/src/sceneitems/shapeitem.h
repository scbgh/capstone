//
// shapeitem.h
// Base class for items in the map scene
//

#ifndef _SHAPEITEM_H_
#define _SHAPEITEM_H_

#include "sceneitem.h"
#include <QtGui>

class MapScene;
class ConnectItem;
struct Shape;

class ShapeItem : public QObject, public SceneItem {
    Q_OBJECT

public:
    explicit ShapeItem(QSharedPointer<Shape> shape);

    QSharedPointer<Shape> underlyingShape() const { return shape_; }

    QPointF preMovePoint() const { return preMovePoint_; }
    void setPreMovePoint(const QPointF& preMovePoint) { preMovePoint_ = preMovePoint; }
public slots:
    virtual void sync() = 0;
    virtual void commit() = 0;
protected:
    MapScene *mapScene() { return (MapScene *)innerShape()->scene(); }

    QSharedPointer<Shape> shape_;
    QPointF preMovePoint_;
};

#endif
