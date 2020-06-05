#pragma once

#include "keyboard_interface.h"

enum class SupportedKeyboards
{
    anorak_91tkl,
    anorak_jellowcake
};

struct s_present_keyboard_info
{
    s_present_keyboard_info(SupportedKeyboards k, device_info_list l) : keyboard(k), keyboards_found_list(l)
    {
    }
    SupportedKeyboards keyboard;
    device_info_list keyboards_found_list;
};

typedef std::vector<struct s_present_keyboard_info> present_keyboard_list;

class AbstractKeyboardFactory
{
public:
    virtual ~AbstractKeyboardFactory()
    {
    }
    virtual void createKeyboard(struct s_device_info const &deviceInfo) const = 0;
    virtual void createKeyboardProperties(SupportedKeyboards keyboardtype, const s_device_info &deviceInfo) const = 0;
    virtual void probeForKeyboards(SupportedKeyboards keyboardtype) const = 0;
    virtual void probeForKeyboards(present_keyboard_list &keyboardList) const = 0;
};

class KeyboardFactory
{
public:
    virtual ~KeyboardFactory()
    {
    }
    virtual void createKeyboard(SupportedKeyboards keyboardtype, struct s_device_info const &deviceInfo) const final;
    virtual void createKeyboardProperties(SupportedKeyboards keyboardtype, const s_device_info &deviceInfo) const final;
    virtual void probeForKeyboards(SupportedKeyboards keyboardtype, present_keyboard_list &keyboardList) const final;
    virtual void probeForKeyboards(present_keyboard_list &keyboardList) const final;
};

QString getKeyboardName(SupportedKeyboards keyboard);
