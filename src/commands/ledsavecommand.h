#ifndef LedSaveCommand_H_
#define LedSaveCommand_H_

#include "keyboard_command.h"

class LedSaveCommand : public KeyboardCommand
{
public:
    LedSaveCommand();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;
};

#endif // LedSaveCommand_H_
