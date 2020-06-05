#include "ledchangecommand.h"

LedChangeCommand::LedChangeCommand(unsigned int led)
    : _led(led)
{

}

unsigned int LedChangeCommand::led()
{
    return _led;
}
