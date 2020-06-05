#include "debugconfigsetcommand.h"

DebugConfigSetCommand::DebugConfigSetCommand(unsigned int config)
    : _config(config)
{

}

void DebugConfigSetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setDebugConfig(_config);
}

unsigned int DebugConfigSetCommand::config()
{
    return _config;
}
