#pragma once
#include "keyboard_command.h"

class BlSetCommand : public KeyboardCommand
{
public:
    BlSetCommand(bool enabled, unsigned int level);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    bool _enabled;
    unsigned int _level;
};
