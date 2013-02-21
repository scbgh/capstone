//
// connectitem.h
// A scene item that connects two other object
//

#ifndef _CONNECTITEM_H_
#define _CONNECTITEM_H_

#include <QGraphicsLineItem>
#include "sceneitem.h"

class VertexItem;

enum ConnectionType { kFixtureConnection, kJointConnection };

class ConnectItem : public QObject, public QGraphicsLineItem, public SceneItem {
    Q_OBJECT

public:
    ConnectItem(QSharedPointer<Entity> entity, SceneItem *shape1, SceneItem *shape2,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual int type() const { return kConnectItem; }

    SceneItem *shape1() const { return shape1_; }
    void setShape1(SceneItem *shape1);

    SceneItem *shape2() const { return shape2_; }
    void setShape2(SceneItem *shape2);

    ConnectionType connectionType() const { return connectionType_; }
    void setConnectionType(ConnectionType connectionType) { connectionType_ = connectionType; }

    void addVertexItem(VertexItem *vertexItem);

    virtual QPainterPath shape() const;
    virtual QGraphicsItem *innerShape() const { return (QGraphicsItem *)this; };

public slots:
    void sync();

signals:
    void tryingToDetach(bool isShape1);

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
    SceneItem *shape1_;
    SceneItem *shape2_; 
    ConnectionType connectionType_;
    QVector<VertexItem *> vertices_;
};

#endif
