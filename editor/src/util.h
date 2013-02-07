//
// util.h
// Utility functions
//

#ifndef _UTIL_H_
#define _UTIL_H_

#include <QtCore>

template <typename T>
QVariant::Type variantUserType()
{
    return (QVariant::Type)QVariant::fromValue(T()).userType();
}

template <typename Parent, typename Enum>
QMetaEnum enumToMetaEnum()
{
    const QMetaObject& meta = Parent::staticMetaObject;
    const char *enumName = QString(QMetaType::typeName(qMetaTypeId<Enum>()))
        .split("::").takeLast().toAscii().data();
    int idx = meta.indexOfEnumerator(enumName);
    return meta.enumerator(idx);
}

template <typename Parent, typename Enum>
QString enumToString(Enum val)
{
    return QString(enumToMetaEnum<Parent, Enum>().valueToKey(val));
}

#endif
