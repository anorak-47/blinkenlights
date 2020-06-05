#ifndef SECTORCHANGECOMMAND_H
#define SECTORCHANGECOMMAND_H

#include "keyboard_command.h"

class SectorChangeCommand : public KeyboardCommand
{
public:
    SectorChangeCommand(unsigned int sector);

    unsigned int sector();

protected:
    unsigned int _sector;
};

#endif // SECTORCHANGECOMMAND_H
