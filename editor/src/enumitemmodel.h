//
// enumitemmodel.h
// A model using a Qt reified enum as a data source
//

#ifndef _ENUMITEMMODEL_H_
#define _ENUMITEMMODEL_H_

#include <QStandardItemModel>

template <typename Parent, typename Enum>
class EnumItemModel : public QAbstractListModel {
public:
    EnumItemModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits = 1,
        Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;
};

#include "enumitemmodel.inl"

#endif
