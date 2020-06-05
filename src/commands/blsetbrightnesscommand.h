#pragma once
#include "keyboard_command.h"

class BlSetBrightnessCommand : public KeyboardCommand
{
public:
    BlSetBrightnessCommand(unsigned int brightness);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    unsigned int _brightness;
};
