#include "ui_keyboardsettings_jellowcake.h"
#include "common.h"
#include "keyboard_settings_jellowcake.h"
#include "ledbrightness.h"
#include "keyboard_command_queue.h"
#include "ledbrightnesscommand.h"
#include "ledilluminatecommand.h"
#include "ledgetpwmcommand.h"
#include "ledsavecommand.h"
#include "keymapconfigsetcommand.h"
#include "keymapconfiggetcommand.h"
#include "keymapconfigsavecommand.h"
#include "debugconfigsetcommand.h"
#include "debugconfiggetcommand.h"
#include "debugconfigsavecommand.h"
#include "sleepledcommand.h"
#include "bootloaderstartcommand.h"
#include "blgetcommand.h"
#include "blsetcommand.h"
#include "blgetbrightnesscommand.h"
#include "blsetbrightnesscommand.h"
#include <QtWidgets/QMessageBox>
#include <QtCore/QDebug>

KeyboardSettingsJellowcake::KeyboardSettingsJellowcake(const s_device_info &deviceInfo, QWidget *parent)
    : QWidget(parent), ui(new Ui::KeyboardSettingsJellowcake), _deviceInfo(deviceInfo)
{
    ui->setupUi(this);

    initWidgets();
}

KeyboardSettingsJellowcake::~KeyboardSettingsJellowcake()
{
    delete ui;
}

void KeyboardSettingsJellowcake::syncKeyboardStates()
{
    on_pbDebugSync_clicked();
    on_pbBLSync_clicked();
    on_pbLayoutSync_clicked();
}

void KeyboardSettingsJellowcake::initWidgets()
{
    ui->lManufacturer->setText(_deviceInfo.manufacturer);
    ui->lProduct->setText(_deviceInfo.product);
    ui->lDescription->setText(_deviceInfo.description);
    ui->lVersion->setText(_deviceInfo.version);
    ui->lBuild->setText(_deviceInfo.build);
    ui->lPID->setText(_deviceInfo.pid);
    ui->lVID->setText(_deviceInfo.vid);
    ui->lSerial->setText(_deviceInfo.serialnumber);
}

void KeyboardSettingsJellowcake::debugConfigSynced(unsigned int config)
{
    debug_config_t dc;
    dc.raw = config;

    ui->cb_debug_enable->setChecked(dc.enable);
    ui->cb_debug_matrix->setChecked(dc.matrix);
    ui->cb_debug_keyboard->setChecked(dc.keyboard);
    ui->cb_debug_mouse->setChecked(dc.mouse);
}

void KeyboardSettingsJellowcake::on_pbDebugSync_clicked()
{
    using namespace std::placeholders; // for _1, _2, _3...
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(
        new DebugConfigGetCommand(std::bind(&KeyboardSettingsJellowcake::debugConfigSynced, this, _1)))));
}

void KeyboardSettingsJellowcake::on_pbDebugSet_clicked()
{
    debug_config_t dc;

    dc.enable = ui->cb_debug_enable->isChecked();
    dc.matrix = ui->cb_debug_matrix->isChecked();
    dc.keyboard = ui->cb_debug_keyboard->isChecked();
    dc.mouse = ui->cb_debug_mouse->isChecked();

    KeyboardCommandQueueInstance().enqueue(
        std::move(std::unique_ptr<KeyboardCommand>(new DebugConfigSetCommand(dc.raw))));
}

void KeyboardSettingsJellowcake::on_pbDebugSave_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new DebugConfigSaveCommand())));
}

void KeyboardSettingsJellowcake::keymapConfigSynced(unsigned int config)
{
    keymap_config_t kc;
    kc.raw = config;

    ui->cb_swap_control_capslock->setChecked(kc.swap_control_capslock);
    ui->cb_capslock_to_control->setChecked(kc.capslock_to_control);
    ui->cb_swap_lalt_lgui->setChecked(kc.swap_lalt_lgui);
    ui->cb_swap_ralt_rgui->setChecked(kc.swap_ralt_rgui);
    ui->cb_no_gui->setChecked(kc.no_gui);
    ui->cb_swaap_grave_esc->setChecked(kc.swap_grave_esc);
    ui->cb_swap_backslash_backspace->setChecked(kc.swap_backslash_backspace);
    ui->cb_nkro->setChecked(kc.nkro);
}

void KeyboardSettingsJellowcake::on_pbLayoutSync_clicked()
{
    using namespace std::placeholders; // for _1, _2, _3...
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(
        new KeymapConfigGetCommand(std::bind(&KeyboardSettingsJellowcake::keymapConfigSynced, this, _1)))));
}

void KeyboardSettingsJellowcake::on_pbLayoutSet_clicked()
{
    keymap_config_t kc;

    kc.swap_control_capslock = ui->cb_swap_control_capslock->isChecked();
    kc.capslock_to_control = ui->cb_capslock_to_control->isChecked();
    kc.swap_lalt_lgui = ui->cb_swap_lalt_lgui->isChecked();
    kc.swap_ralt_rgui = ui->cb_swap_ralt_rgui->isChecked();
    kc.no_gui = ui->cb_no_gui->isChecked();
    kc.swap_grave_esc = ui->cb_swaap_grave_esc->isChecked();
    kc.swap_backslash_backspace = ui->cb_swap_backslash_backspace->isChecked();
    kc.nkro = ui->cb_nkro->isChecked();

    KeyboardCommandQueueInstance().enqueue(
        std::move(std::unique_ptr<KeyboardCommand>(new KeymapConfigSetCommand(kc.raw))));
}

void KeyboardSettingsJellowcake::on_pbLayoutSave_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new KeymapConfigSaveCommand())));
}

void KeyboardSettingsJellowcake::backlightSynced(bool enabled, unsigned int level)
{
    Q_UNUSED(level);
    QSignalBlocker bl(ui->cbBacklight);
    ui->cbBacklight->setChecked(enabled);
}

void KeyboardSettingsJellowcake::backlightBrightnessSynced(unsigned int brightness)
{
    QSignalBlocker bl(ui->sbGlobalBrightness);
    ui->sbGlobalBrightness->setValue(brightness);
}

void KeyboardSettingsJellowcake::on_pbBLSync_clicked()
{
    using namespace std::placeholders; // for _1, _2, _3...
    auto getBlState = std::unique_ptr<KeyboardCommand>(
        new BlGetCommand(std::bind(&KeyboardSettingsJellowcake::backlightSynced, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(getBlState));
    auto getBlBrigtness = std::unique_ptr<KeyboardCommand>(
        new BlGetBrightnessCommand(std::bind(&KeyboardSettingsJellowcake::backlightBrightnessSynced, this, _1)));
    KeyboardCommandQueueInstance().enqueue(std::move(getBlBrigtness));
}

void KeyboardSettingsJellowcake::on_pbBLSet_clicked()
{
    qDebug() << __PRETTY_FUNCTION__;
    KeyboardCommandQueueInstance().enqueue(
        std::move(std::unique_ptr<KeyboardCommand>(new BlSetCommand(ui->cbBacklight->isChecked(), 7))));
    KeyboardCommandQueueInstance().enqueue(
        std::move(std::unique_ptr<KeyboardCommand>(new BlSetBrightnessCommand(ui->sbGlobalBrightness->value()))));
}

void KeyboardSettingsJellowcake::on_pbBLSave_clicked()
{
}

void KeyboardSettingsJellowcake::on_pbBootloader_clicked()
{
    int rc = QMessageBox::question(this, _("Start into Bootloader"), _("Really?"),
                                   QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel,
                                   QMessageBox::StandardButton::Cancel);

    if (rc != QMessageBox::StandardButton::Ok)
        return;

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new BootloaderStartCommand())));
}
