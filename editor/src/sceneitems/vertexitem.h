//
//  vertexitem.h
//  An item representing a vertex in the map scene
//

#ifndef _VERTEXITEM_H_
#define _VERTEXITEM_H_

#include <QGraphicsItem>

class VertexItem : QGraphicsItem {
public:
    explicit VertexItem(int id, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

public slots:
    void vertexMoved(int id);
    void vertexDeleted(int id);

private:
    int id_;
};

#endif
