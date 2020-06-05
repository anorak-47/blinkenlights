#include "blsetcommand.h"
#include <QtCore/QDebug>

BlSetCommand::BlSetCommand(bool enabled, unsigned int level)
    : KeyboardCommand(), _enabled(enabled), _level(level)
{

}

void BlSetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setBacklightState(_enabled, _level);
}
