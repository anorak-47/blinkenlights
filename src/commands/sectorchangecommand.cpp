#include "sectorchangecommand.h"

SectorChangeCommand::SectorChangeCommand(unsigned int sector)
    : _sector(sector)
{

}

unsigned int SectorChangeCommand::sector()
{
    return _sector;
}
