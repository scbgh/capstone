//
// enumitemmodel.inl
//

#include "util.h"

//
//
template <typename Parent, typename Enum>
EnumItemModel<Parent, Enum>::EnumItemModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

//
//
template <typename Parent, typename Enum>
int EnumItemModel<Parent, Enum>::rowCount(const QModelIndex& parent) const
{
    return enumToMetaEnum<Parent, Enum>().keyCount();
}

//
//
template <typename Parent, typename Enum>
QVariant EnumItemModel<Parent, Enum>::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::UserRole)) {
        return QVariant();
    }

    QMetaEnum metaEnum = enumToMetaEnum<Parent, Enum>();
    int row = index.row();
    if (row >= metaEnum.keyCount()) {
        return QVariant();
    }
    
    if (role == Qt::DisplayRole) {
        return metaEnum.key(row);
    } else {
        return QVariant::fromValue((Enum)metaEnum.value(row));
    }
}

//
//
template <typename Parent, typename Enum>
QModelIndexList EnumItemModel<Parent, Enum>::match(const QModelIndex& start, int role, const QVariant& value, int hits,
        Qt::MatchFlags flags) const
{
    QModelIndexList list;
    for (int i = 0; i < rowCount(); i++) {
        QModelIndex index = createIndex(i, 0);
        QVariant v = data(index, Qt::UserRole);
        if (v.value<Enum>() == value.value<Enum>()) {
            list << index;
        }
    }
    return list;
}   
