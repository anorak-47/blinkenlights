#pragma once
#include "keyboard_properties.h"
#include <memory>

class KeyboardJellowcakeProtocol;

class KeyboardPropertiesJellowcake : public KeyboardProperties
{
public:
    KeyboardPropertiesJellowcake(const s_device_info &deviceInfo);

    void init() override;

    QString name() const override;
    QString vendor() const override;
    QString description() const override;

    unsigned int columns() const override;
    unsigned int rows() const override;

    bool supportsColorMaps() const override;
    unsigned int colorMapCount() const override;
    QString colorMapName(unsigned int mapid) const override;
    bool supportsDifferentialMapUpdates() const override;
    bool supportsRowBasedMapUpdates() const override;

    bool supportsSectors() const override;
    unsigned int sectorCount() const override;
    QString sectorName(unsigned int sector) const override;
    std::vector<std::shared_ptr<KLEKey>> getSectorKeys(unsigned int sector) const override;

    bool supportsAnimations() const override;
    unsigned int animationCount() const override;
    QString animationName(unsigned int animation) const override;
    bool supportsNativeAnimationNames() const override;

    QWidget *keyboardSettingsWidget() const override;
    QWidget *consoleWidget() const override;
    QWidget *interfaceSettingsDialog() const override;

protected:
    QString jsonKeyboardLayout() const override;
    void prepareSectors() override;

private:
    std::vector<std::vector<std::shared_ptr<KLEKey>>> _sectors;
};
