//
// mapscene.h
// A graphics scene for handling the editing of game maps.
//

#ifndef _MAPSCENE_H_
#define _MAPSCENE_H_

#include <QGraphicsScene>

class MapScene : public QGraphicsScene {
    Q_OBJECT

public:
    MapScene(QObject *parent = 0) :
        QGraphicsScene(parent)
    {
    }
};

#endif
