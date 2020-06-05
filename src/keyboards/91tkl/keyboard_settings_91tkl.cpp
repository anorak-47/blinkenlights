#include "ui_keyboardsettings91tkl.h"
#include "common.h"
#include "keyboard_settings_91tkl.h"
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

KeyboardSettings91tkl::KeyboardSettings91tkl(const s_device_info &deviceInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyboardSettings91tkl),
    _deviceInfo(deviceInfo)
{
    ui->setupUi(this);

    initWidgets();
}

KeyboardSettings91tkl::~KeyboardSettings91tkl()
{
    delete ui;
}

void KeyboardSettings91tkl::syncKeyboardStates()
{
    on_pbDebugSync_clicked();
    on_pbBLSync_clicked();
    on_pbLayoutSync_clicked();
    on_pbLedPwmSync_clicked();
}

void KeyboardSettings91tkl::initWidgets()
{
    ui->glLedPwm->removeWidget(ui->pbLedPwmSave);
    ui->glLedPwm->removeWidget(ui->pbLedPwmSet);
    ui->glLedPwm->removeWidget(ui->pbLedPwmSync);
    ui->glLedPwm->removeWidget(ui->cbLedPwmSync);

    capsLock = new LedBrightness(_("Caps Lock LED"));
    connect(capsLock, SIGNAL(signalBrightnessChanged(uint)), this, SLOT(on_CapsLockBrightnessChanged(uint)));
    connect(capsLock, SIGNAL(signalIlluminationChanged(bool)), this, SLOT(on_CapsLockIlluminationChanged(bool)));
    ui->glLedPwm->addWidget(capsLock, 0, 0, 1, 4);

    scrollLock = new LedBrightness(_("Scroll Lock LED"));
    connect(scrollLock, SIGNAL(signalBrightnessChanged(uint)), this, SLOT(on_ScrollLockBrightnessChanged(uint)));
    connect(scrollLock, SIGNAL(signalIlluminationChanged(bool)), this, SLOT(on_ScrollLockIlluminationChanged(bool)));
    ui->glLedPwm->addWidget(scrollLock, 1, 0, 1, 4);

    ui->glLedPwm->addWidget(ui->cbLedPwmSync, 2, 0);
    ui->glLedPwm->addWidget(ui->pbLedPwmSync, 2, 1);
    ui->glLedPwm->addWidget(ui->pbLedPwmSet, 2, 2);
    ui->glLedPwm->addWidget(ui->pbLedPwmSave, 2, 3);

    ui->lManufacturer->setText(_deviceInfo.manufacturer);
    ui->lProduct->setText(_deviceInfo.product);
    ui->lDescription->setText(_deviceInfo.description);
    ui->lVersion->setText(_deviceInfo.version);
    ui->lBuild->setText(_deviceInfo.build);
    ui->lPID->setText(_deviceInfo.pid);
    ui->lVID->setText(_deviceInfo.vid);
    ui->lSerial->setText(_deviceInfo.serialnumber);
}

void KeyboardSettings91tkl::debugConfigSynced(unsigned int config)
{
    debug_config_t dc;
    dc.raw = config;

    ui->cb_debug_enable->setChecked(dc.enable);
    ui->cb_debug_matrix->setChecked(dc.matrix);
    ui->cb_debug_keyboard->setChecked(dc.keyboard);
    ui->cb_debug_mouse->setChecked(dc.mouse);
}

void KeyboardSettings91tkl::on_pbDebugSync_clicked()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new DebugConfigGetCommand(std::bind(&KeyboardSettings91tkl::debugConfigSynced, this, _1)))));
}

void KeyboardSettings91tkl::on_pbDebugSet_clicked()
{
    debug_config_t dc;

    dc.enable = ui->cb_debug_enable->isChecked();
    dc.matrix = ui->cb_debug_matrix->isChecked();
    dc.keyboard = ui->cb_debug_keyboard->isChecked();
    dc.mouse = ui->cb_debug_mouse->isChecked();

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new DebugConfigSetCommand(dc.raw))));
}

void KeyboardSettings91tkl::on_pbDebugSave_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new DebugConfigSaveCommand())));
}

void KeyboardSettings91tkl::keymapConfigSynced(unsigned int config)
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

void KeyboardSettings91tkl::on_pbLayoutSync_clicked()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new KeymapConfigGetCommand(std::bind(&KeyboardSettings91tkl::keymapConfigSynced, this, _1)))));
}

void KeyboardSettings91tkl::on_pbLayoutSet_clicked()
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

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new KeymapConfigSetCommand(kc.raw))));
}

void KeyboardSettings91tkl::on_pbLayoutSave_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new KeymapConfigSaveCommand())));
}

void KeyboardSettings91tkl::ledBrightnessSynced(unsigned int led, unsigned int pwm)
{
    if (led == 0)
        capsLock->setBrightness(pwm);
    else
        scrollLock->setBrightness(pwm);
}

void KeyboardSettings91tkl::backlightSynced(bool enabled, unsigned int level)
{
    Q_UNUSED(level);
    QSignalBlocker bl(ui->cbBacklight);
    ui->cbBacklight->setChecked(enabled);
}

void KeyboardSettings91tkl::backlightBrightnessSynced(unsigned int brightness)
{
    QSignalBlocker bl(ui->sbGlobalBrightness);
    ui->sbGlobalBrightness->setValue(brightness);
}

void KeyboardSettings91tkl::on_pbLedPwmSync_clicked()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    auto getPwm0 = std::unique_ptr<KeyboardCommand>(new LedGetPwmCommand(0, std::bind(&KeyboardSettings91tkl::ledBrightnessSynced, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(getPwm0));
    auto getPwm1 = std::unique_ptr<KeyboardCommand>(new LedGetPwmCommand(1, std::bind(&KeyboardSettings91tkl::ledBrightnessSynced, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(getPwm1));
}

void KeyboardSettings91tkl::on_pbLedPwmSet_clicked()
{
    unsigned int brightness = capsLock->brightness();
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedBrightnessCommand(0, brightness))));
    brightness = scrollLock->brightness();
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedBrightnessCommand(1, brightness))));

    bool illuminated = capsLock->isChecked();
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedIlluminateCommand(0, illuminated))));
    illuminated = scrollLock->isChecked();
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedIlluminateCommand(1, illuminated))));
}

void KeyboardSettings91tkl::on_pbLedPwmSave_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedSaveCommand())));
}

void KeyboardSettings91tkl::on_pbSleepLedSleep_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SleepLedCommand(1))));
}

void KeyboardSettings91tkl::on_pbSleepLedStop_clicked()
{
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new SleepLedCommand(0))));
}

void KeyboardSettings91tkl::on_cbLedPwmSync_toggled(bool checked)
{
    ui->pbLedPwmSet->setDisabled(checked);
}

void KeyboardSettings91tkl::on_CapsLockBrightnessChanged(unsigned int brightness)
{
    if (!ui->cbLedPwmSync->isChecked())
        return;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedBrightnessCommand(0, brightness))));
}

void KeyboardSettings91tkl::on_ScrollLockBrightnessChanged(unsigned int brightness)
{
    if (!ui->cbLedPwmSync->isChecked())
        return;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedBrightnessCommand(1, brightness))));
}

void KeyboardSettings91tkl::on_CapsLockIlluminationChanged(bool illuminated)
{
    if (!ui->cbLedPwmSync->isChecked())
        return;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedIlluminateCommand(0, illuminated))));
}

void KeyboardSettings91tkl::on_ScrollLockIlluminationChanged(bool illuminated)
{
    if (!ui->cbLedPwmSync->isChecked())
        return;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new LedIlluminateCommand(1, illuminated))));
}

void KeyboardSettings91tkl::on_pbBootloader_clicked()
{
    int rc = QMessageBox::question(this, _("Start into Bootloader"), _("Really?"),
                                   QMessageBox::StandardButton::Ok | QMessageBox::StandardButton::Cancel,
                                   QMessageBox::StandardButton::Cancel);

    if (rc != QMessageBox::StandardButton::Ok)
        return;

    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new BootloaderStartCommand())));
}

void KeyboardSettings91tkl::on_pbBLSync_clicked()
{
    using namespace std::placeholders;  // for _1, _2, _3...
    auto getBlState = std::unique_ptr<KeyboardCommand>(new BlGetCommand(std::bind(&KeyboardSettings91tkl::backlightSynced, this, _1, _2)));
    KeyboardCommandQueueInstance().enqueue(std::move(getBlState));
    auto getBlBrigtness = std::unique_ptr<KeyboardCommand>(new BlGetBrightnessCommand(std::bind(&KeyboardSettings91tkl::backlightBrightnessSynced, this, _1)));
    KeyboardCommandQueueInstance().enqueue(std::move(getBlBrigtness));
}

void KeyboardSettings91tkl::on_pbBLSet_clicked()
{
    qDebug() << __PRETTY_FUNCTION__;
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new BlSetCommand(ui->cbBacklight->isChecked(), 7))));
    KeyboardCommandQueueInstance().enqueue(std::move(std::unique_ptr<KeyboardCommand>(new BlSetBrightnessCommand(ui->sbGlobalBrightness->value()))));
}

void KeyboardSettings91tkl::on_pbBLSave_clicked()
{

}
