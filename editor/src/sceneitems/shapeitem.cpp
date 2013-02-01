//
//  shapeitem.cpp
//

#include <QtGui>
#include "commands.h"
#include "mapscene.h"
#include "shapeitem.h"

//
//
ShapeItem::ShapeItem(QSharedPointer<Shape> shape) :
    shape_(shape)
{
}