#include "ledilluminatecommand.h"
#include <QtCore/QDebug>

LedIlluminateCommand::LedIlluminateCommand(unsigned int led, bool illuminate)
    : LedChangeCommand(led), _illuminated(illuminate)
{

}

void LedIlluminateCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setLedIllumination(_led, _illuminated);
}

bool LedIlluminateCommand::illuminated() const
{
    return _illuminated;
}

void LedIlluminateCommand::setIlluminated(bool illuminated)
{
    _illuminated = illuminated;
}

