#include "sectormapgetcommand.h"

SectorMapGetCommand::SectorMapGetCommand(unsigned int map, std::function<void(std::shared_ptr<s_map>)> func)
    : _map(map), _func(func)
{
}

void SectorMapGetCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
     std::shared_ptr<s_map> map = protocol->getMap(_map);
    _func(map);
}

unsigned int SectorMapGetCommand::map() const
{
    return _map;
}

void SectorMapGetCommand::setMap(unsigned int map)
{
    _map = map;
}
