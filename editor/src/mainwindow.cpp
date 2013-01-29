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

    scene = new MapScene(this);
    view = new QGraphicsView(scene);

    toolButtonGroup = new QButtonGroup(this);
    QAbstractButton *selectButton = createToolButton("Select");
    toolButtonGroup->addButton(selectButton, 0);
    toolButtonGroup->addButton(createToolButton("Draw Polygon"), 1);
    toolButtonGroup->addButton(createToolButton("Draw Circle"), 2);
    selectButton->setChecked(true);

    toolBoxLayout->addStretch();


    setCentralWidget(view);
    setWindowTitle(tr("PGEditor"));
    setUnifiedTitleAndToolBarOnMac(true);

    resize(1280, 720);

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
    toolBoxLayout->addWidget(button);
    button->setCheckable(true);
    return button;
}

//
//
void MainWindow::createActions()
{
    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    showGridAct = new QAction(tr("Show &Grid"), this);
    showGridAct->setShortcut(Qt::Key_G);
    showGridAct->setStatusTip(tr("Show or hide the grid"));
    showGridAct->setCheckable(true);
    showGridAct->setChecked(true);
    connect(showGridAct, SIGNAL(toggled(bool)), this, SLOT(gridToggled(bool)));
}

//
//
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showGridAct);
}

//
//
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
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
    propertyBrowser = new PropertyBrowser(propertyDock);
    propertyDock->setWidget(propertyBrowser);
    propertyDock->setObjectName("propertyDock");
    addDockWidget(Qt::LeftDockWidgetArea, propertyDock);
    viewMenu->addAction(propertyDock->toggleViewAction());

    QDockWidget *toolBoxDock = new QDockWidget(tr("Toolbox"), this);
    QWidget *toolBox = new QWidget(toolBoxDock);
    toolBoxLayout = new QVBoxLayout(toolBox);
    toolBox->setLayout(toolBoxLayout);
    toolBoxDock->setWidget(toolBox);
    toolBoxDock->setObjectName("toolBoxDock");
    addDockWidget(Qt::LeftDockWidgetArea, toolBoxDock);
    viewMenu->addAction(toolBoxDock->toggleViewAction());
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
    scene->setShowGrid(value);
}