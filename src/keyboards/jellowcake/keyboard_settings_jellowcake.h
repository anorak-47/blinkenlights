#pragma once
#include "keyboard_interface.h"
#include <QtWidgets/QWidget>

namespace Ui
{
class KeyboardSettingsJellowcake;
}

class LedBrightness;

class KeyboardSettingsJellowcake : public QWidget
{
    Q_OBJECT

public:
    explicit KeyboardSettingsJellowcake(const s_device_info &deviceInfo, QWidget *parent = 0);
    virtual ~KeyboardSettingsJellowcake();

public slots:
    void syncKeyboardStates();

private slots:
    void on_pbDebugSync_clicked();
    void on_pbDebugSet_clicked();
    void on_pbDebugSave_clicked();

    void on_pbLayoutSync_clicked();
    void on_pbLayoutSet_clicked();
    void on_pbLayoutSave_clicked();

    void on_pbBootloader_clicked();

    void on_pbBLSync_clicked();
    void on_pbBLSet_clicked();
    void on_pbBLSave_clicked();

private:
    void keymapConfigSynced(unsigned int config);
    void debugConfigSynced(unsigned int config);
    void backlightSynced(bool enabled, unsigned int level);
    void backlightBrightnessSynced(unsigned int bri);
    void initWidgets();

    Ui::KeyboardSettingsJellowcake *ui;
    const s_device_info &_deviceInfo;

#pragma pack(1)
    typedef union
    {
        uint8_t raw;
        struct
        {
            bool swap_control_capslock : 1;
            bool capslock_to_control : 1;
            bool swap_lalt_lgui : 1;
            bool swap_ralt_rgui : 1;
            bool no_gui : 1;
            bool swap_grave_esc : 1;
            bool swap_backslash_backspace : 1;
            bool nkro : 1;
        };
    } keymap_config_t;

    typedef union
    {
        struct
        {
            bool enable : 1;
            bool matrix : 1;
            bool keyboard : 1;
            bool mouse : 1;
            uint8_t reserved : 4;
        };
        uint8_t raw;
    } debug_config_t;
#pragma pack()

    LedBrightness *capsLock;
    LedBrightness *scrollLock;
};
