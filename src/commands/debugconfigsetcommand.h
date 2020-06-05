#ifndef DebugConfigSetCommand_H
#define DebugConfigSetCommand_H

#include "keyboard_command.h"

class DebugConfigSetCommand : public KeyboardCommand
{
public:
    DebugConfigSetCommand(unsigned int config);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int config();

protected:
    unsigned int _config;
};

#endif // DebugConfigSetCommand_H
