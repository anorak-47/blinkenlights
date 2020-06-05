#include "keymapconfiggetcommand.h"

KeymapConfigGetCommand::KeymapConfigGetCommand(std::function<void(unsigned int)> func)
    : _func(func)
{

}

void KeymapConfigGetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    unsigned int config = protocol->getKeymapConfig();
    _func(config);
}
