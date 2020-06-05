#include "keymapconfigsavecommand.h"

KeymapConfigSaveCommand::KeymapConfigSaveCommand()
{

}

void KeymapConfigSaveCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->saveKeymapConfig();
}
