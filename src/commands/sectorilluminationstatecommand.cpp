#include "sectorilluminationstatecommand.h"
#include <QtCore/QDebug>

SectorIlluminationStateCommand::SectorIlluminationStateCommand(unsigned int sector, std::function<void(unsigned int, bool)> func)
    : SectorStateCommand(sector), _func(func)
{
}

void SectorIlluminationStateCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    bool illuminated = protocol->getSectorIllumination(_sector);
    _func(_sector, illuminated);
}
