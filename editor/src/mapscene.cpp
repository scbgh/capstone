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
#include <cmath>
#include <QtAlgorithms>
#include <QtGui>

//
//
MapScene::MapScene(QGraphicsView *view, QUndoStack *undoStack, QObject *parent) :
    QGraphicsScene(parent),
    view_(view),
    gridSize_(0.5),
    showGrid_(true),
    snapToGrid_(true),
    drawing_(false),
    mode_(kSelectMode),
    undoStack_(undoStack),
    firstShape_(NULL),
    secondShape_(NULL),
    shapeColor_(QColor(255, 128, 128)),
    bodyColor_(QColor(128, 128, 255)),
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
}

//
//
void MapScene::drawForeground(QPainter *painter, const QRectF& rect)
{
    // Draw the grid
    painter->setWorldMatrixEnabled(true);

    qreal left = qMax(rect.left() - (fmod(rect.left(), gridSize_)), 0.);
    qreal top = qMax(rect.top() - (fmod(rect.top(), gridSize_)), 0.);
    qreal right = qMin(rect.right(), sceneRect().right());
    qreal bottom = qMin(rect.bottom(), sceneRect().bottom());

    // Don't render the grid if it's too dense
    if (rect.width() / gridSize_ > 1024) {
        return;
    }

    QVarLengthArray<QLineF, 1024> linesX;
    for (qreal x = left; x <= right; x += gridSize_)
        linesX.append(QLineF(x, top, x, bottom));

    QVarLengthArray<QLineF, 1024> linesY;
    for (qreal y = top; y <= bottom; y += gridSize_)
        linesY.append(QLineF(left, y, right, y));

    if (showGrid_) {
        painter->setPen(QColor(255, 255, 255, 64)); // TODO: customizable colour?
        painter->drawLines(linesX.data(), linesX.size());
        painter->drawLines(linesY.data(), linesY.size());
    }
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
            QGraphicsItem *item = itemAt(endPoint_);
            if (item && (item->type() == kPolygonShapeItem || item->type() == kCircleShapeItem)) {
                secondShape_ = dynamic_cast<ShapeItem *>(item);
                if (!secondShape_->connections().isEmpty()) {
                    qDebug() << "!";
                    secondShape_ = NULL;
                } else {
                    endPoint_ = secondShape_->innerShape()->pos();
                }
            }
            fixtureConnection_->setLine(QLineF(firstShape_->pos(), endPoint_));
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
            curPoly_.clear();
            curPoly_ << snapPoint(mouseEvent->scenePos());
            QSharedPointer<PolygonShape> shape = QSharedPointer<PolygonShape>(new PolygonShape);
            tempItem_ = polyItem_ = new PolygonShapeItem(shape);
            shape->shapeItem = polyItem_;
            connect(shape.data(), SIGNAL(invalidated()), shape->shapeItem, SLOT(sync()));
            polyItem_->setPen(shapeColor_);
            polyItem_->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            polyItem_->setPolygon(curPoly_);
            addItem(polyItem_);
        } else if (mode_ == kCircleMode && mouseEvent->button() == Qt::LeftButton) {
            drawing_ = true;
            circleOrigin_ = snapPoint(mouseEvent->scenePos());
            QSharedPointer<CircleShape> shape = QSharedPointer<CircleShape>(new CircleShape);
            tempItem_ = circleItem_ = new CircleShapeItem(shape);
            shape->shapeItem = circleItem_;
            connect(shape.data(), SIGNAL(invalidated()), shape->shapeItem, SLOT(sync()));
            circleItem_->setPen(shapeColor_);
            circleItem_->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            circleItem_->setPos(circleOrigin_);
            addItem(circleItem_);
        } else if (mode_ == kBodyMode && mouseEvent->button() == Qt::LeftButton) {
            QPointF origin = snapPoint(mouseEvent->scenePos());
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
        } else if (mode_ == kFixtureMode && mouseEvent->button() == Qt::LeftButton) {
            QGraphicsItem *item = itemAt(mouseEvent->scenePos());
            if (item && item->type() == kBodyShapeItem) {
                drawing_ = true;
                fixture_ = QSharedPointer<Fixture>(new Fixture);
                drawing_ = true;
                firstShape_ = dynamic_cast<BodyShapeItem *>(item);
                tempItem_ = fixtureConnection_ = new ConnectItem(fixture_, firstShape_, NULL);
                fixtureConnection_->setPen(QColor(255, 0, 0));
                fixtureConnection_->setLine(QLineF(firstShape_->scenePos(), mouseEvent->scenePos()));
                fixture_->connectItem = fixtureConnection_;
                connect(fixture_.data(), SIGNAL(invalidated()), fixture_->connectItem, SLOT(sync()));
                addItem(fixtureConnection_);
            }            
        } else if (mode_ == kSelectMode && mouseEvent->button() == Qt::LeftButton) {
            QGraphicsScene::mousePressEvent(mouseEvent);

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
        } else if (mode_ == kCircleMode) {
            drawing_ = false;
            circleItem_->setComplete(true);
            circleItem_->commit();
            circleItem_->sync();
            undoStack_->push(new CreateShapeCommand(this, circleItem_->underlyingShape()));
            tempItem_ = circleItem_ = NULL;
        } else if (mode_ == kFixtureMode) {
            if (secondShape_) {
                drawing_ = false;
                fixtureConnection_->setShape1(firstShape_);
                fixtureConnection_->setShape2(secondShape_);
                fixture_->body = qSharedPointerCast<Body>(firstShape_->entity());
                fixture_->shape = qSharedPointerCast<Shape>(secondShape_->entity());
                fixtureConnection_->setConnectionType(kFixtureConnection);
                fixtureConnection_->setPen(fixtureColor_);
                fixtureConnection_->sync();
                undoStack_->push(new CreateFixtureCommand(this, fixture_));
                tempItem_ = fixtureConnection_ = NULL;
                fixture_.clear();
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
            } else if (item->type() == kConnectItem && !deletedConnections.contains(item)) {
                ConnectItem *conn = dynamic_cast<ConnectItem *>(sceneItem);
                if (conn->connectionType() == kFixtureConnection) {
                    new DeleteFixtureCommand(this, qSharedPointerCast<Fixture>(conn->entity()), command);
                }
            }
        }
        for (auto item : deletedConnections) {
            ConnectItem *conn = dynamic_cast<ConnectItem *>(item);
            new DeleteFixtureCommand(this, qSharedPointerCast<Fixture>(conn->entity()), command);
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
void MapScene::sync()
{
    clear();
    if (!map_) {
        return;
    }

    for (auto& shape : map_->shapes) { addShape(shape); }
    for (auto& fixture : map_->fixtures) { addFixture(fixture); }
    for (auto& joint : map_->joints) { addJoint(joint); }
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
            innerItem->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            break;
        }
        case kCircle: {
            QSharedPointer<CircleShape> circleShape = qSharedPointerCast<CircleShape>(shape);
            CircleShapeItem *innerItem;
            item = innerItem = new CircleShapeItem(circleShape);
            innerItem->setComplete(true);
            innerItem->setPen(shapeColor_);
            innerItem->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            break;
        }
        case kBody: {
            QSharedPointer<Body> body = qSharedPointerCast<Body>(shape);
            BodyShapeItem *innerItem;
            item = innerItem = new BodyShapeItem(body);
            innerItem->setPen(bodyColor_);
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
}

//
//
void MapScene::addJoint(QSharedPointer<Joint> joint)
{
}
