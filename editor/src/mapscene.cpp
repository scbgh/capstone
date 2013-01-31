//
// mapscene.cpp
//

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
    drawing_(false),
    mode_(kSelectMode)
    undoStack_(undoStack),
{
    addRect(0, 0, 1, 1, QPen(Qt::red));
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
            poly << mouseEvent->scenePos();
            polyItem_->setPolygon(poly);
        }
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
            curPoly_ << mouseEvent->scenePos();
            polyItem_ = new PolygonShapeItem(QSharedPointer<PolygonShape>(new PolygonShape));
            polyItem_->setPen(QColor(255, 128, 128));
            polyItem_->setBrush(QColor(255, 128, 128, 128));
            polyItem_->setPolygon(curPoly_);
            addItem(polyItem_);
        }
    } else {
        if (mode_ == kPolygonMode) {
            if (mouseEvent->button() == Qt::LeftButton) {
                curPoly_ << mouseEvent->scenePos();
                polyItem_->setPolygon(curPoly_);
            } else if (mouseEvent->button() == Qt::RightButton) {
                drawing_ = false;
                polyItem_->setComplete(true);
                polyItem_ = NULL;
            }
        }
    }
}

//
//
void MapScene::setMap(GameMap& map)
{
    map_ = QSharedPointer<GameMap>(&map);
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

    for (const auto& shape : map_->shapes) { addShape(*shape); }
    for (const auto& body : map_->bodies) { addBody(*body); }
    for (const auto& fixture : map_->fixtures) { addFixture(*fixture); }
    for (const auto& joint : map_->joints) { addJoint(*joint); }
}

//
//
void MapScene::addShape(const Shape& shape)
{
}

//
//
void MapScene::addBody(const Body& body)
{
}

//
//
void MapScene::addFixture(const Fixture& fixture)
{
}

//
//
void MapScene::addJoint(const Joint& joint)
{
}
