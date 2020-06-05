#ifndef SECTORMAPCOMMAND_H
#define SECTORMAPCOMMAND_H

#include "keyboard_command.h"
#include "keyboard_protocol_interface.h"
#include <QtGui/QColor>
#include <vector>

class SectorMapCommand : public KeyboardCommand
{
public:
    SectorMapCommand(std::unique_ptr<s_map> map);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::unique_ptr<struct s_map> _map;
};

#endif // SECTORMAPCOMMAND_H
