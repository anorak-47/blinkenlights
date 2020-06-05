#include "sleepledcommand.h"

SleepLedCommand::SleepLedCommand(bool on)
    : _on(on)
{

}

bool SleepLedCommand::getOn() const
{
    return _on;
}

void SleepLedCommand::setOn(bool value)
{
    _on = value;
}

void SleepLedCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setSleepLed(_on);
}
