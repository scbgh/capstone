//
// mainwindow.cpp
//

#include "mainwindow.h"
#include "mapscene.h"
#include "propertybrowser.h"
#include <QtGui>

//
//
MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    view_ = new QGraphicsView(this);
    scene_ = new MapScene(view_);

    view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    scene_->setSceneRect(QRect(0, 0, 40, 23));
    view_->setScene(scene_);

    view_->setTransformationAnchor(QGraphicsView::NoAnchor);
    view_->scale(32, 32); // One unit is 32x32 pixels at default zoom
    view_->translate(20, 11.5);
    view_->setMouseTracking(true);

    toolButtonGroup_ = new QButtonGroup(this);
    QAbstractButton *selectButton = createToolButton("Select");
    toolButtonGroup_->addButton(selectButton, 0);
    toolButtonGroup_->addButton(createToolButton("Draw Polygon"), 1);
    toolButtonGroup_->addButton(createToolButton("Draw Circle"), 2);
    connect(toolButtonGroup_, SIGNAL(buttonClicked(int)), this, SLOT(toolButtonClicked(int)));
    selectButton->setChecked(true);

    toolBoxLayout_->addStretch();

    setCentralWidget(view_);
    setWindowTitle(tr("PGEditor"));
    setUnifiedTitleAndToolBarOnMac(true);

    resize(1280, 720);
    toolButtonClicked(0); // Select tool

    readSettings();
}

//
// Load an icon at three sizes, 16x16, 24x24, and 32x32.
QIcon MainWindow::loadIcon(const QString& name)
{
    QIcon icon = QIcon(QString(":/icons16/") + name);
    icon.addFile(QString(":/icons24/") + name);
    icon.addFile(QString(":/icons32/") + name);
    return icon;
}

//
//
QAbstractButton *MainWindow::createToolButton(const QString& name)
{
    QPushButton *button = new QPushButton(name);
    toolBoxLayout_->addWidget(button);
    button->setCheckable(true);
    return button;
}

//
//
void MainWindow::createActions()
{
    quitAct_ = new QAction(tr("&Quit"), this);
    quitAct_->setShortcuts(QKeySequence::Quit);
    quitAct_->setStatusTip(tr("Quit the application"));
    connect(quitAct_, SIGNAL(triggered()), this, SLOT(close()));

    showGridAct_ = new QAction(tr("Show &Grid"), this);
    showGridAct_->setShortcut(Qt::Key_G);
    showGridAct_->setStatusTip(tr("Show or hide the grid"));
    showGridAct_->setCheckable(true);
    showGridAct_->setChecked(true);
    connect(showGridAct_, SIGNAL(toggled(bool)), this, SLOT(gridToggled(bool)));
}

//
//
void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(quitAct_);

    editMenu_ = menuBar()->addMenu(tr("&Edit"));

    viewMenu_ = menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(showGridAct_);
}

//
//
void MainWindow::createToolBars()
{
    fileToolBar_ = addToolBar(tr("File"));
}

//
//
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

//
void MainWindow::createDockWindows()
{
    QDockWidget *propertyDock = new QDockWidget(tr("Properties"), this);
    propertyBrowser_ = new PropertyBrowser(propertyDock);
    propertyDock->setWidget(propertyBrowser_);
    propertyDock->setObjectName("propertyDock");
    addDockWidget(Qt::LeftDockWidgetArea, propertyDock);
    viewMenu_->addAction(propertyDock->toggleViewAction());

    QDockWidget *toolBoxDock = new QDockWidget(tr("Toolbox"), this);
    QWidget *toolBox = new QWidget(toolBoxDock);
    toolBoxLayout_ = new QVBoxLayout(toolBox);
    toolBox->setLayout(toolBoxLayout_);
    toolBoxDock->setWidget(toolBox);
    toolBoxDock->setObjectName("toolBoxDock");
    addDockWidget(Qt::LeftDockWidgetArea, toolBoxDock);
    viewMenu_->addAction(toolBoxDock->toggleViewAction());
}

//
//
void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("window");
    if (settings.contains("geometry")) restoreGeometry(settings.value("geometry").toByteArray());
    if (settings.contains("windowState")) restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();
}

//
//
void MainWindow::writeSettings()
{
    QSettings settings;
    settings.beginGroup("window");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
}

//
//
void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
    event->accept();
}

//
//
void MainWindow::gridToggled(bool value)
{
    scene_->setShowGrid(value);
}

//
//
void MainWindow::toolButtonClicked(int id)
{
    switch (id) {
        case 0:
            scene_->setMode(MapScene::kSelectMode);
            break;
        case 1:
            scene_->setMode(MapScene::kPolygonMode);
            break;
        case 2:
            scene_->setMode(MapScene::kCircleMode);
            break;
    }
}