#include "blgetbrightnesscommand.h"

BlGetBrightnessCommand::BlGetBrightnessCommand(std::function<void(unsigned int)> func) : _func(func)
{
}

void BlGetBrightnessCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    unsigned int bri = protocol->getBacklightGlobalBrightness();
    _func(bri);
}
