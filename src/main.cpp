#include "mainwindow.h"
#include "keyboard_command_consumer.h"
#include "keyboard_command_queue.h"
#include "keyboardselectordialog.h"
#include "keyboard_factory.h"
#include <QApplication>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QTimer>

bool select_online_keyboard(bool autoselect = false)
{
    present_keyboard_list keyboardsFoundList;
    KeyboardFactory().probeForKeyboards(keyboardsFoundList);

    if (keyboardsFoundList.empty())
    {
        // no keyboard found
        return false;
    }

    if (autoselect && keyboardsFoundList.size() == 1 &&
        keyboardsFoundList.front().keyboards_found_list.size() == 1)
    {
        // only one keyboard found
        auto const &theOnlyKeyboard = keyboardsFoundList.front();
        KeyboardFactory().createKeyboard(
            theOnlyKeyboard.keyboard,
            theOnlyKeyboard.keyboards_found_list.front());
        return true;
    }

    KeyboardSelectorDialog dlg(keyboardsFoundList);
    int rc = dlg.exec();

    if (rc != QDialog::Accepted)
        return false;

    auto selectedKeyboard = dlg.getSelectedKeyboardDeviceInfo();
    auto keyboardType = dlg.getSelectedKeyboardType();

    KeyboardFactory().createKeyboard(keyboardType, selectedKeyboard);

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("anorak");
    QCoreApplication::setOrganizationDomain("altgr.ddnss.de");
    QCoreApplication::setApplicationName("blinkenlights");

    QSettings settings;
    bool autoselect = settings.value("global/autoselectkeyboard", true).toBool();
    bool autosync = settings.value("global/autosynctokeyboard", true).toBool();

    if (!select_online_keyboard(autoselect))
    {
        return -1;
    }

    KeyboardCommandConsumer consumer;
    consumer.start();

    MainWindow w;
    w.initializeAndShow();

    if (autosync)
    {
        QTimer::singleShot(100, &w, SLOT(syncKeyboardStates()));
    }

    int rc = a.exec();

    w.saveSettings();

    consumer.requestInterruption();
    KeyboardCommandQueueInstance().requestInterruption();
    consumer.wait();

    return rc;
}
