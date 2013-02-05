//
// hasproperties.cpp
//

#include "hasproperties.h"

//
//
QString HasProperties::getPropertyDisplay(const QString& name) const
{
    QVariant value = getProperty(name);
    if (value.canConvert<QString>()) {
        return value.value<QString>();
    }

    switch (value.type()) {
        case QVariant::Point:
        case QVariant::PointF:
            if (value.type() == QVariant::Point || value.type() == QVariant::PointF) {
                QPointF point = value.value<QPointF>();
                return QString("(%1, %2)").arg(point.x()).arg(point.y());
            }
            break;
        default:
            return QString();
    }

    return QString();
}