//
// comboeditor.h
// An editor widget that uses a ComboBox's itemdata as its data source
//

#ifndef _COMBOEDITOR_H_
#define _COMBOEDITOR_H_

#include <QComboBox>

class ComboEditor : public QComboBox {
    Q_OBJECT
    Q_PROPERTY(QVariant currentData READ currentData WRITE setCurrentData USER true)

public:
    ComboEditor(QWidget *parent = 0);
    QVariant currentData() const;
    void setCurrentData(const QVariant& currentData);
};

#endif
