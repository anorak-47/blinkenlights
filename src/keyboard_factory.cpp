#include "keyboard_interface.h"
#include <QtCore/QDebug>
#include "keyboard_factory.h"
#include "keyboard_protocol_interface.h"
#include "keyboards/91tkl/animator_91tkl.h"
#include "keyboards/91tkl/keyboard_interface_91tkl.h"
#include "keyboards/91tkl/keyboard_properties_91tkl.h"
#include "keyboards/jellowcake/keyboard_interface_jellowcake.h"
#include "keyboards/jellowcake/keyboard_properties_jellowcake.h"

void KeyboardFactory::createKeyboard(SupportedKeyboards keyboardtype, const s_device_info &deviceInfo) const
{
    createKeyboardProperties(keyboardtype, deviceInfo);

    switch (keyboardtype)
    {
    case SupportedKeyboards::anorak_91tkl:
    {
        std::shared_ptr<Animator> animator = KeyboardPropertiesInstance().properties()->animator();
        std::shared_ptr<KeyboardInterface91tkl> protocol =
            std::make_shared<KeyboardInterface91tkl>(deviceInfo, animator);
        KeyboardCommandTransferInterface().setKeyboardCommandTransferImplementation(protocol);
    }
    break;
    case SupportedKeyboards::anorak_jellowcake:
    {
        std::shared_ptr<KeyboardInterfaceJellowcake> protocol =
            std::make_shared<KeyboardInterfaceJellowcake>(deviceInfo, nullptr);
        KeyboardCommandTransferInterface().setKeyboardCommandTransferImplementation(protocol);
    }
    break;
    }
}

void KeyboardFactory::createKeyboardProperties(SupportedKeyboards keyboardtype, const s_device_info &deviceInfo) const
{
    switch (keyboardtype)
    {
    case SupportedKeyboards::anorak_91tkl:
    {
        auto anorak91tkl = std::unique_ptr<KeyboardProperties91tkl>(new KeyboardProperties91tkl(deviceInfo));
        KeyboardPropertiesInstance().setKeyboardProperties(std::move(anorak91tkl));
    }
    break;
    case SupportedKeyboards::anorak_jellowcake:
    {
        auto anorakJellowcake =
            std::unique_ptr<KeyboardPropertiesJellowcake>(new KeyboardPropertiesJellowcake(deviceInfo));
        KeyboardPropertiesInstance().setKeyboardProperties(std::move(anorakJellowcake));
    }
    break;
    }
}

void KeyboardFactory::probeForKeyboards(SupportedKeyboards keyboardtype, present_keyboard_list &keyboardList) const
{
    switch (keyboardtype)
    {
    case SupportedKeyboards::anorak_91tkl:
    {
        qDebug() << "probe for SupportedKeyboards::anorak_91tkl";
        std::unique_ptr<KeyboardInterface91tkl> protocol =
            std::unique_ptr<KeyboardInterface91tkl>(new KeyboardInterface91tkl());
        device_info_list dil = protocol->getDevicesInfoList();
        if (!dil.empty())
        {
            keyboardList.emplace_back(s_present_keyboard_info(SupportedKeyboards::anorak_91tkl, dil));
        }
    }
    break;
    case SupportedKeyboards::anorak_jellowcake:
    {
        qDebug() << "probe for SupportedKeyboards::anorak_jellowcake";
        std::unique_ptr<KeyboardInterfaceJellowcake> protocol =
            std::unique_ptr<KeyboardInterfaceJellowcake>(new KeyboardInterfaceJellowcake());
        device_info_list dil = protocol->getDevicesInfoList();
        if (!dil.empty())
        {
            keyboardList.emplace_back(s_present_keyboard_info(SupportedKeyboards::anorak_jellowcake, dil));
        }
    }
    break;
    }
}

void KeyboardFactory::probeForKeyboards(present_keyboard_list &keyboardList) const
{
    probeForKeyboards(SupportedKeyboards::anorak_91tkl, keyboardList);
}

QString getKeyboardName(SupportedKeyboards keyboard)
{
    s_device_info deviceInfo;

    switch (keyboard)
    {
    case SupportedKeyboards::anorak_91tkl:
    {
        KeyboardProperties91tkl properties(deviceInfo);
        return properties.vendor() + " " + properties.name();
    }
    break;
    case SupportedKeyboards::anorak_jellowcake:
    {
        KeyboardPropertiesJellowcake properties(deviceInfo);
        return properties.vendor() + " " + properties.name();
    }
    break;
    }

    return "unknown";
}
