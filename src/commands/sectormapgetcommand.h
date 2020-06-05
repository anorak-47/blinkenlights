#ifndef SECTORMAPGETCOMMAND_H
#define SECTORMAPGETCOMMAND_H

#include "keyboard_command.h"
#include "keyboard_protocol_interface.h"
#include <functional>

class SectorMapGetCommand : public KeyboardCommand
{
public:
    SectorMapGetCommand(unsigned int map, std::function<void(std::shared_ptr<s_map>)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int map() const;
    void setMap(unsigned int map);

protected:
    unsigned int _map;
    std::function<void(std::shared_ptr<struct s_map>)> _func;
};

#endif // SECTORMAPGETCOMMAND_H
