//
// propertyitemmodel.h
// An item model supporting the HasProperties interface
//

#ifndef _PROPERTYBROWSER_H_
#define _PROPERTYBROWSER_H_

#include <QSharedPointer>
#include <QAbstractListModel>
#include <QSet>

class QUndoStack;
class ShapeItem;
class HasProperties;

class PropertyItemModel : public QAbstractListModel {
    Q_OBJECT

public:
    PropertyItemModel(QUndoStack *undoStack, QObject *parent = 0);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void setSource(QSharedPointer<HasProperties> source);
    QSharedPointer<HasProperties> source() const { return source_; }

public slots:
    void invalidated();

private:
    QSharedPointer<HasProperties> source_;
    QUndoStack *undoStack_;
};

#endif
