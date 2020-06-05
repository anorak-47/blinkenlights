#include "sectormapsetcommand.h"

SectorMapSetCommand::SectorMapSetCommand(unsigned int map) : _map(map)
{
}

void SectorMapSetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->showMap(_map);
}

unsigned int SectorMapSetCommand::map() const
{
    return _map;
}

void SectorMapSetCommand::setMap(unsigned int map)
{
    _map = map;
}
