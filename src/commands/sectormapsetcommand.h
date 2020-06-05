#ifndef SectorMapSetCommand_H
#define SectorMapSetCommand_H

#include "keyboard_command.h"
#include "keyboard_protocol_interface.h"
#include <functional>

class SectorMapSetCommand : public KeyboardCommand
{
public:
    SectorMapSetCommand(unsigned int map);

    void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int map() const;
    void setMap(unsigned int map);

protected:
    unsigned int _map;
};

#endif // SectorMapSetCommand_H
