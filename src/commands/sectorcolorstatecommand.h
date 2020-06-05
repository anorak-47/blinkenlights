#ifndef SECTORCOLORSTATECOMMAND_H
#define SECTORCOLORSTATECOMMAND_H

#include "sectorstatecommand.h"
#include <QtGui/QColor>

class SectorColorStateCommand : public SectorStateCommand
{
public:
    SectorColorStateCommand(unsigned int sector, std::function<void(unsigned int, QColor const&)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

private:
    std::function<void(unsigned int, QColor const&)> _func;
};

#endif // SECTORCOLORSTATECOMMAND_H
