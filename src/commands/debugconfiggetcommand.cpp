#include "debugconfiggetcommand.h"

DebugConfigGetCommand::DebugConfigGetCommand(std::function<void(unsigned int)> func)
    : _func(func)
{

}

void DebugConfigGetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    unsigned int config = protocol->getDebugConfig();
    _func(config);
}
