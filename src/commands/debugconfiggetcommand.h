#ifndef DebugConfigGetCommand_H
#define DebugConfigGetCommand_H

#include "keyboard_command.h"
#include <functional>

class DebugConfigGetCommand : public KeyboardCommand
{
public:
    DebugConfigGetCommand(std::function<void(unsigned int)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(unsigned int)> _func;
};

#endif // DebugConfigGetCommand_H
