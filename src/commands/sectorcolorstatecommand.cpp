#include "sectorcolorstatecommand.h"
#include <QtCore/QDebug>

SectorColorStateCommand::SectorColorStateCommand(unsigned int sector, std::function<void(unsigned int sector, QColor const &color)> func)
    : SectorStateCommand(sector), _func(func)
{

}

void SectorColorStateCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    QColor color = protocol->getSectorColor(_sector);
    _func(_sector, color);
}
