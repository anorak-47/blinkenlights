#pragma once
#include "keyboard_interface_91tkl.h"
#include "threaded_serial_port_jellowcake.h"
#include <memory>

class Animator;

class SerialPortProbeJellowcakeThread : public QThread
{
    Q_OBJECT

public:
    explicit SerialPortProbeJellowcakeThread(QString const &portName, struct s_device_info &deviceInfo,
                                             QObject *parent = nullptr);
    ~SerialPortProbeJellowcakeThread();

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

class KeyboardInterfaceJellowcake : public KeyboardInterface91tkl
{
    Q_OBJECT

public:
    KeyboardInterfaceJellowcake();
    KeyboardInterfaceJellowcake(const s_device_info &deviceInfo, std::shared_ptr<Animator> animator);

    device_info_list getDevicesInfoList() override;

#if 0
    void setSectorIllumination(unsigned int sector, bool illuminate) override;
    void setSectorColor(unsigned int sector, QColor const &color) override;
    void saveSector(unsigned int sector) override;

    bool getSectorIllumination(unsigned int sector) override;
    QColor getSectorColor(unsigned int sector) override;

    void setLedIllumination(unsigned int led, bool illuminate) override;
    void setLedBrightness(unsigned int led, unsigned int brightness) override;
    unsigned int getLedBrightness(unsigned int led) override;

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

    ThreadedSerialPortJellowcake _serialPort;
    QString _serialPortName;
    int _waitTimeout_ms = 200;

    QMutex mutex;
    QWaitCondition cond;

    QString _response;

    bool _protocol_error;
    bool _protocol_timeout;

    std::shared_ptr<Animator> _animator;
#endif
};
