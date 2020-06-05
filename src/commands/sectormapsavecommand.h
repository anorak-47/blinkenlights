#ifndef SectorSaveMapCommand_H
#define SectorSaveMapCommand_H

#include "keyboard_command.h"

class SectorMapSaveCommand : public KeyboardCommand
{
public:
    SectorMapSaveCommand(unsigned int map);

    unsigned int map();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    unsigned int _map;
};

#endif // SectorSaveMapCommand_H
