//
// enumeditorcreator.h
// Template class for creating editor ComboBox widgets for editing Qt reified enums
//

#ifndef _ENUMEDITORCREATOR_H_
#define _ENUMEDITORCREATOR_H_

#include "util.h"
#include "enumitemmodel.h"
#include "editorwidgets/comboeditor.h"
#include <typeinfo>
#include <QItemEditorCreatorBase>

template <typename Parent, typename Enum>
class EnumEditorCreator : public QItemEditorCreatorBase {
public:
    virtual QWidget *createWidget(QWidget *parent) const
    {
        ComboEditor *combo = new ComboEditor(parent);
        combo->setModel(new EnumItemModel<Parent, Enum>);
        return combo;
    }

    virtual QByteArray valuePropertyName() const
    {
        return QByteArray("currentData");
    }
};

#endif
