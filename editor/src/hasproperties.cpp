//
// hasproperties.cpp
//

#include "mapdata.h"
#include "hasproperties.h"
#include "util.h"

//
//
QString HasProperties::getPropertyDisplay(const QString& name) const
{
    QVariant value = getProperty(name);
    if (value.canConvert<QString>() && value.type() != QVariant::Bool) {
        return value.value<QString>();
    }

    #define CHECK_USERTYPE(p, e) \
        if (value.userType() == variantUserType<p::e>()) { \
            return enumToString<p, p::e>(value.value<p::e>()); \
        }

    CHECK_USERTYPE(Body, BodyType);

    #undef CHECK_USERTYPE

    switch (value.type()) {
        case QVariant::Bool:
            return value.value<bool>() ? "True" : "False";
            break;
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