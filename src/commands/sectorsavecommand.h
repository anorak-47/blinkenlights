#ifndef SECTORSAVECOMMAND_H
#define SECTORSAVECOMMAND_H

#include "keyboard_command.h"

class SectorSaveCommand : public KeyboardCommand
{
public:
    SectorSaveCommand(unsigned int sector);

    unsigned int sector();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    unsigned int _sector;
};

#endif // SECTORSAVECOMMAND_H
