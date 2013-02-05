//
// pointeditor.h
// A widget for editing two-dimensional structs
//

#ifndef _POINTEDITOR_H_
#define _POINTEDITOR_H_

#include <QtGui>
#include <QPointF>

class PointEditor : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QPointF point READ computePoint WRITE setPoint USER true)

public:
    PointEditor(QWidget *parent = 0);
    QPointF computePoint() const;
    void setPoint(QPointF point);

private:
    QDoubleSpinBox *xSpin;
    QDoubleSpinBox *ySpin;
};

#endif
