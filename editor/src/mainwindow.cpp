//
// mainwindow.cpp
//

#include "hasproperties.h"
#include "enumeditorcreator.h"
#include "mainwindow.h"
#include "mapdata.h"
#include "mapfile.h"
#include "mapscene.h"
#include "propertyitemmodel.h"
#include "editorwidgets/pointeditor.h"
#include "sceneitems/shapeitem.h"
#include "sceneitems/vertexitem.h"
#include <QtGui>


//
//
MainWindow::MainWindow()
{
    undoStack_ = new QUndoStack(this);
    errMsg_ = new QErrorMessage(this);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    view_ = new QGraphicsView(this);
    scene_ = new MapScene(view_, undoStack_);
    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));

    view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view_->setScene(scene_);

    view_->setTransformationAnchor(QGraphicsView::NoAnchor);
    view_->scale(32, -32); // One unit is 32x32 pixels at default zoom
    view_->translate(20, 11.5);
    view_->setMouseTracking(true);

    propertyItemModel_ = new PropertyItemModel(undoStack_);
    propertyBrowser_->setModel(propertyItemModel_);
    propertyBrowser_->horizontalHeader()->setStretchLastSection(true);

    // set up non-standard property editor widgets
    QItemEditorFactory *factory = new QItemEditorFactory;
    factory->registerEditor(QVariant::PointF, new QItemEditorCreator<PointEditor>("point"));
    factory->registerEditor(variantUserType<Body::BodyType>(), new EnumEditorCreator<Body, Body::BodyType>);

    QItemDelegate *delegate = new QItemDelegate;
    delegate->setItemEditorFactory(factory);
    propertyBrowser_->setItemDelegate(delegate);

    toolButtonGroup_ = new QButtonGroup(this);
    QAbstractButton *selectButton = createToolButton("Select");
    toolButtonGroup_->addButton(selectButton, kSelectButton);
    toolButtonGroup_->addButton(createToolButton("Polygon Shape"), kPolygonButton);
    toolButtonGroup_->addButton(createToolButton("Circle Shape"), kCircleButton);
    toolButtonGroup_->addButton(createToolButton("Physics Body"), kBodyButton);
    toolButtonGroup_->addButton(createToolButton("Body to Shape Fixture"), kFixtureButton);
    toolButtonGroup_->addButton(createToolButton("Distance Joint"), kDistanceJointButton);
    //toolButtonGroup_->addButton(createToolButton("Friction Joint"), kFrictionJointButton);
    //toolButtonGroup_->addButton(createToolButton("Gear Joint"), kGearJointButton);
    toolButtonGroup_->addButton(createToolButton("Prismatic Joint"), kPrismaticJointButton);
    toolButtonGroup_->addButton(createToolButton("Pulley Joint"), kPulleyJointButton);
    toolButtonGroup_->addButton(createToolButton("Revolute Joint"), kRevoluteJointButton);
    //toolButtonGroup_->addButton(createToolButton("Rope Joint"), kRopeJointButton);
    toolButtonGroup_->addButton(createToolButton("Weld Joint"), kWeldJointButton);
    //toolButtonGroup_->addButton(createToolButton("Wheel Joint"), kWheelJointButton);
    connect(toolButtonGroup_, SIGNAL(buttonClicked(int)), this, SLOT(toolButtonClicked(int)));
    selectButton->setChecked(true);

    toolBoxLayout_->addStretch();
    toolBoxLayout_->setSpacing(0);

    setCentralWidget(view_);
    setWindowTitle(tr("PGEditor"));
    setUnifiedTitleAndToolBarOnMac(true);

    resize(1280, 720);
    toolButtonClicked(0); // Select tool

    readSettings();
    newMap();
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
    newAct_ = new QAction(loadIcon("new"), tr("&New"), this);
    newAct_->setShortcuts(QKeySequence::New);
    newAct_->setStatusTip(tr("Start a new map"));
    connect(newAct_, SIGNAL(triggered()), this, SLOT(newMap()));

    openAct_ = new QAction(loadIcon("open"), tr("&Open"), this);
    openAct_->setShortcuts(QKeySequence::Open);
    openAct_->setStatusTip(tr("Read a map from disk"));
    connect(openAct_, SIGNAL(triggered()), this, SLOT(openMap()));

    saveAct_ = new QAction(loadIcon("save"), tr("&Save"), this);
    saveAct_->setShortcuts(QKeySequence::Save);
    saveAct_->setStatusTip(tr("Save this map"));
    connect(saveAct_, SIGNAL(triggered()), this, SLOT(saveMap()));

    saveAsAct_ = new QAction(tr("Save &As..."), this);
    saveAsAct_->setShortcuts(QKeySequence::SaveAs);
    saveAsAct_->setStatusTip(tr("Save a copy of this map"));
    connect(saveAsAct_, SIGNAL(triggered()), this, SLOT(saveMapAs()));

    quitAct_ = new QAction(tr("&Quit"), this);
    quitAct_->setShortcuts(QKeySequence::Quit);
    quitAct_->setStatusTip(tr("Quit the program"));
    connect(quitAct_, SIGNAL(triggered()), this, SLOT(quit()));

    showGridAct_ = new QAction(tr("Show &Grid"), this);
    showGridAct_->setShortcut(Qt::Key_G);
    showGridAct_->setStatusTip(tr("Show or hide the grid"));
    showGridAct_->setCheckable(true);
    showGridAct_->setChecked(true);
    connect(showGridAct_, SIGNAL(toggled(bool)), this, SLOT(gridToggled(bool)));

    snapToGridAct_ = new QAction(tr("S&nap to Grid"), this);
    snapToGridAct_->setStatusTip(tr("Snap points to the grid"));
    snapToGridAct_->setCheckable(true);
    snapToGridAct_->setChecked(true);
    connect(snapToGridAct_, SIGNAL(toggled(bool)), this, SLOT(snapToggled(bool)));

    undoAct_ = undoStack_->createUndoAction(this, tr("&Undo"));
    undoAct_->setShortcuts(QKeySequence::Undo);

    redoAct_ = undoStack_->createRedoAction(this, tr("&Redo"));
    redoAct_->setShortcuts(QKeySequence::Redo);

    resourceDirAct_ = new QAction(tr("Select &Resource Directory"), this);
    resourceDirAct_->setStatusTip(tr("Choose directory containing resources"));
    connect(resourceDirAct_, SIGNAL(triggered()), this, SLOT(setResourceDirectory()));

    selectMapAct_ = new QAction(tr("Select Map &Object"), this);
    selectMapAct_->setStatusTip(tr("Select the map object so its properties can be modified"));
    connect(selectMapAct_, SIGNAL(triggered()), this, SLOT(selectMap()));
}

//
//
void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(newAct_);
    fileMenu_->addAction(openAct_);
    fileMenu_->addAction(saveAct_);
    fileMenu_->addAction(saveAsAct_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(resourceDirAct_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(quitAct_);

    editMenu_ = menuBar()->addMenu(tr("&Edit"));
    editMenu_->addAction(undoAct_);
    editMenu_->addAction(redoAct_);
    editMenu_->addSeparator();
    editMenu_->addAction(selectMapAct_);

    viewMenu_ = menuBar()->addMenu(tr("&View"));
    viewMenu_->addAction(showGridAct_);
    viewMenu_->addAction(snapToGridAct_);
    viewMenu_->addSeparator();
}

//
//
void MainWindow::createToolBars()
{
    fileToolBar_ = addToolBar(tr("File"));
    fileToolBar_->addAction(newAct_);
    fileToolBar_->addAction(openAct_);
    fileToolBar_->addAction(saveAct_);
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
    propertyBrowser_ = new QTableView(propertyDock);
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
void MainWindow::snapToggled(bool value)
{
    scene_->setSnapToGrid(value);
}

//
//
void MainWindow::setResourceDirectory()
{
    QSettings settings;
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
            settings.value("resources/resourceDirectory").toString(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        settings.beginGroup("resources");
        settings.setValue("resourceDirectory", dir);
        settings.endGroup();
    }
}

//
//
void MainWindow::selectMap()
{
    if (map_) {
        propertyItemModel_->setSource(map_);
    }
}

//
//
void MainWindow::toolButtonClicked(int id)
{
    switch (id) {
        case kSelectButton:
            scene_->setMode(MapScene::kSelectMode);
            break;
        case kPolygonButton:
            scene_->setMode(MapScene::kPolygonMode);
            break;
        case kCircleButton:
            scene_->setMode(MapScene::kCircleMode);
            break;
        case kBodyButton:
            scene_->setMode(MapScene::kBodyMode);
            break;
        case kFixtureButton:
            scene_->setMode(MapScene::kFixtureMode);
            break;
        case kDistanceJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kDistanceJoint);
            break;
        case kFrictionJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kFrictionJoint);
            break;
        case kGearJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kGearJoint);
            break;
        case kPrismaticJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kPrismaticJoint);
            break;
        case kPulleyJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kPulleyJoint);
            break;
        case kRevoluteJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kRevoluteJoint);
            break;
        case kRopeJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kRopeJoint);
            break;
        case kWeldJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kWeldJoint);
            break;
        case kWheelJointButton:
            scene_->setMode(MapScene::kJointMode);
            scene_->setJointMode(MapScene::kWheelJoint);
            break;
    }
}

//
//
bool MainWindow::askSaveIfDirty()
{
    if (!undoStack_->isClean()) {
        QMessageBox msgBox;
        msgBox.setText("The map has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:
                if (!saveMap()) {
                    return false;
                }
                break;
            case QMessageBox::Cancel:
                return false;
                break;
            default:
                break;
        }
    }

    return true;
}

//
//
void MainWindow::writeMap(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        errMsg_->showMessage(QString("Failed to save file '%1'").arg(filename));
        return;
    }

    QTextStream writer(&file);
    writer << mapToJson(map_);
}

//
//
void MainWindow::readMap(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errMsg_->showMessage(QString("Failed to open file '%1'").arg(filename));
        return;
    }

    QTextStream reader(&file);
    QString json;
    reader >> json;

    map_ = jsonToMap(json);
    scene_->setMap(map_);
    scene_->sync();
}

//
//
bool MainWindow::newMap()
{
    if (!askSaveIfDirty()) {
        return false;
    }

    map_ = QSharedPointer<GameMap>(new GameMap);
    map_->width = (1280. / 32);
    map_->height = (720. / 32);
    scene_->setMap(map_);

    return true;
}

//
//
bool MainWindow::openMap()
{
    if (!askSaveIfDirty()) {
        return false;
    } else {
        QString filename = QFileDialog::getOpenFileName(this, tr("Open Map"), ".", tr("JSON Files (*.json)"));
        if (!filename.isEmpty()) {
            mapFilename_ = filename;
            readMap(filename);
            undoStack_->clear();
            return true;
        }
        return false;
    }
}

//
//
bool MainWindow::saveMap()
{
    if (mapFilename_.isEmpty()) {
        return saveMapAs();
    } else {
        writeMap(mapFilename_);
        undoStack_->setClean();
        return true;
    }
}

//
//
bool MainWindow::saveMapAs()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Map"), "untitled.json", tr("JSON Files (*.json)"));
    if (!filename.isEmpty()) {
        mapFilename_ = filename;
        writeMap(filename);
        undoStack_->setClean();
        return true;
    }
    return false;
}

//
//
bool MainWindow::quit()
{
    if (!askSaveIfDirty()) {
        return false;
    }
    close();
    return true;
}

//
//
void MainWindow::selectionChanged()
{
    if (scene_->selectedItems().isEmpty() || scene_->selectedItems().size() > 1) {
        propertyItemModel_->setSource(QSharedPointer<HasProperties>());
    } else {
        SceneItem *sceneItem = dynamic_cast<SceneItem *>(scene_->selectedItems().takeFirst());
        VertexItem *vertexItem = dynamic_cast<VertexItem *>(sceneItem);
        if (vertexItem) {
            propertyItemModel_->setSource(vertexItem->parentEntity());
        } else if (sceneItem) {
            propertyItemModel_->setSource(sceneItem->entity());
        }
    }
}
