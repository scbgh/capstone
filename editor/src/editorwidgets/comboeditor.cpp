//
// comboeditor.cpp
//

#include "comboeditor.h"

//
//
ComboEditor::ComboEditor(QWidget *parent) :
    QComboBox(parent)
{
}

//
//
QVariant ComboEditor::currentData() const
{
    return itemData(currentIndex());
}

//
//
void ComboEditor::setCurrentData(const QVariant& currentData)
{
    int idx = findData(currentData);
    if (idx != -1) {
        setCurrentIndex(idx);
    }
}
