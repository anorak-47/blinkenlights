#ifndef DebugConfigSaveCommand_H
#define DebugConfigSaveCommand_H

#include "keyboard_command.h"

class DebugConfigSaveCommand : public KeyboardCommand
{
public:
    DebugConfigSaveCommand();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;
};

#endif // DebugConfigSaveCommand_H
