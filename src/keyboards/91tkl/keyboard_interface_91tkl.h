#pragma once
#include "keyboard_interface.h"
#include "threaded_serial_port_91tkl.h"
#include <memory>

class Animator;

class SerialPortProbeThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialPortProbeThread(QString const &portName, struct s_device_info &deviceInfo,
                                   QObject *parent = nullptr);
    ~SerialPortProbeThread();

    void run() override;

    bool foundAKeyboard() const;
    bool openFailed() const;
    QString lastError() const;

private:
    QString _portName;
    struct s_device_info &_deviceInfo;
    bool _foundAKeyboard = false;
    bool _open_failed = false;
    QString _last_error;
};

class KeyboardInterface91tkl : public KeyboardInterface
{
    Q_OBJECT

public:
    KeyboardInterface91tkl();
    KeyboardInterface91tkl(const s_device_info &deviceInfo, std::shared_ptr<Animator> animator);
    ~KeyboardInterface91tkl();

    device_info_list getDevicesInfoList() override;

    void setSectorIllumination(unsigned int sector, bool illuminate) override;
    void setSectorColor(unsigned int sector, QColor const &color) override;
    QString getColorMapAsJSONString(std::unique_ptr<s_map> colormap, bool compact = true) override;
    void setMap(std::unique_ptr<struct s_map> map) override;
    void showMap(unsigned int map) override;
    void saveSector(unsigned int sector) override;
    void saveMap(unsigned int map) override;

    bool getSectorIllumination(unsigned int sector) override;
    QColor getSectorColor(unsigned int sector) override;
    std::shared_ptr<s_map> getMap(unsigned int map) override;

    void setAnimationRunning(unsigned int animation, bool running) override;
    void setAnimationColor(unsigned int animation, unsigned int colorid, QColor const &color) override;
    void setAnimationFPS(unsigned int animation, unsigned int fps) override;
    void saveAnimation(unsigned int animation) override;

    unsigned int getAnimationCurrent() override;
    bool getAnimationRunning(unsigned int animation) override;
    QColor getAnimationColor(unsigned int animation, unsigned int colorid) override;
    unsigned int getAnimationFPS(unsigned int animation) override;
    QStringList getAnimationNames() override;

    void setLedIllumination(unsigned int led, bool illuminate) override;
    void setLedBrightness(unsigned int led, unsigned int brightness) override;
    unsigned int getLedBrightness(unsigned int led) override;

    void getBacklightState(bool &enabled, unsigned int &level) override;
    void setBacklightState(bool enabled, unsigned int level) override;
    unsigned int getBacklightGlobalBrightness() override;
    void setBacklightGlobalBrightness(unsigned int brightness) override;
    void saveBacklightState() override;

    void setKeymapConfig(unsigned int config) override;
    void setDebugConfig(unsigned int config) override;

    unsigned int getKeymapConfig() override;
    unsigned int getDebugConfig() override;

    void saveLedConfig() override;
    void saveKeymapConfig() override;
    void saveDebugConfig() override;

    void setSleepLed(bool on) override;

    void enterBootloader() override;

    void getKeyboardInfo() override;

public slots:
    void on_request(QString const &request);

private slots:
    void on_response(const QString &s);
    void on_error(const QString &s);
    void on_timeout(const QString &s);
    void on_output(const QString &s);

protected:
    bool sendCommandAndWaitForAck(const QString &request);
    void writeRequest(QString const &request);
    bool isValidResponse(QString const &response);
    int scaleDownHue(int hue);
    int scaleUpHue(int hue);
    bool getResponsePartsIfResponseIsValid(const QString &response, const QString &filter, int expectedLength,
                                           QStringList &parts);
    void getMapRow(unsigned int map, unsigned int row, s_map_row &map_row);
    bool probePort(const QString &portName, s_device_info &deviceInfo);
    QString getColorMapAsSingleRowJSONString(std::unique_ptr<s_map> colormap);

    ThreadedSerialPort91tkl _serialPort;
    QString _serialPortName;
    int _waitTimeout_ms = 200;

    QMutex mutex;
    QWaitCondition cond;

    QString _response;

    bool _protocol_error;
    bool _protocol_timeout;

    std::shared_ptr<Animator> _animator;
};
