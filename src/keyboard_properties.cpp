#include "klekey.h"
#include "common.h"
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <map>
#include <math.h>
#include "keyboard_properties.h"

KeyboardProperties::KeyboardProperties()
{

}

KeyboardProperties::KeyboardProperties(const s_device_info &deviceInfo) : _deviceInfo(deviceInfo)
{

}

KeyboardProperties::~KeyboardProperties()
{

}

void KeyboardProperties::init()
{

}

QString KeyboardProperties::name() const
{
    return _("unknown");
}

QString KeyboardProperties::vendor() const
{
    return _("unknown");
}

QString KeyboardProperties::description() const
{
    return _("unknown");
}

unsigned int KeyboardProperties::columns() const
{
    return 0;
}

unsigned int KeyboardProperties::rows() const
{
    return 0;
}

bool KeyboardProperties::supportsColorMaps() const
{
    return false;
}

unsigned int KeyboardProperties::colorMapCount() const
{
    return 0;
}

QString KeyboardProperties::colorMapName(unsigned int mapid) const
{
    return QString(_("Map %1")).arg(mapid);
}

bool KeyboardProperties::supportsDifferentialMapUpdates() const
{
    return false;
}

bool KeyboardProperties::supportsRowBasedMapUpdates() const
{
    return false;
}

bool KeyboardProperties::supportsSectors() const
{
    return false;
}

unsigned int KeyboardProperties::sectorCount() const
{
    return 0;
}

QString KeyboardProperties::sectorName(unsigned int) const
{
    return QString();
}

std::vector<std::shared_ptr<KLEKey>> KeyboardProperties::getSectorKeys(unsigned int sector) const
{
    Q_UNUSED(sector);
    return std::vector<std::shared_ptr<KLEKey>>();
}

std::shared_ptr<KLEKey> KeyboardProperties::findKeyAt(double x, double y) const
{
    for (auto &key : _keys)
    {
        if (key->x == x && key->y == y)
        {
            return key;
        }
    }

    return std::shared_ptr<KLEKey>();
}

std::shared_ptr<KLEKey> KeyboardProperties::findKey(int row, int col) const
{
    for (auto &key : _keys)
    {
        if (key->row == row && key->col == col)
        {
            return key;
        }
    }

    return std::shared_ptr<KLEKey>();
}

std::vector<std::shared_ptr<KLEKey> > KeyboardProperties::getKeys() const
{
    return _keys;
}

bool KeyboardProperties::supportsAnimations() const
{
    return false;
}

unsigned int KeyboardProperties::animationCount() const
{
    return 0;
}

QString KeyboardProperties::animationName(unsigned int sector) const
{
    return QString(_("Sector %1").arg(sector));
}

bool KeyboardProperties::supportsNativeAnimationNames() const
{
    return false;
}

QWidget *KeyboardProperties::keyboardSettingsWidget() const
{
    return 0;
}

QWidget *KeyboardProperties::consoleWidget() const
{
    return 0;
}

QWidget *KeyboardProperties::interfaceSettingsDialog() const
{
    return 0;
}

std::shared_ptr<Animator> KeyboardProperties::animator() const
{
    return _animator;
}

QString KeyboardProperties::jsonKeyboardLayout() const
{
    return QString();
}

void KeyboardProperties::parseJSONKeyboardLayout()
{
    int f = 3;
    int f2 = 3;
    QColor color;
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonKeyboardLayout().toUtf8(), &error);

    if (doc.isNull())
    {
        qDebug() << __PRETTY_FUNCTION__ << ": " << error.errorString();
        qDebug() << __PRETTY_FUNCTION__ << ": " << error.offset;
        qDebug() << __PRETTY_FUNCTION__ << ": " << jsonKeyboardLayout()[error.offset];
        qDebug() << __PRETTY_FUNCTION__ << ": " << jsonKeyboardLayout().mid(error.offset-10, 20);
    }

    if (!doc.isArray())
        return;

    QJsonArray rows = doc.array();
    //qDebug() << "rows: " << rows.size();

    double y = 0.0;

    for (int row = 0; row < rows.size(); row++)
    {
        double x = 0.0;
        unsigned int cols = 0;
        unsigned int col = 0;

        QJsonArray columns = rows.at(row).toArray();
        //qDebug() << "columns: " << columns.size();

        double width = 1.0;
        double height = 1.0;

        for (int column = 0; column < columns.size(); column++)
        {
            QJsonValue kleKey = columns.at(column);

            if (kleKey.isObject())
            {
                //qDebug() << "object: " << kleKey;

                QJsonObject kleKeyObj = kleKey.toObject();
                if (kleKeyObj.contains("x"))
                    x += kleKeyObj.value("x").toDouble();
                if (kleKeyObj.contains("y"))
                    y += kleKeyObj.value("y").toDouble();
                if (kleKeyObj.contains("w"))
                    width = kleKeyObj.value("w").toDouble();
                if (kleKeyObj.contains("h"))
                    height = kleKeyObj.value("h").toDouble();
                if (kleKeyObj.contains("c"))
                    color = QColor(kleKeyObj.value("c").toString());
                if (kleKeyObj.contains("f"))
                    f = kleKeyObj.value("f").toInt();
                if (kleKeyObj.contains("f2"))
                    f2 = kleKeyObj.value("f2").toInt();

                //qDebug() << "object: x=" << x << ", y=" << y << ", width=" << width << ", height=" << height;

                //TODO: validate input
            }
            else
            {
                //qDebug() << kleKey;

                std::shared_ptr<KLEKey> key = std::make_shared<KLEKey>();
                key->width = width;
                key->height = height;
                key->x = x;
                key->y = y;
                key->row = row;
                key->codes = kleKey.toString().split("\n");
                key->color = color;
                key->f = f;
                key->f2 = f2;

                _keys.push_back(key);

                //qDebug() << "key " << key->codes.join("|") << " at: " << key->x << "/" << key->y << " " << key->width << "x" << key->height << ", row: " << row;

                // Increment the x position.
                x += width;

                // Increment the number of columns.
                cols ++;

                // Increment the real column index.
                col ++;

                width = 1;
                height = 1;
            }

        }

        // Set the number of keyboard columns and width.
        real_columns = qMax(cols, real_columns);
        this->width = qMax(this->width, x);

        // Increment the y position.
        y ++;
    }

    std::map<QString, bool> posTaken;
    QString posTakenKey("%1,%2");

    for (unsigned int i = 0; i < _keys.size(); ++i)
    {
        auto &key = _keys[i];

        double col = floor((key->x + key->width / 2.0) / this->width * this->real_columns);

        // If the position is already taken, increment the column by one.
        auto taken = posTaken.find(posTakenKey.arg(key->row).arg(col));
        while (taken != posTaken.end())
        {
            col++;
            taken = posTaken.find(posTakenKey.arg(key->row).arg(col));
        }

        // Set the column.
        key->col = col;

        // Save the position as taken.

        posTaken[posTakenKey.arg(key->row).arg(key->col)] = true;

        // If the column number exceeds the number of columns possible, go back and shift everything back by 1.
        if (col >= real_columns)
        {
            // Shift keys until we reach an unclaimed position.
            int prevIndex = i;
            auto prev = _keys[prevIndex --];
            auto taken = posTaken.find(posTakenKey.arg(prev->row).arg(prev->col-1));

            while (taken != posTaken.end()) {
                prev->col --;
                taken = posTaken.find(posTakenKey.arg(prev->row).arg(prev->col-1));
            }
            prev->col --;
        }
    }

    /*
    for (unsigned int i = 0; i < _keys.size(); ++i)
    {
        auto const &key = _keys[i];
        qDebug() << "key " << key->codes.join("|") << " at: " << key->x << "/" << key->y << " " << key->width << "x" << key->height;
    }
    */
}

void KeyboardProperties::prepareSectors()
{

}

std::unique_ptr<KeyboardProperties> KeyboardPropertiesInstance::_properties = std::unique_ptr<KeyboardProperties>(new KeyboardProperties());

KeyboardPropertiesInstance::KeyboardPropertiesInstance()
{

}

void KeyboardPropertiesInstance::setKeyboardProperties(std::unique_ptr<KeyboardProperties> properties)
{
    _properties = std::move(properties);
    _properties->init();
}

const KeyboardProperties *KeyboardPropertiesInstance::properties() const
{
    return _properties.get();
}
