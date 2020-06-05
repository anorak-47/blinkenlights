#include "keyboard_properties_jellowcake.h"
#include "keyboard_interface_jellowcake.h"
#include "keyboard_console_jellowcake.h"
#include "keyboard_settings_jellowcake.h"
#include "keyboard_protocol_interface.h"
#include "common.h"
#include "serial_settings_dialog.h"
#include "compat.h"
#include <QtCore/QDebug>
#include <vector>
#include <memory>

KeyboardPropertiesJellowcake::KeyboardPropertiesJellowcake(const s_device_info &deviceInfo)
    : KeyboardProperties(deviceInfo)
{
}

void KeyboardPropertiesJellowcake::init()
{
    // qDebug() << __PRETTY_FUNCTION__;

    //_animator = std::make_shared<AnimatorJellowcake>();
    // initialize_compat_jellowcake({});

    parseJSONKeyboardLayout();
    prepareSectors();
}

QString KeyboardPropertiesJellowcake::name() const
{
    return "Jellowcake";
}

QString KeyboardPropertiesJellowcake::vendor() const
{
    return "anorak";
}

QString KeyboardPropertiesJellowcake::description() const
{
    return _("A left hand dactyl manuform keyboard with RGB light for gaming");
}

unsigned int KeyboardPropertiesJellowcake::columns() const
{
    return 17;
}

unsigned int KeyboardPropertiesJellowcake::rows() const
{
    return 5;
}

bool KeyboardPropertiesJellowcake::supportsColorMaps() const
{
    return true;
}

unsigned int KeyboardPropertiesJellowcake::colorMapCount() const
{
    return 8;
}

QString KeyboardPropertiesJellowcake::colorMapName(unsigned int mapid) const
{
    return _("Custom Color Map %1").arg(mapid);
}

bool KeyboardPropertiesJellowcake::supportsDifferentialMapUpdates() const
{
    return true;
}

bool KeyboardPropertiesJellowcake::supportsRowBasedMapUpdates() const
{
    return true;
}

bool KeyboardPropertiesJellowcake::supportsSectors() const
{
    return false;
}

unsigned int KeyboardPropertiesJellowcake::sectorCount() const
{
    return 0;
}

QString KeyboardPropertiesJellowcake::sectorName(unsigned int sector) const
{
    return {};
}

QString KeyboardPropertiesJellowcake::jsonKeyboardLayout() const
{
    static QString layout =
        "[[{\"c\":\"#7b9b48\",\"t\":\"#e4dedd\",\"p\":\"DSA\",\"a\":7,\"f\":6},\"Esc\",\"Any\",{\"x\":0.5,\"c\":\"#"
        "733636\"},\"F1\",\"F2\",\"F3\",\"F4\",{\"x\":0.25,\"c\":\"#483527\"},\"F5\",\"F6\",\"F7\",\"F8\",{\"x\":0.25,"
        "\"c\":\"#733636\"},\"F9\",\"F10\",\"F11\",\"F12\",{\"x\":0.25,\"c\":\"#483527\",\"a\":4,\"f\":4},\"Druck\nS-"
        "Abf\",{\"f\":3},\"Scroll\n\n\n\n\n\nLock\",{\"f\":4},\"Pause\nUntbr\"],[{\"y\":0.5,\"f\":6.0},\"°\n^\",\"!"
        "\n1\",\"\\\"\n2\",\"§\n3\",\"$\n4\",\"%\n5\",\"&\n6\",\"/"
        "\n7\n\n{\",\"(\n8\n\n[\",\")\n9\n\n]\",\"=\n0\n\n}\",\"?\nß\n\nBS\",\"UQ\nLQ\",{\"c\":\"#733636\",\"a\":7,"
        "\"w\":2},\"Backspace\",{\"x\":0.25,\"c\":\"#483527\",\"a\":4,\"f\":4},\"Einfg\",\"Pos1\",\"PgUp\"],[{\"c\":\"#"
        "733636\",\"a\":7,\"f\":6,\"w\":1.5},\"Tab\",{\"c\":\"#483527\"},\"Q\",\"W\",\"E\",\"R\",\"T\",\"Y\",\"U\","
        "\"I\",\"O\",\"P\",\"Ü\",{\"a\":4},\"*\n+\n\n~\",{\"x\":0.25,\"c\":\"#733636\",\"a\":7,\"w\":1.25,\"h\":2,"
        "\"w2\":1.5,\"h2\":1,\"x2\":-0.25},\"Enter\",{\"x\":0.25,\"c\":\"#483527\",\"a\":4,\"f\":4},\"Entf\",\"Ende\","
        "\"PgDn\"],[{\"c\":\"#733636\",\"a\":7,\"f\":6,\"w\":1.75},\"Caps "
        "Lock\",{\"c\":\"#483527\"},\"A\",\"S\",\"D\",\"F\",\"G\",\"H\",\"J\",\"K\",\"L\",\"Ö\",\"Ä\",{\"a\":4},"
        "\"SQw\n#\"],[{\"c\":\"#733636\",\"a\":7,\"w\":1.25},\"Shift\",{\"c\":\"#483527\",\"a\":4},\">\n<\n\n|\",{"
        "\"a\":7},\"Z\",\"X\",\"C\",\"V\",\"B\",\"N\",\"M\",{\"a\":4},\";\n,\",\":\n.\",\"&\n-\",{\"c\":\"#733636\","
        "\"a\":7,\"w\":2.75},\"Shift\",{\"x\":0.25,\"c\":\"#7b9b48\",\"f\":4},\"Copy\",{\"c\":\"#483527\"},\"Up\",{"
        "\"c\":\"#7b9b48\"},\"Paste\"],[{\"c\":\"#733636\",\"f\":6,\"w\":1.25},\"Ctrl\",{\"w\":1.25},\"Meta\",{\"w\":1."
        "25},\"Alt\",{\"c\":\"#483527\",\"w\":6.25},\"Space\",{\"c\":\"#733636\",\"w\":1.25},\"AltGr\",{\"w\":1.25},"
        "\"Gui\",{\"w\":1.25},\"FN0\",{\"w\":1.25},\"Ctrl\",{\"x\":0.25,\"c\":\"#483527\",\"f\":4},\"Left\",\"Down\","
        "\"Right\"]]";
    return layout;
}

void KeyboardPropertiesJellowcake::prepareSectors()
{
    struct sectorKey
    {
        int row;
        int col;
    };
    std::vector<std::vector<sectorKey>> sectorDefinition{
        {{2, 2}, {3, 2}, {3, 3}, {3, 4}},
        {{2, 0},  {3, 0},  {4, 0},  {5, 0},  {5, 1},  {5, 2},  {5, 6},  {5, 9},  {5, 11},
         {5, 12}, {5, 13}, {4, 12}, {2, 13}, {1, 13}, {1, 14}, {1, 15}, {1, 16}, {2, 14},
         {2, 15}, {2, 16}, {0, 14}, {0, 15}, {0, 16}, {4, 14}, {4, 16}},
        {{0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}},
        {{4, 15}, {5, 14}, {5, 15}, {5, 16}},
        {{1, 0},  {1, 1},  {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 6}, {1, 7},  {1, 8},  {1, 9},  {1, 10},
         {1, 11}, {1, 12}, {2, 1}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7},  {2, 8},  {2, 9},  {2, 10},
         {2, 11}, {2, 12}, {3, 5}, {3, 6}, {3, 7}, {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13},
         {4, 1},  {4, 2},  {4, 3}, {4, 4}, {4, 5}, {4, 6}, {4, 7}, {4, 8},  {4, 9},  {4, 10}, {4, 11}},
        {{0, 0}, {0, 1}}};

    _sectors.resize(sectorCount());
    unsigned int currentSector = 0;

    for (auto &sector : sectorDefinition)
    {
        for (auto &key : sector)
        {
            _sectors[currentSector].push_back(findKey(key.row, key.col));
        }

        currentSector++;
    }
}

std::vector<std::shared_ptr<KLEKey>> KeyboardPropertiesJellowcake::getSectorKeys(unsigned int sector) const
{
    if (sector >= sectorCount())
        return std::vector<std::shared_ptr<KLEKey>>();

    return _sectors[sector];
}

bool KeyboardPropertiesJellowcake::supportsAnimations() const
{
    return true;
}

unsigned int KeyboardPropertiesJellowcake::animationCount() const
{
    return 0;
}

QString KeyboardPropertiesJellowcake::animationName(unsigned int) const
{
    return QString();
}

bool KeyboardPropertiesJellowcake::supportsNativeAnimationNames() const
{
    return true;
}

QWidget *KeyboardPropertiesJellowcake::keyboardSettingsWidget() const
{
    return new KeyboardSettingsJellowcake(_deviceInfo);
}

QWidget *KeyboardPropertiesJellowcake::interfaceSettingsDialog() const
{
    return new SettingsDialog();
}

QWidget *KeyboardPropertiesJellowcake::consoleWidget() const
{
    KeyboardConsoleJellowcake *console = new KeyboardConsoleJellowcake();

    std::shared_ptr<KeyboardInterface> protocol = KeyboardCommandTransferInterface().getProtocol();

    QObject::connect(protocol.get(), SIGNAL(signalRequest(QString)), console, SLOT(on_request(QString)));
    QObject::connect(protocol.get(), SIGNAL(signalResponse(QString)), console, SLOT(on_response(QString)));
    QObject::connect(protocol.get(), SIGNAL(signalWarning(QString)), console, SLOT(on_warning(QString)));
    QObject::connect(protocol.get(), SIGNAL(signalError(QString)), console, SLOT(on_error(QString)));
    QObject::connect(protocol.get(), SIGNAL(signalMessage(QString)), console, SLOT(on_message(QString)));
    QObject::connect(console, SIGNAL(signalRequest(QString)), protocol.get(), SLOT(on_request(QString)));

    return console;
}
