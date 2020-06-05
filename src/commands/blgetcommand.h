#pragma once
#include "keyboard_command.h"
#include <functional>

class BlGetCommand : public KeyboardCommand
{
public:
    BlGetCommand(std::function<void(bool, unsigned int)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(bool, unsigned int)> _func;
};
