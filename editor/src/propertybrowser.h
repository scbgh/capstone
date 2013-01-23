//
// propertybrowser.h
// A widget that allows the editing of a set of properties.
//

#ifndef _PROPERTYBROWSER_H_
#define _PROPERTYBROWSER_H_

#include <QTableView>

class PropertyBrowser : public QTableView {
    Q_OBJECT

public:
    PropertyBrowser(QWidget *parent = NULL) :
        QTableView(parent)
    {
    }
};

#endif
