//
// mainwindow.h
// The main window of the editor.
//

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

class MapScene;
class PropertyBrowser;
class QAbstractButton;
class QAction;
class QButtonGroup;
class QGraphicsView;
class QMenu;
class QToolBar;
class QVBoxLayout;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    QIcon loadIcon(const QString& name);
    QAbstractButton *createToolButton(const QString& name);

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();

    void readSettings();
    void writeSettings();

    QAction *quitAct;

    QGraphicsView *view;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QToolBar *fileToolBar;
    QVBoxLayout *toolBoxLayout;
    QButtonGroup *toolButtonGroup;
    MapScene *scene;
    PropertyBrowser *propertyBrowser;
};

#endif
