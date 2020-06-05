#include "common.h"
#include "keyboard_interface_91tkl.h"
#include "animator_91tkl.h"
#include <QtCore/QElapsedTimer>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QDebug>

KeyboardInterface91tkl::KeyboardInterface91tkl() : KeyboardInterface()
{
    _protocol_error = false;
    _protocol_timeout = false;
}

KeyboardInterface91tkl::KeyboardInterface91tkl(const s_device_info &deviceInfo, std::shared_ptr<Animator> animator)
    : KeyboardInterface(), _serialPortName(deviceInfo.portName), _animator(animator)
{
    _protocol_error = false;
    _protocol_timeout = false;

    qDebug() << "91tkl: using port: " << _serialPortName;
    _serialPort.setPort(_serialPortName);

    connect(&_serialPort, SIGNAL(response(QString)), this, SLOT(on_response(QString)));
    connect(&_serialPort, SIGNAL(output(QString)), this, SLOT(on_output(QString)));
    connect(&_serialPort, SIGNAL(error(QString)), this, SLOT(on_error(QString)));
    connect(&_serialPort, SIGNAL(timeout(QString)), this, SLOT(on_timeout(QString)));

    _serialPort.start();
}

KeyboardInterface91tkl::~KeyboardInterface91tkl()
{
    QMutexLocker l(&mutex);
    cond.wakeAll();
    _serialPort.close();
}

bool KeyboardInterface91tkl::sendCommandAndWaitForAck(QString const &request)
{
    QMutexLocker l(&mutex);
    qDebug() << __PRETTY_FUNCTION__ << ": request: " << request;

    writeRequest(request);
    cond.wait(&mutex, _waitTimeout_ms * 3);

    if (_protocol_error)
    {
        emit signalError(_("request [%1] failed with a protocol error").arg(request.simplified()));
        return false;
    }

    if (_protocol_timeout)
    {
        emit signalError(_("request [%1] failed with a protocol timeout").arg(request.simplified()));
        return false;
    }

    qDebug() << __PRETTY_FUNCTION__ << ": response: " << _response;

    if (!isValidResponse(_response))
    {
        emit signalError(_("request [%1] returned with an error").arg(request.simplified()));
        // qDebug() << "sendCommandAndWaitForAck: invalid response: [" << _response << "]";
        // qDebug() << "sendCommandAndWaitForAck: request: [" << request << "]";
        return false;
    }

    return true;
}

bool KeyboardInterface91tkl::isValidResponse(const QString &response)
{
    QStringList parts = response.split('\n', QString::SkipEmptyParts);

    // qDebug() << "parts: " << parts.length() << ", <" << parts.join("|") << ">";

    if (parts.empty())
    {
        qCritical() << "got empty response!";
        emit signalError(_("response is empty"));
        return false;
    }

    // qDebug() << "last: " << parts.last();
    // qDebug() << "last: " << (parts.last() == ">OK" ? "valid" : "invalid");

    if (parts.last() == ">ERR")
    {
        qCritical() << "got error: " << response;
        emit signalError(_("error [%1]").arg(response.simplified()));
        return false;
    }

    if (parts.last() != ">OK")
    {
        qCritical() << "got invalid response: " << response;
        emit signalError(_("invalid response [%1]").arg(response.simplified()));
        return false;
    }

    return true;
}

bool KeyboardInterface91tkl::getResponsePartsIfResponseIsValid(QString const &response, QString const &filter,
                                                               int expectedLength, QStringList &responseParts)
{
    QStringList parts = response.split('\n', QString::SkipEmptyParts);

    // at last the response string and ">OK"
    if (parts.length() < 2)
    {
        qCritical() << "ERROR: response to short: [" << response << "]";
        emit signalError(_("ERROR: response to short: [%1]").arg(response));
        return false;
    }

    if (parts.last() != ">OK")
    {
        qCritical() << "ERROR: command failed:  [" << response << "]";
        emit signalError(_("ERROR: command failed: [%1]").arg(response));
        return false;
    }

    QStringList response_parts = parts.filter(filter);

    if (response_parts.length() != 1)
    {
        qCritical() << "ERROR: filter string '" << filter << "' not found in response: [" << response << "]";
        emit signalError(_("ERROR: filter string '%1' not found in response: [%2]").arg(filter).arg(response));
        return false;
    }

    QString theRealResponse = response_parts.first();
    // qDebug() << "command response: " << theRealResponse;
    responseParts = theRealResponse.split(' ');

    if (responseParts.length() != expectedLength)
    {
        qCritical() << "ERROR: invalid response length: [" << response << "]";
        emit signalError(_("ERROR: response has %1 parts, expected %2. Parts: [%3]")
                             .arg(responseParts.length())
                             .arg(expectedLength)
                             .arg(responseParts.join("][")));
        return false;
    }

    return true;
}

int KeyboardInterface91tkl::scaleUpHue(int hue)
{
    return qRound((356.0 / 256.0) * hue);
}

int KeyboardInterface91tkl::scaleDownHue(int hue)
{
    return qRound((256.0 / 360.0) * hue);
}

void KeyboardInterface91tkl::setSectorIllumination(unsigned int sector, bool illuminate)
{
    static QString cmd("sector %1 %2");
    QString request = cmd.arg(sector).arg(illuminate);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::setSectorColor(unsigned int sector, const QColor &color)
{
    static QString cmd = "sector %1 %2 %3 %4";
    int hue = scaleDownHue(color.hue());
    QString request = cmd.arg(sector).arg(hue).arg(color.saturation()).arg(color.value());
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::saveSector(unsigned int sector)
{
    Q_UNUSED(sector);
    static QString cmd = "sector save";
    sendCommandAndWaitForAck(cmd);
}

void KeyboardInterface91tkl::saveMap(unsigned int map)
{
    Q_UNUSED(map);
    static QString cmd = "map save";
    sendCommandAndWaitForAck(cmd);
}

QString KeyboardInterface91tkl::getColorMapAsSingleRowJSONString(std::unique_ptr<s_map> colormap)
{
    unsigned int mapid = colormap->map;

    qDebug() << "colormap: map " << mapid;

    QJsonObject jsmap;
    jsmap["map"] = (int)mapid;

    struct s_map_row const &map_row = colormap->rows[0];

    if (map_row.entries.empty())
    {
        qCritical() << __PRETTY_FUNCTION__ << ": ERROR: row without entries!";
        return QString();
    }

    jsmap["row"] = (int)map_row.row;

    QJsonArray jscols;
    for (auto &key : map_row.entries)
    {
        QJsonArray jscol;
        jscol.push_back((int)key.col);
        jscol.push_back(key.color.name().remove(0, 1));
        jscols.push_back(jscol);
    }

    jsmap["cols"] = jscols;

    QJsonDocument jd(jsmap);
    return jd.toJson(QJsonDocument::Compact);
}

QString KeyboardInterface91tkl::getColorMapAsJSONString(std::unique_ptr<s_map> colormap, bool compact)
{
    if (colormap->rows.size() != 1)
        return KeyboardInterface::getColorMapAsJSONString(std::move(colormap), compact);

    return getColorMapAsSingleRowJSONString(std::move(colormap));
}

void KeyboardInterface91tkl::setMap(std::unique_ptr<s_map> colormap)
{
    if (colormap->rows.size() != 1)
    {
        qCritical() << __PRETTY_FUNCTION__ << ": ERROR: supports only row based updates!";
        qCritical() << __PRETTY_FUNCTION__ << ": ERROR: number of given rows: " << colormap->rows.size();
        return;
    }

    QString request = getColorMapAsJSONString(std::move(colormap));
    request.prepend("map set ");
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::showMap(unsigned int map)
{
    static QString cmd("map %1");
    QString request = cmd.arg(map);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::getMapRow(unsigned int map, unsigned int row, s_map_row &map_row)
{
    static QString cmd("map %1 %2");

    if (!sendCommandAndWaitForAck(cmd.arg(map).arg(row)))
        return;

    // !map 1 2
    // .map {...}
    // >OK

    QStringList parts;
    if (!getResponsePartsIfResponseIsValid(_response, ".map ", 2, parts))
    {
        qWarning() << "request [" << cmd << "] failed!";
        return;
    }

    QString const &jsonstring = parts[1];

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonstring.toUtf8(), &error);

    if (!doc.isNull() && doc.isObject())
    {
        QJsonObject obj = doc.object();

        // qDebug() << __PRETTY_FUNCTION__ << ": obj";

        QJsonValue jmap = obj.value("map");
        if (jmap.isNull() || !jmap.isDouble())
        {
            qCritical() << "getMapRow: key 'map' not found [" << jsonstring << "]";
            return;
        }

        unsigned int response_map = jmap.toInt();

        // qDebug() << __PRETTY_FUNCTION__ << ": map " << response_map;

        if (response_map != map)
        {
            qCritical() << "getMapRow: map '" << response_map << "' does not match requested map '" << map << "'!";
            return;
        }

        QJsonValue jrow = obj.value("row");
        if (jrow.isNull() || !jrow.isDouble())
        {
            qCritical() << "getMapRow: key 'row' not found [" << jsonstring << "]";
            return;
        }

        unsigned int response_row = jrow.toInt();

        if (response_row != row)
        {
            qCritical() << "getMapRow: row '" << response_row << "' does not match requested row '" << row << "'!";
            return;
        }

        // qDebug() << __PRETTY_FUNCTION__ << ": row " << response_row;
        map_row.row = static_cast<unsigned int>(response_row);

        QJsonValue jcols = obj.value("cols");
        if (jcols.isNull() || !jcols.isArray())
        {
            qCritical() << "getMapRow: key 'cols' not found [" << jsonstring << "]";
            return;
        }

        QJsonArray jacols = jcols.toArray();

        // qDebug() << __PRETTY_FUNCTION__ << ": cols " << jacols.size();

        for (auto const &col : jacols)
        {
            QJsonArray jacol = col.toArray();

            // qDebug() << __PRETTY_FUNCTION__ << ": tokens " << jacol.size();
            // qDebug() << __PRETTY_FUNCTION__ << ": column " << jacol[0].toDouble();
            // qDebug() << __PRETTY_FUNCTION__ << ": color " << jacol[1].toString();

            unsigned int response_col = jacol[0].toDouble();
            QColor response_color(QString("#%1").arg(jacol[1].toString()));

            // qDebug() << __PRETTY_FUNCTION__ << ": col " << response_col;
            // qDebug() << __PRETTY_FUNCTION__ << ": color " << response_color.name();

            map_row.entries.emplace_back(s_map_entry(response_row, response_col, response_color));
        }
    }
    else
    {
        qCritical() << __PRETTY_FUNCTION__ << ": ERROR: parsing the json response string: <" << doc.toJson() << ">";
        qCritical() << __PRETTY_FUNCTION__ << ": ERROR: " << error.errorString();
    }
}

std::shared_ptr<s_map> KeyboardInterface91tkl::getMap(unsigned int map)
{
    std::shared_ptr<s_map> colormap = std::shared_ptr<s_map>(new s_map);
    colormap->map = map;

    for (unsigned int row = 0; row < 6; row++)
    {
        struct s_map_row map_row;
        getMapRow(map, row, map_row);
        colormap->rows.push_back(map_row);
    }

    // qDebug() << __PRETTY_FUNCTION__ << ": done for map " << map;

    return colormap;
}

bool KeyboardInterface91tkl::getSectorIllumination(unsigned int sector)
{
    static QString cmd("sector %1");
    QString request = cmd.arg(sector);
    if (!sendCommandAndWaitForAck(request))
        return false;

    // !sector 1
    // .sector 1 1 80 FF 7B
    // >OK

    QStringList parts;
    if (!getResponsePartsIfResponseIsValid(_response, ".sector ", 6, parts))
        return false;

    bool illuminated = parts[2].toInt();
    return illuminated;
}

QColor KeyboardInterface91tkl::getSectorColor(unsigned int sector)
{
    static QString cmd("sector %1");
    QString request = cmd.arg(sector);
    if (!sendCommandAndWaitForAck(request))
        return QColor();

    // !sector 1
    // .sector 1 1 80 FF 7B
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".sector ", 6, iparts))
        return QColor();

    bool ok;
    int hue = iparts[3].toInt(&ok, 16);
    int saturation = iparts[4].toInt(&ok, 16);
    int value = iparts[5].toInt(&ok, 16);
    hue = scaleUpHue(hue);
    return QColor::fromHsv(hue, saturation, value);
}

void KeyboardInterface91tkl::setAnimationRunning(unsigned int animation, bool running)
{
    static QString cmd("animation %1 %2");
    QString request = cmd.arg(animation).arg(running);
    sendCommandAndWaitForAck(request);

    qDebug() << __PRETTY_FUNCTION__ << " animator";

    if (_animator)
    {
        qDebug() << __PRETTY_FUNCTION__ << " set animator";

        if (running)
            _animator->startAnimation(animation);
        else
            _animator->stopAnimation();
    }
}

void KeyboardInterface91tkl::setAnimationColor(unsigned int animation, unsigned int colorid, const QColor &color)
{
    static QString cmd("animation %1 c %2 %3 %4 %5");
    int hue = scaleDownHue(color.hue());
    QString request = cmd.arg(animation).arg(colorid).arg(hue).arg(color.saturation()).arg(color.value());
    sendCommandAndWaitForAck(request);

    if (_animator)
    {
        if (colorid == 0)
            _animator->setAnimationColor1(color);
        else
            _animator->setAnimationColor2(color);
    }
}

void KeyboardInterface91tkl::setAnimationFPS(unsigned int animation, unsigned int fps)
{
    static QString cmd("animation %1 fps %2");
    QString request = cmd.arg(animation).arg(fps);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::saveAnimation(unsigned int animation)
{
    static QString cmd("animation %1 save");
    QString request = cmd.arg(animation);
    sendCommandAndWaitForAck(request);
}

unsigned int KeyboardInterface91tkl::getAnimationCurrent()
{
    static QString cmd("animation");
    if (!sendCommandAndWaitForAck(cmd))
        return 0;

    // !animation
    // .animation 0
    // >OK

    QStringList parts;
    if (!getResponsePartsIfResponseIsValid(_response, ".animation ", 2, parts))
    {
        qDebug() << "request [" << cmd << "] failed!";
        return 0;
    }

    return parts[1].toUInt();
}

bool KeyboardInterface91tkl::getAnimationRunning(unsigned int animation)
{
    static QString cmd("animation %1");
    QString request = cmd.arg(animation);
    if (!sendCommandAndWaitForAck(request))
        return false;

    // !animation 0
    // .animation 0 0
    // >OK

    QStringList parts;
    if (!getResponsePartsIfResponseIsValid(_response, ".animation ", 3, parts))
        return false;

    return parts[2].toUInt();
}

QColor KeyboardInterface91tkl::getAnimationColor(unsigned int animation, unsigned int colorid)
{
    static QString cmd("animation %1 c %2");
    QString request = cmd.arg(animation).arg(colorid);
    if (!sendCommandAndWaitForAck(request))
        return QColor();

    // !animation 0 c 1
    // .animation 0 c 1 82 46 C2
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".animation ", 7, iparts))
        return QColor();

    bool ok;
    int hue = iparts[4].toInt(&ok, 16);
    int saturation = iparts[5].toInt(&ok, 16);
    int value = iparts[6].toInt(&ok, 16);
    hue = scaleUpHue(hue);
    return QColor::fromHsv(hue, saturation, value);
}

unsigned int KeyboardInterface91tkl::getAnimationFPS(unsigned int animation)
{
    static QString cmd("animation %1 fps");
    QString request = cmd.arg(animation);
    if (!sendCommandAndWaitForAck(request))
        return 1;

    // !animation 0 fps
    // .animation 0 fps 25
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".fps ", 3, iparts))
        return 1;

    bool ok;
    int fps = iparts[2].toInt(&ok);
    return (ok ? fps : 1);
}

QStringList KeyboardInterface91tkl::getAnimationNames()
{
    QString request = "animation list";
    if (!sendCommandAndWaitForAck(request))
        return QStringList();

    /*
    qDebug() << request;

    writeRequest(request);
    cond.wait(&mutex, _waitTimeout_ms * 3);

    if (!isValidResponse(_response))
    {
        qDebug() << "getAnimationNames: invalid response: " << _response;
        return QStringList();
    }
    */

    QStringList parts = _response.split('\n', QString::SkipEmptyParts);

    if (parts.length() < 2)
    {
        qDebug() << "getAnimationNames: invalid response: too short! " << _response;
        return QStringList();
    }

    QStringList response_parts = parts.filter(QRegExp("^\\.name [0-9]+ .+"));

    if (response_parts.length() == 0)
    {
        qDebug() << "getAnimationNames: invalid response: no animation names! [" << response_parts.join("|") << "]";
        return QStringList();
    }

    QStringList animation_names;

    for (int i = 0; i < response_parts.size(); ++i)
    {
        QStringList iparts = response_parts[i].split(' ');

        if (iparts.size() <= 2)
        {
            qDebug() << "getAnimationNames: invalid response: invalid name! " << _response;
            return QStringList();
        }

        iparts.pop_front();
        iparts.pop_front();

        animation_names += iparts.join(" ");
    }

    qDebug() << "getAnimationNames: " << animation_names.join("|");

    return animation_names;
}

void KeyboardInterface91tkl::setLedIllumination(unsigned int led, bool illuminate)
{
    static QString cmd("stabri %1 %2");
    QString request = cmd.arg(led).arg(illuminate ? "on" : "off");
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::setLedBrightness(unsigned int led, unsigned int brightness)
{
    static QString cmd("stabri %1 %2");
    QString request = cmd.arg(led).arg(brightness);
    sendCommandAndWaitForAck(request);
}

unsigned int KeyboardInterface91tkl::getLedBrightness(unsigned int led)
{
    static QString cmd("stabri %1");
    QString request = cmd.arg(led);
    if (!sendCommandAndWaitForAck(request))
        return 0;

    // !stabri 1
    // .stabri 1 254
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".brightness ", 3, iparts))
        return 0;

    return iparts[2].toUInt();
}

void KeyboardInterface91tkl::getBacklightState(bool &enabled, unsigned int &level)
{
    bool ok;
    static QString cmd("bl");
    QString request = cmd;
    if (!sendCommandAndWaitForAck(request))
        return;

    // !bl
    // .backlight_config.raw: 0F
    // .enable: 1
    // .level: 7

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".enable", 2, iparts))
        return;

    enabled = iparts[1].toUInt(&ok);

    if (!getResponsePartsIfResponseIsValid(_response, ".level", 2, iparts))
        return;

    level = iparts[1].toUInt(&ok);
}

void KeyboardInterface91tkl::setBacklightState(bool enabled, unsigned int level)
{
    Q_UNUSED(level);
    static QString cmd("bl enable %1");
    QString request = cmd.arg(enabled);
    sendCommandAndWaitForAck(request);
}

unsigned int KeyboardInterface91tkl::getBacklightGlobalBrightness()
{
    bool ok;
    static QString cmd("issi gbr");
    QString request = cmd;
    if (!sendCommandAndWaitForAck(request))
        return 0;

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".gbr", 2, iparts))
        return 0;

    unsigned int brightness = iparts[1].toUInt(&ok);
    if (!ok)
        return 0;
    return brightness;
}

void KeyboardInterface91tkl::setBacklightGlobalBrightness(unsigned int brightness)
{
    static QString cmd("issi gbr %1");
    QString request = cmd.arg(brightness);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::saveBacklightState()
{
    sendCommandAndWaitForAck("issi save");
}

void KeyboardInterface91tkl::setKeymapConfig(unsigned int config)
{
    static QString cmd("keymap raw %1");
    QString request = cmd.arg(config);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::setDebugConfig(unsigned int config)
{
    static QString cmd("debug raw %1");
    QString request = cmd.arg(config);
    sendCommandAndWaitForAck(request);
}

unsigned int KeyboardInterface91tkl::getKeymapConfig()
{
    static QString cmd("keymap");
    if (!sendCommandAndWaitForAck(cmd))
        return 0;

    // !keymap
    // .keymap_config.raw 0
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".keymap_config.raw ", 2, iparts))
        return 0;

    bool ok;
    unsigned int raw = iparts[1].toUInt(&ok, 16);
    return (ok ? raw : 0);
}

unsigned int KeyboardInterface91tkl::getDebugConfig()
{
    static QString cmd("debug");
    if (!sendCommandAndWaitForAck(cmd))
        return 0;

    // !debug
    // .debug_config.raw 00
    // >OK

    QStringList iparts;
    if (!getResponsePartsIfResponseIsValid(_response, ".debug_config.raw ", 2, iparts))
        return 0;

    bool ok;
    unsigned int raw = iparts[1].toUInt(&ok, 16);
    return (ok ? raw : 0);
}

void KeyboardInterface91tkl::saveLedConfig()
{
    QString cmd("stabri save");
    sendCommandAndWaitForAck(cmd);
}

void KeyboardInterface91tkl::saveKeymapConfig()
{
    QString cmd("keymap save");
    sendCommandAndWaitForAck(cmd);
}

void KeyboardInterface91tkl::saveDebugConfig()
{
    QString cmd("debug save");
    sendCommandAndWaitForAck(cmd);
}

void KeyboardInterface91tkl::setSleepLed(bool on)
{
    static QString cmd("sleepled %1");
    QString request = cmd.arg(on);
    sendCommandAndWaitForAck(request);
}

void KeyboardInterface91tkl::enterBootloader()
{
    static QString cmd("boot");
    sendCommandAndWaitForAck(cmd);
    // expect to disconnect...
}

void KeyboardInterface91tkl::getKeyboardInfo()
{
    static QString cmd = "info";
    if (sendCommandAndWaitForAck(cmd))
    {
        QStringList parts = _response.split('\n', QString::SkipEmptyParts);

        if (parts.length() < 2)
        {
            qDebug() << "getKeyboardInfo: invalid response: " << _response;
            return;
        }

        // TODO...
    }
}

void KeyboardInterface91tkl::on_request(const QString &request)
{
    // qDebug() << "on_request: " << request;
    writeRequest(request);
}

void KeyboardInterface91tkl::on_response(const QString &s)
{
    QMutexLocker l(&mutex);
    // qDebug() << "on_response: " << s;
    _response = s;
    emit signalResponse(_response);
    cond.wakeAll();
}

void KeyboardInterface91tkl::on_output(const QString &s)
{
    QMutexLocker l(&mutex);
    // qDebug() << "on_output: " << s;
    emit signalMessage(s);
}

void KeyboardInterface91tkl::on_error(const QString &s)
{
    // qDebug() << "on_error: " << s;
    _protocol_error = true;
    emit signalError(s);
    cond.wakeAll();
}

void KeyboardInterface91tkl::on_timeout(const QString &s)
{
    // qDebug() << "on_timeout: " << s;
    _protocol_timeout = true;
    emit signalError(s);
    cond.wakeAll();
}

void KeyboardInterface91tkl::writeRequest(const QString &request)
{
    static QString finalRequest("!%1\n");
    _response.clear();
    _protocol_error = false;
    _protocol_timeout = false;
    emit signalRequest(finalRequest.arg(request));
    _serialPort.transaction(finalRequest.arg(request));
}

device_info_list KeyboardInterface91tkl::getDevicesInfoList()
{
    device_info_list deviceInfoList;
    QStringList probePortNames;

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        if (info.vendorIdentifier() == 0x1209 && info.productIdentifier() == 0xaa91)
        {
            probePortNames += info.portName();
        }
    }

    for (QStringList::iterator it = probePortNames.begin(); it != probePortNames.end(); ++it)
    {
        struct s_device_info deviceInfo;
        QString port = *it;
        qDebug() << "probe port " << port;

        SerialPortProbeThread *portProbeThread = new SerialPortProbeThread(port, deviceInfo);

        portProbeThread->start();
        portProbeThread->wait();

        if (portProbeThread->foundAKeyboard())
        {
            for (const QSerialPortInfo &info : infos)
            {
                if (info.portName() == port)
                {
                    deviceInfo.serialnumber = info.serialNumber();
                }
            }

            qDebug() << "keyboard: " << deviceInfo.manufacturer;
            qDebug() << "keyboard: " << deviceInfo.product;
            qDebug() << "keyboard: " << deviceInfo.serialnumber;

            deviceInfo.portName = port;
            deviceInfoList.push_back(deviceInfo);
        }
        else if (portProbeThread->openFailed())
        {
            qDebug() << "error opening port: " << portProbeThread->lastError();
        }
        else if (!portProbeThread->lastError().isEmpty())
        {
            qDebug() << "error: " << portProbeThread->lastError();
        }

        delete portProbeThread;
    }

    return deviceInfoList;
}

SerialPortProbeThread::SerialPortProbeThread(QString const &portName, struct s_device_info &deviceInfo, QObject *parent)
    : QThread(parent), _portName(portName), _deviceInfo(deviceInfo)
{
}

SerialPortProbeThread::~SerialPortProbeThread()
{
}

void SerialPortProbeThread::run()
{
    QElapsedTimer timer;
    QSerialPort serial;
    serial.setPortName(_portName);

    qDebug() << "probe port " << _portName;

    if (!serial.open(QIODevice::ReadWrite))
    {
        // emit error(_("Can't open %1, error (code %2):
        // %3").arg(portName).arg(serial.error()).arg(serial.errorString()));
        _open_failed = true;
        _last_error = _("Can't open serial port %1, error (code %2): %3")
                          .arg(_portName)
                          .arg(serial.error())
                          .arg(serial.errorString());
        return;
    }

    qDebug() << "opened " << _portName;

    QByteArray requestData = QString("!info\n").toLocal8Bit();
    serial.write(requestData);
    if (serial.waitForBytesWritten(250))
    {
        qDebug() << "waitForBytesWritten " << _portName;

        QByteArray responseData = serial.readAll();
        QString partialResponse(responseData);

        qDebug() << "partialResponse: " << partialResponse;

        timer.start();

        while (!partialResponse.endsWith(">OK\n") && !partialResponse.endsWith(">ERR\n") &&
               !partialResponse.contains(">NC\n"))
        {
            while (serial.waitForReadyRead(1))
                responseData += serial.readAll();

            partialResponse = QString(responseData);

            if (timer.elapsed() > 1000)
                break;
        }

        QString response(responseData);
        qDebug() << "response: " << response;
        // emit this->response(response);

        bool foundPid = false;
        bool foundVid = false;
        QStringList parts = response.split("\n");

        qDebug() << "parts: [" << parts.join("][") << "]";

        int pos = parts.indexOf(QRegExp("^\\.pid.*"));
        if (pos != -1)
        {
            _deviceInfo.pid = parts[pos].section(" ", 1);
            qDebug() << "pid " << _deviceInfo.pid;
            if (parts[pos].contains("AA91"))
                foundPid = true;
        }

        pos = parts.indexOf(QRegExp("^\\.vid.*"));
        if (pos != -1)
        {
            _deviceInfo.vid = parts[pos].section(" ", 1);
            qDebug() << "vid " << _deviceInfo.vid;
            if (parts[pos].contains("1209"))
                foundVid = true;
        }
        pos = parts.indexOf(QRegExp("^\\.desc.*"));
        if (pos != -1)
        {
            _deviceInfo.description = parts[pos].section(" ", 1);
        }
        pos = parts.indexOf(QRegExp("^\\.product.*"));
        if (pos != -1)
        {
            _deviceInfo.product = parts[pos].section(" ", 1);
        }
        pos = parts.indexOf(QRegExp("^\\.manufacturer.*"));
        if (pos != -1)
        {
            _deviceInfo.manufacturer = parts[pos].section(" ", 1);
        }
        pos = parts.indexOf(QRegExp("^\\.version.*"));
        if (pos != -1)
        {
            _deviceInfo.version = parts[pos].section(" ", 1);
        }
        pos = parts.indexOf(QRegExp("^\\.build.*"));
        if (pos != -1)
        {
            _deviceInfo.build = parts[pos].section(" ", 1);
        }

        _foundAKeyboard = (foundPid && foundVid);
    }
}

bool SerialPortProbeThread::foundAKeyboard() const
{
    return _foundAKeyboard;
}

bool SerialPortProbeThread::openFailed() const
{
    return _open_failed;
}

QString SerialPortProbeThread::lastError() const
{
    return _last_error;
}
