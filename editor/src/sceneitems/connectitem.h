//
// connectitem.h
// A scene item that connects two other object
//

#ifndef _CONNECTITEM_H_
#define _CONNECTITEM_H_

#include <QGraphicsLineItem>
#include "sceneitem.h"

class ConnectItem : public QObject, public QGraphicsLineItem, public SceneItem {
    Q_OBJECT

public:
    ConnectItem(SceneItem *shape1, SceneItem *shape2,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);  

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual int type() const { return kConnectItem; }

    SceneItem *shape1() const { return shape1_; }
    void setShape1(SceneItem *shape1);

    SceneItem *shape2() const { return shape2_; }
    void setShape2(SceneItem *shape2);

    void sync();

    virtual QAbstractGraphicsShapeItem *innerShape() { return (QAbstractGraphicsShapeItem *)this; };

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

signals:
    void tryingToDetach(bool isShape1);

private:
    SceneItem *shape1_;
    SceneItem *shape2_;    
};

#endif
