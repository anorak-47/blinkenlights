#ifndef SECTORILLUMINATIONSTATECOMMAND_H
#define SECTORILLUMINATIONSTATECOMMAND_H

#include "sectorstatecommand.h"

class SectorIlluminationStateCommand : public SectorStateCommand
{
public:
    SectorIlluminationStateCommand(unsigned int sector, std::function<void(unsigned int, bool)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol);

private:
    std::function<void(unsigned int, bool)> _func;
};

#endif // SECTORILLUMINATIONSTATECOMMAND_H
