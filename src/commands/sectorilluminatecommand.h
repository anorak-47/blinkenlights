#ifndef SECTORILLUMINATECOMMAND_H
#define SECTORILLUMINATECOMMAND_H

#include "sectorchangecommand.h"

class SectorIlluminateCommand : public SectorChangeCommand
{
public:
    SectorIlluminateCommand(unsigned int sector, bool illuminate);

    bool illuminate() const;
    void setIlluminate(bool illuminate);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    bool _illuminate;
};

#endif // SECTORILLUMINATECOMMAND_H
