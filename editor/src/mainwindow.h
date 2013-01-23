//
// mainwindow.h
// The main window of the editor.
//

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

class QAction;
class QMenu;


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
};

#endif
