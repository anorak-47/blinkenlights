#include "sectorsavecommand.h"

SectorSaveCommand::SectorSaveCommand(unsigned int sector)
    : _sector(sector)
{

}

unsigned int SectorSaveCommand::sector()
{
    return _sector;
}

void SectorSaveCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->saveSector(_sector);
}
