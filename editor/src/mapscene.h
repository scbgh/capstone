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
class ShapeItem;
class PolygonShapeItem;
class CircleShapeItem;
class BodyShapeItem;
class ConnectItem;

class MapScene : public QGraphicsScene {
    Q_OBJECT

public:
    enum Mode { kSelectMode, kPolygonMode, kCircleMode, kBodyMode, kFixtureMode, kJointMode };
    enum JointMode { kDistanceJoint, kFrictionJoint, kGearJoint, kPrismaticJoint, kPulleyJoint, kRevoluteJoint,
        kRopeJoint, kWeldJoint, kWheelJoint };

    explicit MapScene(QGraphicsView *view, QUndoStack *undoStack, QObject *parent = 0);

    QSharedPointer<GameMap> map() const { return map_; }
    void setMap(QSharedPointer<GameMap> map_);

    float gridSize() const { return gridSize_; }
    void setGridSize(float gridSize) { gridSize_ = gridSize; update(); }

    bool showGrid() const { return showGrid_; }
    void setShowGrid(bool showGrid) { showGrid_ = showGrid; update(); }

    bool snapToGrid() const { return snapToGrid_; }
    void setSnapToGrid(bool snapToGrid) { snapToGrid_ = snapToGrid; }

    Mode mode() const { return mode_; }
    void setMode(Mode mode);

    JointMode jointMode() const { return jointMode_; }
    void setJointMode(JointMode jointMode) { jointMode_ = jointMode; }

    QUndoStack *undoStack() const { return undoStack_; }

    // Snap a point to the grid if snap-to-grid is on
    QPointF snapPoint(const QPointF& point);

    // Refresh the items in the scene to match the map
    void sync();

protected:
    virtual void drawBackground(QPainter *painter, const QRectF& rect);
    virtual void drawForeground(QPainter *painter, const QRectF& rect);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void keyPressEvent(QKeyEvent *keyEvent);

private slots:
    void mapInvalidated();

private:
    void beginPolygon(const QPointF& point);
    void beginCircle(const QPointF& point);
    void beginFixture(const QPointF& point, QGraphicsItem *item);
    void beginJoint(const QPointF& point, QGraphicsItem *item);
    void placeBody(const QPointF& point);
    void endPolygon(const QPointF& point);
    void endCircle(const QPointF& point);
    void endFixture(const QPointF& point);
    void endJoint(const QPointF& point);
    void makeVerticesForJoint(ConnectItem *item, QSharedPointer<Joint> joint, bool init);

    void drawGrid(float spacing, QColor color, const QRectF& rect, QPainter *painter);

    void addShape(QSharedPointer<Shape> shape);
    void addFixture(QSharedPointer<Fixture> fixture);
    void addJoint(QSharedPointer<Joint> joint);

    bool itemIsShape(const QGraphicsItem *item) const;

    QSharedPointer<GameMap> map_;
    float gridSize_;
    bool showGrid_;
    bool snapToGrid_;
    QGraphicsView *view_;
    Mode mode_;
    JointMode jointMode_;
    bool drawing_;
    QGraphicsItem *tempItem_;
    QUndoStack *undoStack_;
    bool moving_;
    QScopedPointer<QImage> foreImage_;
    QScopedPointer<QImage> backImage_;

    QColor shapeColor_;
    QColor bodyColor_;
    QColor fixtureColor_;
    QColor jointColor_;

    // Polygon mode
    QPolygonF curPoly_;
    PolygonShapeItem *polyItem_;

    // Circle mode
    QPointF circleOrigin_;
    CircleShapeItem *circleItem_;

    // Fixture/joint mode
    QPointF endPoint_;
    BodyShapeItem *firstShape_;
    ShapeItem *secondShape_;
    ConnectItem *fixtureConnection_;
    ConnectItem *jointConnection_;
    QSharedPointer<Fixture> fixture_;
    QSharedPointer<Joint> joint_;
};

#endif
