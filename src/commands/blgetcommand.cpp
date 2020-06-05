#include "blgetcommand.h"

BlGetCommand::BlGetCommand(std::function<void(bool, unsigned int)> func) : _func(func)
{
}

void BlGetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    bool enabled;
    unsigned int level;
    protocol->getBacklightState(enabled, level);
    _func(enabled, level);
}
