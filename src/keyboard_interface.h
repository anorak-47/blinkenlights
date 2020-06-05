#pragma once

#include <QtGui/QColor>
#include <QtCore/QObject>
#include <memory>

struct s_map_entry
{
    s_map_entry(unsigned int r, unsigned int c, QColor const &co) : row(r), col(c), color(co)
    {
    }
    unsigned int row;
    unsigned int col;
    QColor color;
};

struct s_map_row
{
    unsigned int row;
    std::vector<struct s_map_entry> entries;
};

struct s_map
{
    unsigned int map;
    std::vector<struct s_map_row> rows;
};

typedef std::shared_ptr<s_map> sMapPtr;

struct s_device_info
{
    QString vid;
    QString pid;

    QString manufacturer;
    QString product;
    QString description;
    QString version;
    QString build;
    QString serialnumber;

    QString portName;
};

typedef std::vector<struct s_device_info> device_info_list;

class KeyboardInterface : public QObject
{
    Q_OBJECT

public:
    KeyboardInterface();
    virtual ~KeyboardInterface();

    virtual device_info_list getDevicesInfoList() = 0;

    virtual void setSectorIllumination(unsigned int sector, bool illuminate) = 0;
    virtual void setSectorColor(unsigned int sector, QColor const &color) = 0;
    virtual QString getColorMapAsJSONString(std::unique_ptr<s_map> colormap, bool compact = true);
    virtual sMapPtr getColorMapFromJSONString(QString colormap);
    virtual void setMap(std::unique_ptr<struct s_map> map) = 0;
    virtual void showMap(unsigned int map) = 0;
    virtual void saveSector(unsigned int sector) = 0;
    virtual void saveMap(unsigned int map) = 0;

    virtual bool getSectorIllumination(unsigned int sector) = 0;
    virtual QColor getSectorColor(unsigned int sector) = 0;
    virtual std::shared_ptr<s_map> getMap(unsigned int map) = 0;

    virtual void setAnimationRunning(unsigned int animation, bool running) = 0;
    virtual void setAnimationColor(unsigned int animation, unsigned int colorid, QColor const &color) = 0;
    virtual void setAnimationFPS(unsigned int animation, unsigned int fps) = 0;
    virtual void saveAnimation(unsigned int animation) = 0;

    virtual unsigned int getAnimationCurrent() = 0;
    virtual bool getAnimationRunning(unsigned int animation) = 0;
    virtual QColor getAnimationColor(unsigned int animation, unsigned int colorid) = 0;
    virtual unsigned int getAnimationFPS(unsigned int animation) = 0;
    virtual QStringList getAnimationNames() = 0;

    virtual void setLedIllumination(unsigned int led, bool illuminate) = 0;
    virtual void setLedBrightness(unsigned int led, unsigned int brightness) = 0;
    virtual unsigned int getLedBrightness(unsigned int led) = 0;
    virtual void saveLedConfig() = 0;

    virtual void getBacklightState(bool &enabled, unsigned int &level) = 0;
    virtual void setBacklightState(bool enabled, unsigned int level) = 0;
    virtual unsigned int getBacklightGlobalBrightness() = 0;
    virtual void setBacklightGlobalBrightness(unsigned int brightness) = 0;
    virtual void saveBacklightState() = 0;

    virtual void setKeymapConfig(unsigned int config) = 0;
    virtual unsigned int getKeymapConfig() = 0;
    virtual void saveKeymapConfig() = 0;

    virtual void setDebugConfig(unsigned int config) = 0;
    virtual unsigned int getDebugConfig() = 0;
    virtual void saveDebugConfig() = 0;

    virtual void setSleepLed(bool on) = 0;
    virtual void enterBootloader() = 0;

    virtual void getKeyboardInfo() = 0;

signals:
    void signalRequest(QString const &request);
    void signalResponse(QString const &response);
    void signalWarning(QString const &warnmsg);
    void signalError(QString const &errormsg);
    void signalMessage(QString const &msg);
};
