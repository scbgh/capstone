//
// enumeditorcreator.h
// Template class for creating editor ComboBox widgets for editing Qt reified enums
//

#ifndef _ENUMEDITORCREATOR_H_
#define _ENUMEDITORCREATOR_H_

#include "util.h"
#include "editorwidgets/comboeditor.h"
#include <typeinfo>
#include <QItemEditorCreatorBase>

template <typename Parent, typename Enum>
class EnumEditorCreator : public QItemEditorCreatorBase {
public:
    virtual QWidget *createWidget(QWidget *parent) const
    {
        ComboEditor *combo = new ComboEditor(parent);
        QMetaObject meta = Parent::staticMetaObject;
        const char *enumName = typeid(Enum).name();
        int idx = meta.indexOfEnumerator(enumName);
        QMetaEnum metaEnum = meta.enumerator(idx);

        for (int i = 0; i < metaEnum.keyCount(); i++) {
            combo->addItem(metaEnum.key(i), QVariant(metaEnum.value(i)));
        }

        return combo;
    }

    virtual QByteArray valuePropertyName() const
    {
        return QByteArray("currentData");
    }
};

#endif
