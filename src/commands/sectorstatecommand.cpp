#include "sectorstatecommand.h"

SectorStateCommand::SectorStateCommand(unsigned int sector)
    : _sector(sector)
{

}

unsigned int SectorStateCommand::sector()
{
    return _sector;
}
