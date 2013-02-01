//
// mapscene.cpp
//

#include "commands.h"
#include "mapdata.h"
#include "mapscene.h"
#include "sceneitems/polygonshapeitem.h"
#include <cmath>
#include <QtAlgorithms>
#include <stdio.h>
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
    shapeColor_(QColor(255, 128, 128))
{
    addRect(0, 0, 1, 1, QPen(Qt::red));
}

//
//
bool MapScene::itemIsShape(const QGraphicsItem *item) const
{
    return item->type() == kPolygonShapeItem;
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
        if (mode_ == kPolygonMode) {
            drawing_ = true;
            curPoly_.clear();
            curPoly_ << snapPoint(mouseEvent->scenePos());
            QSharedPointer<PolygonShape> shape = QSharedPointer<PolygonShape>(new PolygonShape);
            tempItem_ = polyItem_ = new PolygonShapeItem(shape);
            shape->shapeItem = polyItem_;
            polyItem_->setPen(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue()));
            polyItem_->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            polyItem_->setPolygon(curPoly_);
            addItem(polyItem_);
        } else if (mode_ == kSelectMode) {
            QGraphicsScene::mousePressEvent(mouseEvent);

            if (mouseEvent->button() == Qt::LeftButton) {
                if (itemAt(mouseEvent->scenePos())) {
                    qDebug() << "Move started";
                    moving_ = true;
                    for (auto item : selectedItems()) {
                        if (itemIsShape(item)) {
                            ShapeItem *shapeItem = dynamic_cast<ShapeItem *>(item);
                            shapeItem->setPreMovePoint(shapeItem->underlyingShape()->position);
                        }
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
                undoStack_->push(new CreatePolygonCommand(this, qSharedPointerCast<PolygonShape>(polyItem_->underlyingShape())));
                tempItem_ = polyItem_ = NULL;
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
            qDebug() << "Move ended";
            QUndoCommand *command = new QUndoCommand();
            command->setText("Move Shapes");
            for (auto item : selectedItems()) {
                if (itemIsShape(item)) {
                    ShapeItem *shapeItem = dynamic_cast<ShapeItem *>(item);
                    shapeItem->commit();
                    shapeItem->sync();
                    new MoveShapeCommand(this, shapeItem->underlyingShape(), shapeItem->preMovePoint(), command);
                }
            }
            undoStack_->push(command);
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
        command->setText("Delete Shapes");
        for (auto item : selectedItems()) {
            if (itemIsShape(item)) {
                ShapeItem *shapeItem = dynamic_cast<ShapeItem *>(item);
                new DeleteShapeCommand(this, shapeItem->underlyingShape(), command);
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
    sync();
}

//
//
void MapScene::setMode(Mode mode)
{
    mode_ = mode;
    if (mode == kSelectMode) {
        view_->setDragMode(QGraphicsView::RubberBandDrag);
    } else {
        view_->setDragMode(QGraphicsView::NoDrag);
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
    for (auto& body : map_->bodies) { addBody(body); }
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

    switch (shape->type()) {
        case kPolygon: {
            QSharedPointer<PolygonShape> polyShape = qSharedPointerCast<PolygonShape>(shape);
            PolygonShapeItem *item = new PolygonShapeItem(polyShape);
            shape->shapeItem = item;
            item->sync();
            addItem(item);
            item->setComplete(true);
            item->setPen(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue()));
            item->setBrush(QColor(shapeColor_.red(), shapeColor_.green(), shapeColor_.blue(), 128));
            break;
        }
        case kCircle: {
            break;
        }
    }
}

//
//
void MapScene::addBody(QSharedPointer<Body> body)
{
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
