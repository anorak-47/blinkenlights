#include "sectorilluminatecommand.h"
#include <QtCore/QDebug>

SectorIlluminateCommand::SectorIlluminateCommand(unsigned int sector, bool illuminate)
    : SectorChangeCommand(sector), _illuminate(illuminate)
{

}

bool SectorIlluminateCommand::illuminate() const
{
    return _illuminate;
}

void SectorIlluminateCommand::setIlluminate(bool illuminate)
{
    _illuminate = illuminate;
}

void SectorIlluminateCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    qDebug() << "++ SectorIlluminateCommand::exec -----------";
    protocol->setSectorIllumination(_sector, _illuminate);
    qDebug() << "-- SectorIlluminateCommand::exec -----------";
}
