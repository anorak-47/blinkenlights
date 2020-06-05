#ifndef SECTORSTATECOMMAND_H
#define SECTORSTATECOMMAND_H

#include "keyboard_command.h"
#include <functional>

class SectorStateCommand : public KeyboardCommand
{
public:
    SectorStateCommand(unsigned int sector);

    unsigned int sector();

protected:
    unsigned int _sector;
};

#endif // SECTORSTATECOMMAND_H
