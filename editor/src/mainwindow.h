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

private slots:
    void gridToggled(bool value);

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

    QAction *quitAct_;
    QAction *showGridAct_;
    QAction *snapToGridAct_;

    QGraphicsView *view_;
    QMenu *fileMenu_;
    QMenu *editMenu_;
    QMenu *viewMenu_;
    QToolBar *fileToolBar_;
    QVBoxLayout *toolBoxLayout_;
    QButtonGroup *toolButtonGroup_;
    MapScene *scene_;
    PropertyBrowser *propertyBrowser_;
};

#endif
