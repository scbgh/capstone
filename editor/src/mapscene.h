//
// mapscene.h
// A graphics scene for handling the editing of game maps.
//

#ifndef _MAPSCENE_H_
#define _MAPSCENE_H_

#include <QGraphicsScene>

struct Shape;
struct Body;
struct Fixture;
struct Joint;
struct GameMap;
class QGraphicsView;
class QUndoStack;
class PolygonShapeItem;

class MapScene : public QGraphicsScene {
    Q_OBJECT

public:
    enum Mode { kSelectMode, kPolygonMode, kCircleMode, kFixtureMode };

    explicit MapScene(QGraphicsView *view, QUndoStack *undoStack, QObject *parent = 0);

    const GameMap& map() const { return *map_; }
    void setMap(GameMap& map_);

    float gridSize() const { return gridSize_; }
    void setGridSize(float gridSize) { gridSize_ = gridSize; update(); }

    bool showGrid() const { return showGrid_; }
    void setShowGrid(bool showGrid) { showGrid_ = showGrid; update(); }

    Mode mode() const { return mode_; }
    void setMode(Mode mode);

    QUndoStack *undoStack() const { return undoStack_; }

protected:
    virtual void drawBackground(QPainter *painter, const QRectF& rect);
    virtual void drawForeground(QPainter *painter, const QRectF& rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    // Refresh the items in the scene to match the map
    void sync();

    void addShape(const Shape& shape);
    void addBody(const Body& body);
    void addFixture(const Fixture& fixture);
    void addJoint(const Joint& joint);

    QSharedPointer<GameMap> map_;
    float gridSize_;
    bool showGrid_;
    QGraphicsView *view_;
    Mode mode_;
    bool drawing_;
    QUndoStack *undoStack_;

    // Polygon mode
    QPolygonF curPoly_;
    PolygonShapeItem *polyItem_;

};

#endif
