#include "ledsavecommand.h"
#include <QtCore/QDebug>

LedSaveCommand::LedSaveCommand()
{

}

void LedSaveCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->saveLedConfig();
}
