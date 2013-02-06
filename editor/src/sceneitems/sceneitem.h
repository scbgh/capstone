//
// sceneitem.h
// Base mixin for all scene items
//

#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include <QVector>
#include <QGraphicsItem>

class ConnectItem;
class QAbstractGraphicsShapeItem;

enum SceneItemTypes {
    kPolygonShapeItem = QGraphicsItem::UserType + 1,
    kCircleShapeItem,
    kBodyShapeItem,
    kConnectItem
};

//
//
class SceneItem {
public:
    virtual QAbstractGraphicsShapeItem *innerShape() const = 0;

    virtual const QVector<ConnectItem *>& connections() const { return connections_; };
    virtual void addConnection(ConnectItem *connection) { connections_.append(connection); }
    virtual void removeConnection(ConnectItem *connection)
    {
        int idx = connections_.indexOf(connection);
        if (idx != -1) {
            connections_.remove(idx);
        }
    }

private:
    QVector<ConnectItem *> connections_;
};

#endif
