#include "ledgetpwmcommand.h"

LedGetPwmCommand::LedGetPwmCommand(unsigned int led, std::function<void(unsigned int, unsigned int)> func)
    : LedChangeCommand(led), _func(func)
{

}

void LedGetPwmCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    unsigned int brightness = protocol->getLedBrightness(_led);
    _func(_led, brightness);
}
