#include "sectormapsavecommand.h"

SectorMapSaveCommand::SectorMapSaveCommand(unsigned int map) : _map(map)
{
}

unsigned int SectorMapSaveCommand::map()
{
    return _map;
}

void SectorMapSaveCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->saveMap(_map);
}
