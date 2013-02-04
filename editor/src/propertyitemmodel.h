//
// propertyitemmodel.h
// An item model supporting the HasProperties interface
//

#ifndef _PROPERTYBROWSER_H_
#define _PROPERTYBROWSER_H_

#include <QSharedPointer>
#include <QAbstractListModel>

class ShapeItem;
class HasProperties;

struct PropertyIndex {
    QSharedPointer<HasProperties> target;
    QString name;
};
Q_DECLARE_METATYPE(PropertyIndex);

class PropertyItemModel : public QAbstractListModel {
    Q_OBJECT

public:
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int rule = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setSource(QSharedPointer<HasProperties> source);
    QSharedPointer<HasProperties> source() const { return source_; }

public slots:
    void invalidated();

private:
    QSharedPointer<HasProperties> source_;
};

#endif
