//
// mapscene.cpp
//

#include "commands.h"
#include "mapdata.h"
#include "mapscene.h"
#include "sceneitems/bodyshapeitem.h"
#include "sceneitems/circleshapeitem.h"
#include "sceneitems/connectitem.h"
#include "sceneitems/polygonshapeitem.h"
#include "sceneitems/sceneitem.h"
#include "sceneitems/vertexitem.h"
#include <cmath>
#include <QtAlgorithms>
#include <QtGui>

//
//
MapScene::MapScene(QGraphicsView *view, QUndoStack *undoStack, QObject *parent) :
    QGraphicsScene(parent),
    view_(view),
    gridSize_(0.25),
    showGrid_(true),
    snapToGrid_(true),
    drawing_(false),
    mode_(kSelectMode),
    undoStack_(undoStack),
    firstShape_(NULL),
    secondShape_(NULL),
    shapeColor_(QColor(255, 128, 128)),
    bodyColor_(QColor(128, 128, 255)),
    jointColor_(QColor(0, 0, 255)),
    fixtureColor_(QColor(255, 255, 128))
{
    addRect(0, 0, 1, 1, QPen(Qt::red));
}

//
//
bool MapScene::itemIsShape(const QGraphicsItem *item) const
{
    return item->type() == kPolygonShapeItem || item->type() == kCircleShapeItem || item->type() == kBodyShapeItem;
}

//
//
void MapScene::drawBackground(QPainter *painter, const QRectF& rect)
{
    painter->fillRect(rect, Qt::black);
    painter->save();
    painter->setTransform(QTransform::fromTranslate(0, -sceneRect().height()) * QTransform::fromScale(1, -1), true);
    if (backImage_) {
        painter->drawImage(sceneRect(), *backImage_); 
    }
    if (foreImage_) {
        painter->drawImage(sceneRect(), *foreImage_);
    }
    painter->restore();
}

//
//
void MapScene::drawGrid(float spacing, QColor color, const QRectF& rect, QPainter *painter)
{
    qreal left = qMax(rect.left() - (fmod(rect.left(), spacing)), 0.);
    qreal top = qMax(rect.top() - (fmod(rect.top(), spacing)), 0.);
    qreal right = qMin(rect.right(), sceneRect().right());
    qreal bottom = qMin(rect.bottom(), sceneRect().bottom());

    // Don't render the grid if it's too dense
    if (rect.width() / spacing > 1024) {
        return;
    }

    QVarLengthArray<QLineF, 1024> linesX;
    for (qreal x = left; x <= right; x += spacing)
        linesX.append(QLineF(x, top, x, bottom));

    QVarLengthArray<QLineF, 1024> linesY;
    for (qreal y = top; y <= bottom; y += spacing)
        linesY.append(QLineF(left, y, right, y));

    painter->setPen(color);
    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}

//
//
void MapScene::drawForeground(QPainter *painter, const QRectF& rect)
{

    // Draw the grid
    painter->setWorldMatrixEnabled(true);

    if (showGrid_) {
        drawGrid(0.25, QColor(255, 255, 255, 32), rect, painter);
        drawGrid(1.0, QColor(255, 255, 255, 64), rect, painter);
    }
}

//
//
void MapScene::beginPolygon(const QPointF& point)
{
    curPoly_.clear();
    curPoly_ << snapPoint(point);
    QSharedPointer<PolygonShape> shape = QSharedPointer<PolygonShape>(new PolygonShape);
    tempItem_ = polyItem_ = new PolygonShapeItem(shape);
    shape->shapeItem = polyItem_;
    connect(shape.data(), SIGNAL(invalidated()), shape->shapeItem, SLOT(sync()));
    polyItem_->setPen(shapeColor_);
    polyItem_->pen().setWidth(3.0);
    polyItem_->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
    polyItem_->setPolygon(curPoly_);
    addItem(polyItem_);
}

//
//
void MapScene::beginCircle(const QPointF& point)
{
    circleOrigin_ = snapPoint(point);
    QSharedPointer<CircleShape> shape = QSharedPointer<CircleShape>(new CircleShape);
    tempItem_ = circleItem_ = new CircleShapeItem(shape);
    shape->shapeItem = circleItem_;
    connect(shape.data(), SIGNAL(invalidated()), shape->shapeItem, SLOT(sync()));
    circleItem_->setPen(shapeColor_);
    circleItem_->pen().setWidth(3.0);
    circleItem_->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
    circleItem_->setPos(circleOrigin_);
    addItem(circleItem_);
}

//
//
void MapScene::beginFixture(const QPointF& point, QGraphicsItem *item)
{
    fixture_ = QSharedPointer<Fixture>(new Fixture);
    firstShape_ = dynamic_cast<BodyShapeItem *>(item);
    tempItem_ = fixtureConnection_ = new ConnectItem(fixture_, firstShape_, NULL);
    fixtureConnection_->setPen(QColor(255, 0, 0));
    fixtureConnection_->pen().setWidth(3.0);
    fixtureConnection_->setLine(QLineF(firstShape_->scenePos(), point));
    fixtureConnection_->setZValue(-1);
    fixture_->connectItem = fixtureConnection_;
    connect(fixture_.data(), SIGNAL(invalidated()), fixture_->connectItem, SLOT(sync()));
    addItem(fixtureConnection_);
}

//
//
void MapScene::beginJoint(const QPointF& point, QGraphicsItem *item)
{
    switch (jointMode_) {
        case kDistanceJoint:
            joint_ = QSharedPointer<Joint>((Joint *)new DistanceJoint);
            break;
        case kRevoluteJoint:
            joint_ = QSharedPointer<Joint>((Joint *)new RevoluteJoint);
            break;
        default:
            qFatal("Bad joint mode");
            break;
    }
    firstShape_ = dynamic_cast<BodyShapeItem *>(item);
    tempItem_ = jointConnection_ = new ConnectItem(joint_, firstShape_, NULL);
    jointConnection_->setPen(QColor(255, 0, 0));
    jointConnection_->pen().setWidth(3.0);
    jointConnection_->setLine(QLineF(firstShape_->scenePos(), point));
    jointConnection_->setZValue(-1);
    joint_->connectItem = jointConnection_;
    connect(joint_.data(), SIGNAL(invalidated()), joint_->connectItem, SLOT(sync()));
    addItem(jointConnection_);
}

//
//
void MapScene::placeBody(const QPointF& point)
{
    QPointF origin = snapPoint(point);
    QSharedPointer<Body> body = QSharedPointer<Body>(new Body);
    BodyShapeItem *bodyItem = new BodyShapeItem(body);
    body->shapeItem = bodyItem;
    connect(body.data(), SIGNAL(invalidated()), body->shapeItem, SLOT(sync()));
    bodyItem->setPen(bodyColor_);
    bodyItem->setBrush(QColor(bodyColor_.red(), bodyColor_.green(), bodyColor_.blue(), 128));
    bodyItem->setPos(origin);
    bodyItem->commit();
    bodyItem->sync();
    CreateShapeCommand *cmd = new CreateShapeCommand(this, bodyItem->underlyingShape());
    cmd->setText("Create Body");
    undoStack_->push(cmd);
}

//
//
void MapScene::endPolygon(const QPointF& point)
{
    polyItem_->setPolygon(curPoly_);
    QPolygonF poly = curPoly_;
    QRectF rect = polyItem_->sceneBoundingRect();
    QPointF center = snapPoint(rect.center());
    poly.translate(-center.x(), -center.y()); 
    polyItem_->setPolygon(poly);
    polyItem_->setPos(center);
    polyItem_->setComplete(true);
    polyItem_->commit();
    polyItem_->sync();
    undoStack_->push(new CreateShapeCommand(this, polyItem_->underlyingShape()));
    tempItem_ = polyItem_ = NULL;
}

//
//
void MapScene::endCircle(const QPointF& point)
{
    circleItem_->setComplete(true);
    circleItem_->commit();
    circleItem_->sync();
    undoStack_->push(new CreateShapeCommand(this, circleItem_->underlyingShape()));
    tempItem_ = circleItem_ = NULL;
}

//
//
void MapScene::endFixture(const QPointF& point)
{
    fixtureConnection_->setShape1(firstShape_);
    fixtureConnection_->setShape2(secondShape_);
    fixture_->body = qSharedPointerCast<Body>(firstShape_->entity());
    fixture_->shape = qSharedPointerCast<Shape>(secondShape_->entity());
    fixtureConnection_->setConnectionType(kFixtureConnection);
    fixtureConnection_->setPen(fixtureColor_);
    fixtureConnection_->pen().setWidth(3.0);
    fixtureConnection_->setZValue(10000);
    fixtureConnection_->sync();
    undoStack_->push(new CreateFixtureCommand(this, fixture_));
    tempItem_ = fixtureConnection_ = NULL;
    fixture_.clear();
}

//
//
void MapScene::endJoint(const QPointF& point)
{
    jointConnection_->setShape1(firstShape_);
    jointConnection_->setShape2(secondShape_);
    joint_->bodyA = qSharedPointerCast<Body>(firstShape_->entity());
    joint_->bodyB = qSharedPointerCast<Body>(secondShape_->entity());
    makeVerticesForJoint(jointConnection_, joint_, true);
    jointConnection_->setConnectionType(kJointConnection);
    jointConnection_->setPen(jointColor_);
    jointConnection_->pen().setWidth(3.0);
    jointConnection_->setZValue(10000);
    jointConnection_->sync();
    undoStack_->push(new CreateJointCommand(this, joint_));
    tempItem_ = jointConnection_ = NULL;
    joint_.clear();
}

//
//
void MapScene::makeVerticesForJoint(ConnectItem *item, QSharedPointer<Joint> joint, bool init)
{
#define MAKE_VERTEX(cls, name, val) \
    do { \
        cls *ptr = (cls *)joint.data(); \
        VertexItem *v = new VertexItem( \
            [ptr]() { return ptr->name; }, \
            [ptr](QPointF pt) { ptr->beginUpdate(); ptr->name = pt; ptr->endUpdate(); }, \
            joint_ \
        ); \
        item->addVertexItem(v); \
        ptr->beginUpdate(); \
        if (init) { \
            ptr->name = ptr->val; \
        } \
        ptr->endUpdate(); \
    } while (0)

    switch (joint->type()) {
        case kDistance: {
            MAKE_VERTEX(DistanceJoint, anchorA, bodyA->position);
            MAKE_VERTEX(DistanceJoint, anchorB, bodyB->position);
            break;
        }
        case kRevolute: {
            MAKE_VERTEX(RevoluteJoint, anchor, bodyA->position);
            break;            
        }
        default:
            break;
    }

#undef MAKE_VERTEX
}

//
//
void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (drawing_) {
        if (mode_ == kPolygonMode) {
            QPolygonF poly = curPoly_;
            poly << snapPoint(mouseEvent->scenePos());
            polyItem_->setPolygon(poly);
        } else if (mode_ == kCircleMode) {
            qreal radius = QVector2D(mouseEvent->scenePos() - circleOrigin_).length();
            QPointF point = snapPoint(QPointF(radius, 0));
            radius = point.x();
            circleItem_->setRect(-radius, -radius, 2*radius, 2*radius);
        } else if (mode_ == kFixtureMode) {
            endPoint_ = mouseEvent->scenePos();
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && (item->type() == kPolygonShapeItem || item->type() == kCircleShapeItem)) {
                secondShape_ = dynamic_cast<ShapeItem *>(item);
                if (!secondShape_->connections().isEmpty()) {
                    secondShape_ = NULL;
                } else {
                    endPoint_ = secondShape_->innerShape()->pos();
                }
            } else {
                secondShape_ = NULL;
            }
            fixtureConnection_->setLine(QLineF(firstShape_->pos(), endPoint_));
        } else if (mode_ == kJointMode) {
            endPoint_ = mouseEvent->scenePos();
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == kBodyShapeItem) {
                secondShape_ = dynamic_cast<ShapeItem *>(item);
                endPoint_ = secondShape_->innerShape()->pos();
            } else {
                secondShape_ = NULL;
            }
            jointConnection_->setLine(QLineF(firstShape_->pos(), endPoint_));
        }
    } else {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

//
//
void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!drawing_) {
        if (mode_ == kPolygonMode && mouseEvent->button() == Qt::LeftButton) {
            drawing_ = true;
            beginPolygon(mouseEvent->scenePos());
        } else if (mode_ == kCircleMode && mouseEvent->button() == Qt::LeftButton) {
            drawing_ = true;
            beginCircle(mouseEvent->scenePos());
        } else if (mode_ == kBodyMode && mouseEvent->button() == Qt::LeftButton) {
            placeBody(mouseEvent->scenePos());
        } else if (mode_ == kFixtureMode && mouseEvent->button() == Qt::LeftButton) {
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == kBodyShapeItem) {
                drawing_ = true;
                beginFixture(mouseEvent->scenePos(), item);                
            }
        } else if (mode_ == kJointMode && mouseEvent->button() == Qt::LeftButton) {
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == kBodyShapeItem) {
                drawing_ = true;
                beginJoint(mouseEvent->scenePos(), item);                
            }
        } else if (mode_ == kSelectMode && mouseEvent->button() == Qt::LeftButton) {
            QGraphicsScene::mousePressEvent(mouseEvent);

            // Begin moving action
            if (itemAt(mouseEvent->scenePos())) {
                moving_ = true;
                for (auto item : selectedItems()) {
                    if (itemIsShape(item)) {
                        ShapeItem *shapeItem = dynamic_cast<ShapeItem *>(item);
                        shapeItem->setPreMovePoint(shapeItem->underlyingShape()->position);
                    }
                }
            }
        } else {
            QGraphicsScene::mousePressEvent(mouseEvent);
        }
    } else {
        if (mode_ == kPolygonMode) {
            if (mouseEvent->button() == Qt::LeftButton) {
                curPoly_ << snapPoint(mouseEvent->scenePos());
                polyItem_->setPolygon(curPoly_);
            } else if (mouseEvent->button() == Qt::RightButton) {
                drawing_ = false;
                endPolygon(mouseEvent->scenePos());
            }
        } else if (mode_ == kCircleMode) {
            drawing_ = false;
            endCircle(mouseEvent->scenePos());
        } else if (mode_ == kFixtureMode) {
            if (secondShape_) {
                drawing_ = false;
                endFixture(mouseEvent->scenePos());
            }
        } else if (mode_ == kJointMode) {
            if (secondShape_) {
                drawing_ = false;
                endJoint(mouseEvent->scenePos());
            }
        }
    }

    update();
}

//
//
void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mode_ == kSelectMode) {
        if (moving_) {
            QUndoCommand *command = new QUndoCommand();
            command->setText("Move Shapes");
            for (auto item : selectedItems()) {
                if (itemIsShape(item)) {
                    ShapeItem *shapeItem = dynamic_cast<ShapeItem *>(item);
                    if (shapeItem->preMovePoint() != shapeItem->innerShape()->scenePos()) {
                        shapeItem->commit();
                        shapeItem->sync();
                        new MoveShapeCommand(this, shapeItem->underlyingShape(), shapeItem->preMovePoint(), command);
                    }
                }
            }
            if (command->childCount()) {
                undoStack_->push(command);
            }
            moving_ = false;
        }
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

//
//
void MapScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() == Qt::Key_Escape) {
        if (drawing_) {
            drawing_ = false;
            if (tempItem_) {
                removeItem(tempItem_);
            }
            tempItem_ = NULL;
        }
    } else if (keyEvent->key() == Qt::Key_Delete || keyEvent->key() == Qt::Key_Backspace) {
        QUndoCommand *command = new QUndoCommand();
        command->setText("Delete");
        QList<QGraphicsItem *> deletedConnections;
        for (auto item : selectedItems()) {
            SceneItem *sceneItem = dynamic_cast<SceneItem *>(item);
            if (itemIsShape(item)) {
                new DeleteShapeCommand(this, dynamic_cast<ShapeItem *>(sceneItem)->underlyingShape(), command);
                for (auto conn : sceneItem->connections()) {
                    if (!deletedConnections.contains(conn)) {
                        deletedConnections << conn;
                    }
                }
            }
        }

        for (auto item : selectedItems()) {
            SceneItem *sceneItem = dynamic_cast<SceneItem *>(item);
            if (item->type() == kConnectItem && !deletedConnections.contains(item)) {
                ConnectItem *conn = dynamic_cast<ConnectItem *>(sceneItem);
                if (conn->connectionType() == kFixtureConnection) {
                    new DeleteFixtureCommand(this, qSharedPointerCast<Fixture>(conn->entity()), command);
                } else if (conn->connectionType() == kJointConnection) {
                    new DeleteJointCommand(this, qSharedPointerCast<Joint>(conn->entity()), command);
                }
            }
        }

        for (auto item : deletedConnections) {
            ConnectItem *conn = dynamic_cast<ConnectItem *>(item);
            if (conn->connectionType() == kFixtureConnection) {
                new DeleteFixtureCommand(this, qSharedPointerCast<Fixture>(conn->entity()), command);
            } else if (conn->connectionType() == kJointConnection) {
                new DeleteJointCommand(this, qSharedPointerCast<Joint>(conn->entity()), command);
            }
        }

        undoStack_->push(command);
    }
}

//
//
void MapScene::setMap(QSharedPointer<GameMap> map)
{
    map_ = map;
    setSceneRect(QRect(0, 0, map_->width, map_->height));
    connect(map_.data(), SIGNAL(invalidated()), this, SLOT(mapInvalidated())); 
    sync();
}

//
//
void MapScene::setMode(Mode mode)
{
    mode_ = mode;
    if (mode == kSelectMode) {
        view_->setDragMode(QGraphicsView::RubberBandDrag);
        view_->setCursor(Qt::ArrowCursor);
    } else {
        view_->setDragMode(QGraphicsView::NoDrag);
        view_->setCursor(Qt::CrossCursor);
    }
}

//
//
void MapScene::mapInvalidated()
{
    QSettings settings;
    QString resourceDir = settings.value("resources/resourceDirectory").toString();
    QDir dir(resourceDir);

    backImage_.reset(new QImage(dir.absoluteFilePath(map_->backImage)));
    foreImage_.reset(new QImage(dir.absoluteFilePath(map_->foreImage)));
    invalidate();
}

//
//
void MapScene::sync()
{
    clear();
    if (!map_) {
        return;
    }

    for (auto& shape : map_->shapes) { addShape(shape); }
    for (auto& fixture : map_->fixtures) { addFixture(fixture); }
    for (auto& joint : map_->joints) { addJoint(joint); }

    mapInvalidated();
}

//
//
QPointF MapScene::snapPoint(const QPointF& point)
{
    if (!snapToGrid_) {
        return point;
    }

    // Round a point to the nearest integer multiple of the grid size
    QPointF newPoint = point;
    newPoint /= gridSize_;
    newPoint.setX(qRound(newPoint.x()));
    newPoint.setY(qRound(newPoint.y()));
    newPoint *= gridSize_;
    return newPoint;
}

//
//
void MapScene::addShape(QSharedPointer<Shape> shape)
{
    ShapeItem *item;

    switch (shape->type()) {
        case kPolygon: {
            QSharedPointer<PolygonShape> polyShape = qSharedPointerCast<PolygonShape>(shape);
            PolygonShapeItem *innerItem;
            item = innerItem = new PolygonShapeItem(polyShape);
            innerItem->setComplete(true);
            innerItem->setPen(shapeColor_);
            innerItem->pen().setWidth(3.0);
            innerItem->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            break;
        }
        case kCircle: {
            QSharedPointer<CircleShape> circleShape = qSharedPointerCast<CircleShape>(shape);
            CircleShapeItem *innerItem;
            item = innerItem = new CircleShapeItem(circleShape);
            innerItem->setComplete(true);
            innerItem->setPen(shapeColor_);
            innerItem->pen().setWidth(3.0);
            innerItem->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            break;
        }
        case kBody: {
            QSharedPointer<Body> body = qSharedPointerCast<Body>(shape);
            BodyShapeItem *innerItem;
            item = innerItem = new BodyShapeItem(body);
            innerItem->setPen(bodyColor_);
            innerItem->pen().setWidth(3.0);
            innerItem->setBrush(QColor(bodyColor_.red(), bodyColor_.green(), bodyColor_.blue(), 128));
            break;
        }
    }

    shape->shapeItem = item;
    item->sync();
    addItem(item->innerShape());
    connect(shape.data(), SIGNAL(invalidated()), shape->shapeItem, SLOT(sync()));
}

//
//
void MapScene::addFixture(QSharedPointer<Fixture> fixture)
{
    ConnectItem *fixConn = new ConnectItem(fixture, 0, 0);
    fixConn->setShape1(fixture->body->shapeItem);
    fixConn->setShape2(fixture->shape->shapeItem);
    fixConn->setConnectionType(kFixtureConnection);
    fixConn->setPen(fixtureColor_);
    fixConn->pen().setWidth(3.0);

    fixture->connectItem = fixConn;
    fixConn->sync();
    addItem(fixConn->innerShape());
    connect(fixture.data(), SIGNAL(invalidated()), fixture->connectItem, SLOT(sync()));
}

//
//
void MapScene::addJoint(QSharedPointer<Joint> joint)
{
    ConnectItem *jointConn = new ConnectItem(joint, 0, 0);
    jointConn->setShape1(joint->bodyA->shapeItem);
    jointConn->setShape2(joint->bodyB->shapeItem);
    jointConn->setConnectionType(kJointConnection);
    jointConn->setPen(jointColor_);
    jointConn->pen().setWidth(3.0);
    joint->connectItem = jointConn;
    makeVerticesForJoint(jointConn, joint, false);
    jointConn->sync();
    addItem(jointConn->innerShape());
    connect(joint.data(), SIGNAL(invalidated()), joint->connectItem, SLOT(sync()));
}
