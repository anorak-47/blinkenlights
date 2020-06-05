#include "ledbrightnesscommand.h"
#include <QtCore/QDebug>

LedBrightnessCommand::LedBrightnessCommand(unsigned int led, unsigned int brightness)
    : LedChangeCommand(led), _brightness(brightness)
{

}

void LedBrightnessCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setLedBrightness(_led, _brightness);
}

unsigned int LedBrightnessCommand::brightness() const
{
    return _brightness;
}

void LedBrightnessCommand::setBrightness(unsigned int brightness)
{
    _brightness = brightness;
}
