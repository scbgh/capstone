//
// pointeditor.cpp
//

#include "pointeditor.h"

//
//
PointEditor::PointEditor(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout;
    setLayout(layout);
    xSpin = new QDoubleSpinBox();
    ySpin = new QDoubleSpinBox();
    layout->setSpacing(0);
    layout->addWidget(xSpin);
    layout->addWidget(ySpin);
    layout->setContentsMargins(0, 0, 0, 0);
}

//
//
QPointF PointEditor::computePoint() const
{
    return QPointF(xSpin->value(), ySpin->value());
}

//
//
void PointEditor::setPoint(QPointF point)
{
    xSpin->setValue(point.x());
    ySpin->setValue(point.y());
}
