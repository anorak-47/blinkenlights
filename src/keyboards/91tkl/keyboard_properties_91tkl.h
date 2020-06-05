#pragma once
#include "keyboard_properties.h"
#include <memory>

class Keyboard91TKLProtocol;

class KeyboardProperties91tkl : public KeyboardProperties
{
public:
    KeyboardProperties91tkl(const s_device_info &deviceInfo);

    virtual void init() override;

    virtual QString name() const override;
    virtual QString vendor() const override;
    virtual QString description() const override;

    virtual unsigned int columns() const override;
    virtual unsigned int rows() const override;

    virtual bool supportsColorMaps() const override;
    virtual unsigned int colorMapCount() const override;
    virtual QString colorMapName(unsigned int mapid) const override;
    virtual bool supportsDifferentialMapUpdates() const override;
    virtual bool supportsRowBasedMapUpdates() const override;

    virtual bool supportsSectors() const override;
    virtual unsigned int sectorCount() const override;
    virtual QString sectorName(unsigned int sector) const override;
    std::vector<std::shared_ptr<KLEKey> > getSectorKeys(unsigned int sector) const override;

    virtual bool supportsAnimations() const override;
    virtual unsigned int animationCount() const override;
    virtual QString animationName(unsigned int animation) const override;
    virtual bool supportsNativeAnimationNames() const override;

    virtual QWidget* keyboardSettingsWidget() const override;
    virtual QWidget* consoleWidget() const override;
    virtual QWidget* interfaceSettingsDialog() const override;

protected:
    virtual QString jsonKeyboardLayout() const override;
    virtual void prepareSectors() override;

private:
    std::vector<std::vector<std::shared_ptr<KLEKey>>> _sectors;
};
