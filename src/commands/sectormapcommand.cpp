#include "sectormapcommand.h"
#include <QtCore/QDebug>

SectorMapCommand::SectorMapCommand(std::unique_ptr<s_map> map)
    : _map(std::move(map))
{
}

void SectorMapCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setMap(std::move(_map));
}
