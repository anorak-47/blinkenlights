#pragma once
#include "keyboard_command.h"
#include <functional>

class BlGetBrightnessCommand : public KeyboardCommand
{
public:
    BlGetBrightnessCommand(std::function<void(unsigned int)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(unsigned int)> _func;
};
