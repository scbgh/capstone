//
// propertyitemdelegate.h
// An item delegate for displaying properties in the property viewer
//

#ifndef _PROPERTYITEMDELEGATE_H_
#define _PROPERTYITEMDELEGATE_H_

#include <QItemDelegate>

class PropertyItemDelegate : public QItemDelegate {
    Q_OBJECT 

public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;

public slots:
    void commitAndCloseEditor();
};

#endif
