#ifndef BootloaderStartCommand_H
#define BootloaderStartCommand_H

#include "keyboard_command.h"

class BootloaderStartCommand : public KeyboardCommand
{
public:
    BootloaderStartCommand();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;
};

#endif // BootloaderStartCommand_H
