//
// propertyitemmodel.cpp
//

#include "propertyitemmodel.h"
#include "hasproperties.h"
#include "mapdata.h"
#include "sceneitems/shapeitem.h"

//
//
void PropertyItemModel::setSource(QSharedPointer<HasProperties> source)
{
    beginResetModel();
    if (source_) {
        disconnect(source_.data(), 0, this, 0);
    }
    source_ = source;
    if (source_) {
        connect(source_.data(), SIGNAL(invalidated()), this, SLOT(invalidated()));
    }
    endResetModel();
}

//
//
int PropertyItemModel::rowCount(const QModelIndex& parent) const
{
    if (!source_) {
        return 0;
    }
    return source_->properties().size();
}

//
//
QVariant PropertyItemModel::data(const QModelIndex& index, int role) const
{
    if (!source_ || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole)) {
        return QVariant();
    }

    int row = index.row();
    if (row >= source_->properties().size()) {
        return QVariant();
    }
    const QString& name = source_->properties()[row];

    if (role == Qt::DisplayRole) {
        return source_->getPropertyDisplay(name);
    } else if (role == Qt::EditRole) {
        PropertyIndex pi = { source_, name };
        return QVariant::fromValue(pi);
    } else {
        return QVariant();
    }
}

//
//
bool PropertyItemModel::setData(const QModelIndex& index, const QVariant& value, int rule)
{
    return false;
}

//
//
Qt::ItemFlags PropertyItemModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return 0;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

//
//
QVariant PropertyItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!source_) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return QVariant("Name");
                    break;
                case 1:
                    return QVariant("Value");
                    break;
                default:
                    return QVariant();
            }
        } else if (orientation == Qt::Vertical) {
            if (section < source_->properties().size()) {
                return QVariant(source_->properties()[section]);
            } else {
                return QVariant();
            }
        }
    }

    return QVariant();
}

//
//
void PropertyItemModel::invalidated()
{
    reset();
}