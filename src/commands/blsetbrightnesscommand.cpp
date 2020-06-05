#include "blsetbrightnesscommand.h"
#include <QtCore/QDebug>

BlSetBrightnessCommand::BlSetBrightnessCommand(unsigned int brightness) : KeyboardCommand(), _brightness(brightness)
{
}

void BlSetBrightnessCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setBacklightGlobalBrightness(_brightness);
}
