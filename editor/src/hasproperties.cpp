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
                return QString("%1 %2").arg(point.x()).arg(point.y());
            }
            break;
        default:
            return QString();
    }

    return QString();
}

//
//
void HasProperties::setPropertyDisplay(const QString& name, const QString& val)
{
    int type = propertyType(name);

    switch (type) {
        case QMetaType::Bool:
            if (val.toLower() == "true" || val == "1") {
                setProperty(name, QVariant(true));
            } else {
                setProperty(name, QVariant(false));
            }
            break;
        case QMetaType::Int:
            setProperty(name, QVariant(val.toInt()));
            break;
        case QMetaType::Double:
            setProperty(name, QVariant(val.toDouble()));
            break;
        case QMetaType::Float:
            setProperty(name, QVariant(val.toFloat()));
            break;
        case QMetaType::QString:
            setProperty(name, val);
            break;
        case QMetaType::QPoint:
        case QMetaType::QPointF: {
            QRegExp re("(\\d*(?:\\.\\d+)?) (\\d*(?:\\.\\d+)?)");
            int pos = re.indexIn(val);
            double x = re.cap(1).toDouble();
            double y = re.cap(2).toDouble();
            QPointF pt = QPointF(x, y);
            if (type == QMetaType::QPoint) {
                setProperty(name, QVariant(pt.toPoint()));
            } else {
                setProperty(name, QVariant(pt));
            }
            break;
        }
        default:
            setProperty(name, QVariant());
            break;
    }
}