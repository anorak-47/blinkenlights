#include "keymapconfigsetcommand.h"

KeymapConfigSetCommand::KeymapConfigSetCommand(unsigned int config)
    : _config(config)
{

}

void KeymapConfigSetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setKeymapConfig(_config);
}

unsigned int KeymapConfigSetCommand::config()
{
    return _config;
}
