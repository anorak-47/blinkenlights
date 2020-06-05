#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sectoreditor.h"
#include "keycapcoloreditor.h"
#include "animationcontrol.h"
#include "keyboard_properties.h"
#include "keygraphicsitem.h"
#include "common.h"
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDockWidget>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtCore/QTimer>
#include <QtCore/QDebug>

const int SCENE_MARGIN = 15;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _keyboardConnectionSettingsWidget = 0;

    createScene();
    setupChildWidgets();
    setupInterfaceSettingsDialog();
    setupKeyboardSettingsDialog();
    setupConsoleWidget();
    createStatusBar();

    showStatusMsg(_("Initializing..."));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupInterfaceSettingsDialog()
{
    auto properties = KeyboardPropertiesInstance().properties();
    QWidget *keyboardConnectionSettingsWidget = properties->interfaceSettingsDialog();
    if (keyboardConnectionSettingsWidget)
    {
        _keyboardConnectionSettingsWidget = keyboardConnectionSettingsWidget;
    }
}

void MainWindow::setupKeyboardSettingsDialog()
{
    auto properties = KeyboardPropertiesInstance().properties();
    QWidget *keyboardSettingsWidget = properties->keyboardSettingsWidget();
    if (keyboardSettingsWidget)
    {
        ui->tabWidgetKeyboard->addTab(keyboardSettingsWidget, _("Settings"));
        connect(this, SIGNAL(signalSyncKeyboardStates()), keyboardSettingsWidget, SLOT(syncKeyboardStates()));
    }
}

void MainWindow::setupConsoleWidget()
{
    auto properties = KeyboardPropertiesInstance().properties();
    QWidget *consoleWidget = properties->consoleWidget();

    if (consoleWidget)
    {
        //ui->tabWidgetKeyboard->addTab(consoleWidget, _("Console"));

        auto properties = KeyboardPropertiesInstance().properties();
        QSettings settings;

        //_consoleDockWidget = new QDockWidget(_("blinkenlights - %1 console").arg(properties->name()), this);
        _consoleDockWidget = new QDockWidget(_("%1 console").arg(properties->name()), this);
        _consoleDockWidget->setObjectName(properties->name() + "/console");
        _consoleDockWidget->setWidget(consoleWidget);

        QString areasettings = properties->name() + "/console/dockarea";
        Qt::DockWidgetArea area = static_cast<Qt::DockWidgetArea>(settings.value(areasettings, Qt::BottomDockWidgetArea).toInt());
        addDockWidget(area, _consoleDockWidget);

        bool floating = settings.value(properties->name() + "/console/dockfloating", false).toBool();
        _consoleDockWidget->setFloating(floating);

        _consoleDockWidget->restoreGeometry(settings.value(properties->name() + "/console/geometry").toByteArray());
        _consoleDockWidget->setVisible(settings.value(properties->name() + "/console/dockvisible", true).toBool());

        QAction *tvAction = _consoleDockWidget->toggleViewAction();
        //tvAction->setText(_("%1 console").arg(properties->name()));
        //tvAction->setIconText(_("%1 console").arg(properties->name()));
        ui->menuOptions->addAction(tvAction);

        connect(_consoleDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(consoleDockWidgetVisibilityChanged(bool)));
    }
}

void MainWindow::createStatusBar()
{
    auto properties = KeyboardPropertiesInstance().properties();

    _statusBarVendor = new QLabel(properties->vendor());
    statusBar()->addPermanentWidget(_statusBarVendor);

    _statusBarKeyboard = new QLabel(properties->name());
    statusBar()->addPermanentWidget(_statusBarKeyboard);
}

void MainWindow::setupChildWidgets()
{
    auto properties = KeyboardPropertiesInstance().properties();

    if (properties->supportsColorMaps() || properties->supportsSectors())
    {
        se = new SectorEditor(ui->tabWidgetMode);
        connect(se, SIGNAL(signalSectorColorChanged(QColor)), this, SLOT(on_sectorColorChanged(QColor)));
        connect(se, SIGNAL(signalMapColorChanged(QColor)), this, SLOT(on_mapColorChanged(QColor)));
        connect(se, SIGNAL(signalGraphicsSceneChanged()), this, SLOT(on_updateGraphicsScene()));
        connect(se, SIGNAL(signalSectorModeChanged(bool)), this, SLOT(on_sectorModeChanged(bool)));
        connect(se, SIGNAL(signalMapSectorSelected(int)), this, SLOT(on_mapSectorSelected(int)));
        connect(se, SIGNAL(signalMapSelectAll(bool)), this, SLOT(on_mapSelectAll(bool)));
        connect(se, SIGNAL(signalMapSelectInvert()), this, SLOT(on_mapSelectInvert()));
        connect(this, SIGNAL(signalSyncKeyboardStates()), se, SLOT(on_syncKeyboardStates()));
        ui->tabWidgetMode->addTab(se, _("Key Colors"));
    }

    if (properties->supportsAnimations())
    {
        ac = new AnimationControl(ui->tabWidgetMode);
        connect(this, SIGNAL(signalSyncKeyboardStates()), ac, SLOT(on_syncAnimationState()));
        ui->tabWidgetMode->addTab(ac, _("Animations"));

        std::shared_ptr<Animator> animator = KeyboardPropertiesInstance().properties()->animator();
        if (animator)
        {
            connect(animator.get(), SIGNAL(signalChanged(AnimationMatrix)), this, SLOT(on_animationMatrixChanged(AnimationMatrix)));
            connect(animator.get(), SIGNAL(signalStarted()), this, SLOT(on_animationStarted()));
            connect(animator.get(), SIGNAL(signalStopped()), this, SLOT(on_animationStopped()));
            connect(this, SIGNAL(signalKeyPressEventInGraphicsView(QKeyEvent)), animator.get(), SLOT(on_KeyPressEventInGraphicsView(QKeyEvent)));
            connect(this, SIGNAL(signalKeyReleaseEventInGraphicsView(QKeyEvent)), animator.get(), SLOT(on_KeyReleaseEventInGraphicsView(QKeyEvent)));
        }
    }
}

void MainWindow::createScene()
{
    QGraphicsScene *scene = new QGraphicsScene();

    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();

    for (auto &key : klekeys)
    {
        SimpleKeyGraphicsItem *item = new SimpleKeyGraphicsItem(key);
        item->setPos(QPointF(key->x * 100.0, key->y * 100.0));
        scene->addItem(item);
    }

    ui->graphicsView->setScene(scene);
    ui->graphicsView->installEventFilter(this);
    ui->graphicsView->show();
}

void MainWindow::syncKeyboardStates()
{
    emit signalSyncKeyboardStates();
}

void MainWindow::initializeAndShow()
{
    show();    

    readSettings();
    zoomToFit();

    showStatusMsg(_("Ready"));
}

void MainWindow::zoomToFit()
{
    qDebug() << __PRETTY_FUNCTION__;
    ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect().adjusted(-SCENE_MARGIN,-SCENE_MARGIN,SCENE_MARGIN,SCENE_MARGIN), Qt::KeepAspectRatio);
}

void MainWindow::showStatusMsg(const QString &msg, int timeout)
{
    statusBar()->showMessage(msg, timeout);
}

void MainWindow::on_actionSync_triggered()
{
    syncKeyboardStates();
}

void MainWindow::on_actionConnect_triggered()
{
}

void MainWindow::on_actionDisconnect_triggered()
{

}

void MainWindow::on_actionConfigure_triggered()
{
    if (_keyboardConnectionSettingsWidget)
    {
        _keyboardConnectionSettingsWidget->show();
    }
}

void MainWindow::on_updateGraphicsScene()
{
    //qDebug() << __PRETTY_FUNCTION__;
    ui->graphicsView->scene()->update(ui->graphicsView->scene()->sceneRect());
    ui->graphicsView->update();
}

void MainWindow::on_sectorColorChanged(const QColor &color)
{
    Q_UNUSED(color);
    on_updateGraphicsScene();
}

void MainWindow::on_mapColorChanged(const QColor &color)
{
    auto items = ui->graphicsView->scene()->selectedItems();
    foreach (QGraphicsItem *item, items)
    {
        SimpleKeyGraphicsItem *sitem = dynamic_cast<SimpleKeyGraphicsItem*>(item);
        if (sitem)
            sitem->setKeyColor(color);
    }

    on_updateGraphicsScene();
}

void MainWindow::consoleDockWidgetVisibilityChanged(bool visible)
{
    Q_UNUSED(visible)
    QTimer::singleShot(10, this, SLOT(zoomToFit()));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    //zoomToFit();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == ui->graphicsView)
    {
        if (ev->type() == QEvent::Resize)
        {
            zoomToFit();
            return false;
        }
        else if (ev->type() == QEvent::KeyPress)
        {
            QKeyEvent *kev = dynamic_cast<QKeyEvent*>(ev);
            if (kev)
            {
                QKeyEvent skev = *kev;
                emit signalKeyPressEventInGraphicsView(skev);
                return false;
            }
        }
        else if (ev->type() == QEvent::KeyRelease)
        {
            QKeyEvent *kev = dynamic_cast<QKeyEvent*>(ev);
            if (kev)
            {
                QKeyEvent skev = *kev;
                emit signalKeyReleaseEventInGraphicsView(skev);
                return false;
            }
        }
        return false;
    }
    else
    {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, ev);
    }
}

void MainWindow::on_updateGraphicsItemsFlags(bool inMapMode, bool inAnimatonMode)
{
    qDebug() << __PRETTY_FUNCTION__ << " inMapMode: " << inMapMode;
    qDebug() << __PRETTY_FUNCTION__ << " inAnimatonMode: " << inAnimatonMode;

    auto items = ui->graphicsView->scene()->items();
    foreach (QGraphicsItem *item, items)
    {
        item->setAcceptHoverEvents(inMapMode && !inAnimatonMode);

        SimpleKeyGraphicsItem *sitem = dynamic_cast<SimpleKeyGraphicsItem*>(item);
        if (sitem)
        {
            sitem->setAdaptForegroundColor(!inAnimatonMode);
            sitem->setShowRowColumn(!inAnimatonMode);
        }
    }
}

void MainWindow::on_sectorModeChanged(bool inSectorMode)
{
    on_updateGraphicsItemsFlags(!inSectorMode, false);

    if (inSectorMode)
    {
        auto items = ui->graphicsView->scene()->items();
        foreach (QGraphicsItem *item, items)
        {
            item->setSelected(false);
            SimpleKeyGraphicsItem *sitem = dynamic_cast<SimpleKeyGraphicsItem*>(item);
            if (sitem)
                sitem->setSelectable(false);
        }
    }
    else
    {
        auto items = ui->graphicsView->scene()->items();
        foreach (QGraphicsItem *item, items)
        {
            SimpleKeyGraphicsItem *sitem = dynamic_cast<SimpleKeyGraphicsItem*>(item);
            if (sitem)
                sitem->setSelectable(true);
        }
    }
}

void MainWindow::on_tabWidgetMode_currentChanged(int index)
{
    qDebug() << __PRETTY_FUNCTION__ << " index: " << index;
    on_updateGraphicsItemsFlags(!se->inSectorMode(), index == 1);
}

void MainWindow::on_animationMatrixChanged(const AnimationMatrix &matrix)
{
    //qDebug() << __PRETTY_FUNCTION__;
    auto klekeys = KeyboardPropertiesInstance().properties()->getKeys();
    for (auto &key : klekeys)
        key->color = matrix[key->row][key->col];

    on_updateGraphicsScene();
}

void MainWindow::on_animationStarted()
{
    auto items = ui->graphicsView->scene()->items();
    foreach (QGraphicsItem *item, items)
    {
        item->setSelected(false);
        item->setAcceptHoverEvents(false);

        SimpleKeyGraphicsItem *sitem = dynamic_cast<SimpleKeyGraphicsItem*>(item);
        if (sitem)
        {
            sitem->setAdaptForegroundColor(false);
            sitem->setShowRowColumn(false);
        }
    }
}

void MainWindow::on_animationStopped()
{

}

void MainWindow::on_mapSectorSelected(int sector)
{
    //auto items = ui->graphicsView->scene()->items();
    auto klekeys = KeyboardPropertiesInstance().properties()->getSectorKeys(sector);
    for (auto &key : klekeys)
        key->item->setSelected(!key->item->isSelected());
}

void MainWindow::on_mapSelectAll(bool select)
{
    auto items = ui->graphicsView->scene()->items();
    foreach (QGraphicsItem *item, items)
        item->setSelected(select);
}

void MainWindow::on_mapSelectInvert()
{
    auto items = ui->graphicsView->scene()->items();
    foreach (QGraphicsItem *item, items)
        item->setSelected(!item->isSelected());
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());

    /*
    settings.setValue("MainWindow/splitter", ui->splitter->saveGeometry());

    int c = 0;
    for(int size : ui->splitter->sizes())
    {
        settings.setValue(QString("MainWindow/splitter-size-%1").arg(c++), size);
    }
    */

    if (_consoleDockWidget)
    {
        auto properties = KeyboardPropertiesInstance().properties();
        settings.setValue(properties->name() + "/console/dockarea", dockWidgetArea(_consoleDockWidget));
        settings.setValue(properties->name() + "/console/dockfloating", _consoleDockWidget->isFloating());
        settings.setValue(properties->name() + "/console/dockvisible", _consoleDockWidget->isVisible());
        settings.setValue(properties->name() + "/console/geometry", _consoleDockWidget->saveGeometry());
    }
}

void MainWindow::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    //ui->splitter->restoreGeometry(settings.value("MainWindow/splitter").toByteArray());
    //restoreSplitter();
}

void MainWindow::on_actionSave_triggered()
{
    QString appDataLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    qDebug() << appDataLocation;

    if (!QDir(appDataLocation).exists())
    {
        qDebug() << "mkpath";
        QDir().mkpath(appDataLocation);
    }

    QString defaultSaveFileName = QString("%1/colormap_%2.json").arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(se->getCurrentColorMapIndex());

    QString saveFileName = QFileDialog::getSaveFileName(this, _("Save Color Map"), defaultSaveFileName, _("JSON formated file (*.json)"));

    if (saveFileName.isEmpty())
        return;

    if (QFile::exists(saveFileName))
    {
        //TODO: overwrite?
    }

    QFile saveFile(saveFileName);

    if (!saveFile.open(QIODevice::WriteOnly))
    {
        qWarning("Couldn't open save file.");
        return;
    }

    QString colormap = se->getColorMapAsJSONString();
    saveFile.write(colormap.toUtf8());
    saveFile.close();
}

void MainWindow::on_actionLoad_triggered()
{
    QString loadFileName = QFileDialog::getOpenFileName(this, _("Load Color Map"),
                                                        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation),
                                                        _("JSON (*.json)"));

    if (loadFileName.isEmpty())
        return;

    if (!QFile::exists(loadFileName))
    {
        return;
    }

    QFile loadFile(loadFileName);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open file.");
        return;
    }

    QString jsonstring = loadFile.readAll();

    se->setColorMapFromJSONString(jsonstring);

    loadFile.close();
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}
