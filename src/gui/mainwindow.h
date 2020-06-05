#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "animator.h"
#include <QMainWindow>
#include <QtWidgets/QLabel>
#include <QtGui/QKeyEvent>

namespace Ui {
class MainWindow;
}

class AnimationControl;
class SectorEditor;
class QDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void initializeAndShow();
    void saveSettings();
    void readSettings();

signals:
    void signalSyncKeyboardStates();
    void signalKeyPressEventInGraphicsView(QKeyEvent ev);
    void signalKeyReleaseEventInGraphicsView(QKeyEvent ev);

public slots:
    void zoomToFit();
    void syncKeyboardStates();
    void showStatusMsg(QString const &msg, int timeout = 0);
    void on_updateGraphicsScene();
    void on_updateGraphicsItemsFlags(bool inMapMode, bool inAnimatonMode);
    void on_sectorModeChanged(bool inSectorMode);

private slots:
    void on_actionSync_triggered();
    void on_actionConnect_triggered();
    void on_sectorColorChanged(QColor const &color);
    void on_mapColorChanged(QColor const &color);
    void on_tabWidgetMode_currentChanged(int index);
    void on_animationMatrixChanged(AnimationMatrix const &matrix);
    void on_animationStarted();
    void on_animationStopped();

    void on_mapSectorSelected(int sector);
    void on_mapSelectAll(bool select);
    void on_mapSelectInvert();

    void on_actionDisconnect_triggered();
    void on_actionConfigure_triggered();

    void on_actionSave_triggered();
    void on_actionLoad_triggered();

    void consoleDockWidgetVisibilityChanged(bool visible);

    void on_actionQuit_triggered();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void createScene();
    void setupChildWidgets();
    void setupKeyboardSettingsDialog();
    void setupInterfaceSettingsDialog();
    void setupConsoleWidget();
    void createStatusBar();

    Ui::MainWindow *ui;

    SectorEditor *se;
    AnimationControl *ac;

    QLabel *_statusBarVendor;
    QLabel *_statusBarKeyboard;

    QWidget *_keyboardConnectionSettingsWidget;
    QDockWidget *_consoleDockWidget = 0;
};

#endif // MAINWINDOW_H
