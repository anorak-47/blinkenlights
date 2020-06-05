#include "debugconfigsavecommand.h"

DebugConfigSaveCommand::DebugConfigSaveCommand()
{

}

void DebugConfigSaveCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->saveDebugConfig();
}
