#pragma once
#include "keyboard_interface.h"
#include <memory>

class KeyboardCommand
{
public:
    KeyboardCommand();
    virtual ~KeyboardCommand();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) = 0;
};
