//
// sceneitem.h
// Base mixin for all scene items
//

#ifndef _SCENEITEM_H_
#define _SCENEITEM_H_

#include <QVector>
#include <QGraphicsItem>
#include <QSharedPointer>
#include "mapdata.h"

class ConnectItem;
class QGraphicsItem;
class MapScene;

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
    SceneItem(QSharedPointer<Entity> entity) :
        entity_(entity)
    {
    }

    virtual QGraphicsItem *innerShape() const = 0;

    virtual const QVector<ConnectItem *>& connections() const { return connections_; };
    virtual void addConnection(ConnectItem *connection) { connections_.append(connection); }
    virtual void removeConnection(ConnectItem *connection)
    {
        int idx = connections_.indexOf(connection);
        if (idx != -1) {
            connections_.remove(idx);
        }
    }
    virtual QSharedPointer<Entity> entity() const { return entity_; }

    void syncConnections();

protected:
    MapScene *mapScene() { return (MapScene *)innerShape()->scene(); }

    QVector<ConnectItem *> connections_;
    QSharedPointer<Entity> entity_;
};

#endif
