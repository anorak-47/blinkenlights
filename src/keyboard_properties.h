#pragma once
#include "klekey.h"
#include "keyboard_interface.h"
#include <QWidget>
#include <QtCore/QString>
#include <memory>
#include <vector>

class Animator;

class KeyboardProperties
{
public:
    KeyboardProperties();
    KeyboardProperties(const s_device_info &deviceInfo);
    virtual ~KeyboardProperties();

    virtual void init();

    virtual QString name() const;
    virtual QString vendor() const;
    virtual QString description() const;

    virtual unsigned int columns() const;
    virtual unsigned int rows() const;

    virtual bool supportsColorMaps() const;
    virtual unsigned int colorMapCount() const;
    virtual QString colorMapName(unsigned int mapid) const;
    virtual bool supportsDifferentialMapUpdates() const;
    virtual bool supportsRowBasedMapUpdates() const;

    virtual bool supportsSectors() const;
    virtual unsigned int sectorCount() const;
    virtual QString sectorName(unsigned int sector) const;
    virtual std::vector<std::shared_ptr<KLEKey>> getSectorKeys(unsigned int sector) const;

    std::shared_ptr<KLEKey> findKeyAt(double x, double y) const;
    std::shared_ptr<KLEKey> findKey(int row, int col) const;

    std::vector<std::shared_ptr<KLEKey>> getKeys() const;

    virtual bool supportsAnimations() const;
    virtual unsigned int animationCount() const;
    virtual QString animationName(unsigned int sector) const;
    virtual bool supportsNativeAnimationNames() const;

    virtual QWidget *keyboardSettingsWidget() const;
    virtual QWidget *consoleWidget() const;
    virtual QWidget *interfaceSettingsDialog() const;

    virtual std::shared_ptr<Animator> animator() const;

protected:
    virtual QString jsonKeyboardLayout() const;
    virtual void prepareSectors();

    void parseJSONKeyboardLayout();

    unsigned int real_columns = 0;
    double width = 0;

    std::vector<std::shared_ptr<KLEKey>> _keys;
    std::shared_ptr<Animator> _animator;
    s_device_info _deviceInfo;
};

class KeyboardPropertiesInstance
{
public:
    KeyboardPropertiesInstance();

    void setKeyboardProperties(std::unique_ptr<KeyboardProperties> properties);
    KeyboardProperties const *properties() const;

private:
    static std::unique_ptr<KeyboardProperties> _properties;
};
