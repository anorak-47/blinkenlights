#include "sectorcolorcommand.h"
#include <QtCore/QDebug>

SectorColorCommand::SectorColorCommand(unsigned int sector, const QColor &color)
    : SectorChangeCommand(sector), _color(color)
{

}

QColor SectorColorCommand::color() const
{
    return _color;
}

void SectorColorCommand::setColor(const QColor &color)
{
    _color = color;
}

void SectorColorCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setSectorColor(_sector, _color);
}
