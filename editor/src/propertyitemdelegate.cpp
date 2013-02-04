//
// propertyitemdelegate.cpp
//

#include "hasproperties.h"
#include "propertyitemmodel.h"
#include "propertyitemdelegate.h"
#include <QtGui>

//
//
QWidget *PropertyItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QLineEdit *edit =  new QLineEdit(parent);
    connect(edit, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return edit;
}

//
//
void PropertyItemDelegate::setEditorData(QWidget *editor, const QModelIndex& index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    PropertyIndex idx = index.model()->data(index, Qt::EditRole).value<PropertyIndex>();
    edit->setText(idx.target->getPropertyDisplay(idx.name));
}

//
//
void PropertyItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(editor);
    PropertyIndex idx = index.model()->data(index, Qt::EditRole).value<PropertyIndex>();
    idx.target->beginUpdate();
    idx.target->setPropertyDisplay(idx.name, edit->text());
    idx.target->endUpdate();
}

//
//
void PropertyItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect);
}

//
//
void PropertyItemDelegate::commitAndCloseEditor()
{
    QLineEdit *edit = qobject_cast<QLineEdit *>(sender());
    emit commitData(edit);
    emit closeEditor(edit);
}